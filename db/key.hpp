#ifndef CPP_KEY_HPP
#define CPP_KEY_HPP

#include <rocksdb/slice.h>
#include "rdt/rdt.hpp"
#include "ron/uuid.hpp"

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

    Key(const Uuid& id, RDT rdt)
        : bits{
              htobe64((id.origin()._64 << Word::FBS) | id.variety()),
              htobe64((id.value()._64 << Word::FBS) | rdt),
          } {}

    inline RDT rdt() const { return RDT(be64toh(bits.second) & 0xf); }

    inline Uuid id() const {
        uint64pair h{be64toh(bits.first), be64toh(bits.second)};
        return Uuid{
            (h.second >> Word::FBS) | ((h.first & 0xf) << Word::PBS),
            (h.first >> Word::FBS) | (uint64_t(FLAGS::TIME_UUID) << Word::PBS)};
    }

    Key(rocksdb::Slice data)
        : bits{*(uint64_t*)data.data_,
               *(uint64_t*)(data.data_ + sizeof(uint64_t))} {
        assert(data.size() == sizeof(Key));
    }

    operator rocksdb::Slice() const {
        return rocksdb::Slice{(char*)this, sizeof(Key)};
    }

    String str() const {
        return '*' + rdt2uuid(rdt()).str() + '#' + id().str();
    }
};

}  // namespace ron

#endif  // CPP_KEY_HPP
