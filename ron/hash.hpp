#ifndef RON_MERKLE_HPP
#define RON_MERKLE_HPP
#include <botan/sha2_64.h>
#include "encdec.hpp"
#include "op.hpp"
#include "status.hpp"
#include "uuid.hpp"

namespace ron {

/** Let's use SHA512/256 here because it is reasonably fast
 * https://www.cryptopp.com/benchmarks.html
 * and reasonably secure, apparently. */
struct SHA2 {
    static constexpr size_t SIZE = 32;
    static constexpr size_t BIT_SIZE = SIZE*8;
    // defined length, 0 to SIZE
    uint32_t known_bits_; // FIXME use string
    uint8_t bits_[SIZE];
    static constexpr size_t HEX_SIZE = SIZE * 8 / 4;
    static constexpr size_t BASE64_SIZE = SIZE * 8 / 6 + 1;
    SHA2()
        : bits_{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          known_bits_{0} {}
    /** my motivation for using Base64: Base64 is less clutter.
     * noone is going to spell hashes on the phone anyway.
     * @author gritzko  */
    explicit SHA2(slice_t base64) {  // TODO sizes!
        assert(base64.size_ == BASE64_SIZE);
        decode<6, ABC64>(bits_, base64.buf_, BIT_SIZE);
    }

    inline explicit SHA2(const Uuid& uuid);

    inline SHA2(const SHA2& one, const SHA2& two);

    template <typename Cursor>
    SHA2(Cursor& cur, const SHA2& prev, const SHA2& ref);

    static bool valid(slice_t base64) {
        if (base64.size_ != BASE64_SIZE) return false;
        for (int i = 0; i < BASE64_SIZE; i++)
            if (ABC64[base64[i]] < 0) return false;
        return (ABC64[base64[BASE64_SIZE - 1]] & 3) == 0;
    }

    inline bool defined() const { return known_bits_ > 0; }
    bool operator==(const SHA2& b) const {
        return memcmp(bits_, b.bits_, SIZE) == 0;
    }

    bool operator!=(const SHA2& b) const { return !(*this == b); }
    inline bool matches(const SHA2& b) const {
        int bits = std::min(known_bits_, b.known_bits_);
        int bytes = bits >> 3;
        if (memcmp(bits_, b.bits_, size_t(bytes)) != 0) return false;
        int tail = bits & 7;
        if (tail) {
            uint8_t mine = bits_[bytes] >> (8 - tail);
            uint8_t theirs = b.bits_[bytes] >> (8 - tail);
            return mine == theirs;
        }
        return true;
    }
    std::string hex() const {
        char data[HEX_SIZE];
        encode<4, HEX_PUNCT>(data, bits_, SIZE*8);
        return std::string{data, HEX_SIZE};
    }

    static bool ParseHex(SHA2& ret, const std::string& hash) {
        assert(hash.size() <= HEX_SIZE);
        ret.known_bits_ = uint32_t(hash.size()) << 2;
        if (ret.known_bits_>BIT_SIZE) ret.known_bits_ = BIT_SIZE;
        return decode<4, ABC16>(ret.bits_, hash.data(), ret.known_bits_);
    }

    static bool ParseBase64(SHA2& ret, const std::string& hash) {
        assert(hash.size() <= BASE64_SIZE);
        ret.known_bits_ = uint32_t(hash.size()) * 6;
        if (ret.known_bits_>BIT_SIZE) ret.known_bits_ = BIT_SIZE;
        return decode<6, ABC64>(ret.bits_, hash.data(), ret.known_bits_);
    }
    std::string base64() const {
        char data[BASE64_SIZE];
        encode<6, BASE_PUNCT>(data, bits_, BIT_SIZE);
        return std::string{data, BASE64_SIZE};
    }
};

// a binary stream of RON primitives
template <typename sink_t>
struct Stream {
    sink_t sink_;
    Stream() : sink_{} {}
    Stream(sink_t sink) : sink_{sink} {}
    Status Write(slice_t data) {
        sink_.update((uint8_t*)data.buf_, data.size_);
        return Status::OK;
    }
    inline Status WriteAtom(const Atom& atom) {
        uint64pair tmp{atom.value().be(), atom.origin().be()};
        return Write(slice_t{(char*)&tmp, sizeof(uint64pair)});
    }
    inline Status WriteHash(const SHA2& data) {
        return Write(slice_t{(char*)data.bits_, SHA2::SIZE});
    }
    inline Status WriteUuid(const Uuid& uuid) { return WriteAtom(uuid); }
    inline Status WriteAtomRangeless(const Atom& atom) {
        uint64pair tmp{atom.value().be(),
                       htobe64(atom.origin()._64 & Word::FLAG_BITS)};
        return Write(slice_t{(char*)&tmp, sizeof(uint64pair)});
    }
    inline void close(void* result) { sink_.final((uint8_t*)result); }
};

typedef Stream<Botan::SHA_512_256> SHA2Stream;

template <typename Cursor, typename SomeStream>
void WriteOpHashable(const Cursor& cursor, SomeStream& stream,
                     const SHA2& prev_hash, const SHA2& ref_hash) {
    const Op& op = cursor.op();
    stream.WriteUuid(op.id());
    stream.WriteHash(prev_hash);
    stream.WriteUuid(op.ref());
    stream.WriteHash(ref_hash);
    for (fsize_t i = 2; i < op.size(); i++) {
        const Atom& atom = op.atom(i);
        switch (atom.type()) {
            case UUID:
                stream.WriteAtom(atom);
                break;
            case INT:
            case FLOAT:
                stream.WriteAtomRangeless(atom);
                break;
            case STRING:
                stream.WriteAtomRangeless(atom);
                // TODO: slice_t, no alloc
                stream.Write(cursor.string(i));
                break;
        }
    }
}

SHA2::SHA2(const Uuid& uuid) : known_bits_{SIZE << 3} {
    SHA2Stream stream;
    stream.WriteUuid(uuid);
    stream.close(bits_);
}

SHA2::SHA2(const SHA2& one, const SHA2& two) : known_bits_{SIZE << 3} {
    SHA2Stream stream;
    stream.WriteHash(one);
    stream.WriteHash(two);
    stream.close(bits_);
}

template <typename Frame>
inline void hash_op(const typename Frame::Cursor& cur, SHA2& hash,
                    const SHA2& prev_hash, const SHA2& ref_hash) {
    SHA2Stream stream;
    WriteOpHashable<typename Frame::Cursor, SHA2Stream>(cur, stream, prev_hash,
                                                        ref_hash);
    stream.close(hash.bits_);
}

template <typename Cursor>
SHA2::SHA2(Cursor& cur, const SHA2& prev, const SHA2& ref) : known_bits_{SIZE} {
    SHA2Stream stream;
    WriteOpHashable<Cursor, SHA2Stream>(cur, stream, prev, ref);
    stream.close(bits_);
}

};  // namespace ron

#endif
