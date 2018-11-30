#ifndef RON_MERKLE_HPP
#define RON_MERKLE_HPP
#include <botan/sha2_64.h>
#include "uuid.hpp"
#include "status.hpp"
#include "op.hpp"


namespace ron {

    typedef uint8_t SHA3_256[32];

    struct sha3stream {
        Botan::SHA_512_256 hash;
        sha3stream() {
        }
        inline sha3stream& operator << (uint64_t word) {
            hash.update((uint8_t*)&word, sizeof(word));
            return *this;
        }
        inline sha3stream& operator << (slice_t data) {
            hash.update((uint8_t*)data.buf_, data.size_);
            return *this;
        }
        inline sha3stream& operator << (const SHA3_256& data) {
            hash.update((uint8_t*)&data, sizeof(data));
            return *this;
        }
        inline void close (SHA3_256& result) {
            hash.final((uint8_t*)&result);
        }
    };


    // TODO move to const.hpp
    const uint64_t INT_ATOM_FLAGS = uint64_t(ATOM::INT | (VARIANT::RON_ATOM<<2))<<60;
    const uint64_t FLOAT_ATOM_FLAGS = uint64_t(ATOM::FLOAT | (VARIANT::RON_ATOM<<2))<<60;
    const uint64_t STRING_ATOM_FLAGS = uint64_t(ATOM::STRING | (VARIANT::RON_ATOM<<2))<<60;

    /* TODO HASHING MEGACOMMIT
     * [x] lww - roots - method
     * [ ] write to a file, check (test)
     * [ ] compare rhash --sha3-256
     * [ ] string conversions (base64)
     * [ ] utf-8 parsing, tests
     * */
    template <typename Frame, typename Stream>
    void hash_feed(
            const typename Frame::Cursor& cursor,
            Stream& stream,
            const SHA3_256& prev_hash,
            const SHA3_256& ref_hash) {
        const Op& op = cursor.op();
        stream
            << op.id().value().be()
            << op.id().origin().be()
            << prev_hash
            << op.ref().value().be()
            << op.ref().origin().be()
            << ref_hash;
        for(fsize_t i=2; i<op.size(); i++) {
            const Atom& atom = op.atom(i);
            switch (atom.type()) {
                case UUID:
                    stream << atom.value().be();
                    stream << atom.origin().be();
                    break;
                case INT:
                    stream << atom.value().be();
                    stream << htobe64(INT_ATOM_FLAGS);
                    break;
                case FLOAT:
                    stream << atom.value().be();
                    stream << htobe64(FLOAT_ATOM_FLAGS);
                    break;
                case STRING:
                    stream << atom.value().be();
                    stream << htobe64(STRING_ATOM_FLAGS);
                    // TODO: slice_t, no alloc
                    stream << cursor.string(i);
                    break;
            }
        }
    }

    void hash_uuid(const Uuid& uuid, SHA3_256& hash) {
        sha3stream stream;
        stream << uuid.value().be() << uuid.origin().be();
        stream.close(hash);
    }

};

#endif