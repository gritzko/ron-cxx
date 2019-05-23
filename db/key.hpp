#ifndef CPP_KEY_HPP
#define CPP_KEY_HPP

#include "../ron/form.hpp"
#include "../ron/uuid.hpp"
#ifndef NDEBUG
#include <iostream>
#endif

namespace ron {

/**
 *  A 128-bit key has the UUID and the RDT/reducer id.
 *  UUID origin flag bits are always RON_UUID/TIME, hence reused for the RDT
 *  id. The bit layout is: origin(60) variety(4) value(60) reducer(4)
 *  * yarns, scans
 *  * unknown-type fetch
 *  * prefix compression: value tail bits, reducer bits
 *  * same cf for chains(yarns), objects
 *  * origin-locality
 */
struct Key {
    uint64pair bits;

    static constexpr size_t SIZE = sizeof(bits);

    Key() : bits{0, 0} {}

    Key(const Key& b) = default;

    explicit Key(uint64pair b) : bits{b} {}  // NOLINT

    Key(Uuid id, FORM form) {
        bits.first = id.origin._64 & Word::PAYLOAD_BITS;
        bits.second = id.value._64 & Word::PAYLOAD_BITS;
        bits.second |= bits.first << 60;
        bits.first >>= 4;
        bits.first |= uint64_t(form) << 56;
    }

    Key(Uuid id, Uuid form) : Key{id, uuid2form(form)} {}

    inline uint64pair be() const {
        return uint64pair{htobe64(bits.first), htobe64(bits.second)};
    }

    static Key be(Slice data) {
        assert(data.size() == sizeof(bits));
        auto b = *reinterpret_cast<const uint64pair*>(data.begin());
        return Key{uint64pair{be64toh(b.first), be64toh(b.second)}};
    }

    inline bool operator<(const Key& b) const { return bits < b.bits; }
    inline bool operator>(const Key& b) const { return bits > b.bits; }
    inline bool operator==(const Key& b) const { return bits == b.bits; }
    inline bool operator!=(const Key& b) const { return bits != b.bits; }

    inline FORM form() const { return FORM(bits.first >> 56); }

    inline Uuid id(enum UUID uuid_flags = TIME) const {
        return Uuid{(bits.second & Word::PAYLOAD_BITS),
                    ((bits.first << 4) & Word::PAYLOAD_BITS) |
                        (bits.second >> 60) | (uint64_t(uuid_flags) << 60)};
    }

    String str() const {
        return '*' + form2uuid(form()).str() + '#' + id().str();
    }

    static const Key ZERO;
    static const Key END;

    /** Trace data events, see LOG() */
    static bool trace_by_key;
};

inline void LOG(char code, const Key& key, const String& value) {
#ifndef NDEBUG
    if (Key::trace_by_key) {
        std::cerr << code << '\t' << key.str() << '\t'
                  << (value.empty() ? "-\n" : value);
    }
#endif
}

}  // namespace ron

#endif  // CPP_KEY_HPP
