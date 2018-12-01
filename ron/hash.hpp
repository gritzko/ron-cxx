#ifndef RON_MERKLE_HPP
#define RON_MERKLE_HPP
#include <botan/sha2_64.h>
#include "uuid.hpp"
#include "status.hpp"
#include "op.hpp"


namespace ron {

    // Let's use SHA512/256 here because it is reasonably fast
    // https://www.cryptopp.com/benchmarks.html
    // and reasonably secure, apparently.
    typedef uint8_t SHA2[32];

    std::string hex(const SHA2& hash) {
        std::string ret;
        ret.reserve(sizeof(SHA2)<<1);
        for(int j = 0; j < 32; j++) {
            ret.push_back(HEX_PUNCT[hash[j]>>4]);
            ret.push_back(HEX_PUNCT[hash[j]&0xf]);
        }
        return ret;
    }

    // a binary stream of RON primitives
    template<typename sink_t>
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
            return Write(slice_t{(char*)&data, sizeof(data)});
        }
        inline Status WriteUuid(const Uuid& uuid) {
            return WriteAtom(uuid);
        }
        inline Status WriteAtomRangeless(const Atom& atom) {
            uint64pair tmp{atom.value().be(), htobe64(atom.origin()._64&Word::FLAG_BITS)};
            return Write(slice_t{(char*)&tmp, sizeof(uint64pair)});
        }
        inline void close (void* result) {
            sink_.final((uint8_t*)result);
        }
    };

    typedef Stream<Botan::SHA_512_256> SHA2Stream;


    /* TODO HASHING MEGACOMMIT
     * [x] lww - roots - method
     * [x] write to a file, check (test)
     * [x] compare rhash --sha3-256
     * [x] string conversions (base64)
     * [ ] utf-8 parsing, tests
     * */
    template <typename Frame, typename SomeStream>
    void WriteOpHashable(
            const typename Frame::Cursor& cursor,
            SomeStream& stream,
            const SHA2& prev_hash,
            const SHA2& ref_hash) {
        const Op& op = cursor.op();
        stream.WriteUuid(op.id());
        stream.WriteHash(prev_hash);
        stream.WriteUuid(op.ref());
        stream.WriteHash(ref_hash);
        for(fsize_t i=2; i<op.size(); i++) {
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

    void hash_uuid(const Uuid& uuid, SHA2& hash) {
        SHA2Stream stream;
        stream.WriteUuid(uuid);
        stream.close(hash);
    }

};

#endif