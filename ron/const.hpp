#ifndef RON_CONST_HPP
#define RON_CONST_HPP
#include <cassert>
#include <cstdint>
#include "slice.hpp"

namespace ron {

enum RON : uint8_t {
    OPEN = 0,
    CLOSED = 1,
    TEXT = 2,
    BINARY = 4,
    JSONi = 6,
    CBOR = 8,
    NOMINAL = 10,
    TEXT_OPEN = TEXT | OPEN,
    TEXT_CLOSED = TEXT | CLOSED,
    NOMINAL_OPEN = NOMINAL | OPEN,
    // and so on
};

constexpr Char SPEC_PUNCT[] = "*#@:";
enum SPEC : uint8_t { TYPE = 0, OBJECT = 1, EVENT = 2, REF = 3 };

enum VARIANT { RON_UUID = 0, RON_ATOM = 1, RFC4122_UUID = 2, RESERVED = 3 };

constexpr Char UUID_PUNCT[] = "$%+-";
enum UUID : uint8_t { NAME = 0, HASH = 1, TIME = 2, DERIVED = 3 };

constexpr Char ATOM_PUNCT[] = ">='^";
enum ATOM : uint8_t { UUID = 0, BUF = 0, INT = 1, STRING = 2, FLOAT = 3 };

enum FLAGS : uint8_t {
    INT_ATOM = (RON_ATOM << 2U) | ATOM::INT,
    STRING_ATOM = (RON_ATOM << 2U) | ATOM::STRING,
    FLOAT_ATOM = (RON_ATOM << 2U) | ATOM::FLOAT,
    NAME_UUID = (RON_UUID << 2U) | NAME,
    HASH_UUID = (RON_UUID << 2U) | HASH,
    TIME_UUID = (RON_UUID << 2U) | TIME,
    DERIVED_UUID = (RON_UUID << 2U) | DERIVED
};

constexpr Char HEX_PUNCT[] = "0123456789abcdef";

constexpr Char TERM_PUNCT[] = ";,!?";
enum TERM : uint8_t { RAW = 0, REDUCED = 1, HEADER = 2, QUERY = 3 };

inline TERM chr2term(Char c) {
    switch (c) {
        case TERM_PUNCT[RAW]:
            return RAW;
        case TERM_PUNCT[REDUCED]:
            return REDUCED;
        case TERM_PUNCT[HEADER]:
            return HEADER;
        case TERM_PUNCT[QUERY]:
            return QUERY;
        default:
            assert(false);
            return QUERY;
    }
}

constexpr Char BASE_PUNCT[] =
    "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz~";

constexpr Char FRAME_PUNCT[] = ".";
enum FRAME : uint8_t { END = 0 };

void init();

extern const uint8_t ABC[128];
extern const uint64_t IS_BASE[2];
extern const int8_t ABC64[256];
extern const int8_t ABC16[256];

enum ACID : uint8_t {
    NONE = 0,  // single-writer, strictly "state + linear op log", like MySQL
    DISTRIBUTED = 1,  // multiple-writer, partial-order
    IDEMPOTENT = 2,   // survives data duplication
    ID = IDEMPOTENT | DISTRIBUTED,
    COMMUTATIVE =
        4,  // arbitrary order (causality violations don't break convergence)
    CD = COMMUTATIVE | DISTRIBUTED,
    CID = CD | IDEMPOTENT,
    ASSOCIATIVE = 8,  // can form patches
    AD = ASSOCIATIVE | DISTRIBUTED,
    AI = IDEMPOTENT | ASSOCIATIVE,
    AID = ASSOCIATIVE | ID,
    ACD = COMMUTATIVE | AD,
    FULL = ASSOCIATIVE | CID
};

constexpr Char TAB{'\t'};
constexpr Char NL{'\n'};
constexpr Char SPACE{' '};
constexpr Char DOT{'.'};

const String FRAME_TERM{{DOT, NL}};

}  // namespace ron

#endif
