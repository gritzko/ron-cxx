#ifndef RON_CONST_HPP
#define RON_CONST_HPP
#include <cstdint>

namespace ron {

enum RON : uint8_t {
	OPEN = 0,
	CLOSED = 1,
	TEXT = 2,
	BINARY = 4,
	JSON = 6,
	CBOR = 8,
	NOMINAL = 10,
	TEXT_OPEN = TEXT|OPEN,
	TEXT_CLOSED = TEXT|CLOSED,
	NOMINAL_OPEN = NOMINAL|OPEN,
	// and so on
};


static const char* SPEC_PUNCT = "*#@:";
enum SPEC : uint8_t {
	TYPE = 0,
	OBJECT = 1,
	EVENT = 2,
	REF = 3
};

enum VARIANT {
    RON_UUID = 0,
    RON_ATOM = 1,
    RFC4122_UUID = 2,
    RESERVED = 3
};

static const char* UUID_PUNCT = "$%+-";
enum UUID : uint8_t {
	NAME = 0,
	HASH = 1,
	TIME = 2,
	DERIVED = 3
};

static const char* ATOM_PUNCT = ">='^";
enum ATOM : uint8_t {
	UUID = 0,
	BUF = 0,
	INT = 1,
	STRING = 2,
	FLOAT = 3
};

enum FLAGS : uint8_t {
	INT_ATOM = (RON_ATOM<<2) | ATOM::INT,
	STRING_ATOM = (RON_ATOM<<2) | ATOM::STRING,
	FLOAT_ATOM = (RON_ATOM<<2) | ATOM::FLOAT,
	NAME_UUID = (RON_UUID<<2) | NAME,
	HASH_UUID = (RON_UUID<<2) | HASH,
	TIME_UUID = (RON_UUID<<2) | TIME,
	DERIVED_UUID = (RON_UUID<<2) | DERIVED
};

static const char* HEX_PUNCT = "0123456789abcdef";

static const char* TERM_PUNCT = ";,!?";
enum TERM : uint8_t {
	RAW = 0,
	REDUCED = 1,
	HEADER = 2,
	QUERY = 3
};

static const char* REDEF_PUNCT = "`";
enum REDEF : uint8_t {
	PREV = 0
};

static const char* PREFIX_PUNCT = "([{}])";
enum PREFIX : uint8_t {
	PRE4 = 0,
	PRE5 = 1,
	PRE6 = 2,
	PRE7 = 3,
	PRE8 = 4,
	PRE9 = 5
};

static const char* BASE_PUNCT = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz~";
enum BASE : uint8_t {
	DIGIT0 = 0,
	DIGIT1 = 1,
	DIGIT2 = 2,
	DIGIT3 = 3,
	DIGIT4 = 4,
	DIGIT5 = 5,
	DIGIT6 = 6,
	DIGIT7 = 7,
	DIGIT8 = 8,
	DIGIT9 = 9,
	DIGIT10 = 10,
	DIGIT11 = 11,
	DIGIT12 = 12,
	DIGIT13 = 13,
	DIGIT14 = 14,
	DIGIT15 = 15,
	DIGIT16 = 16,
	DIGIT17 = 17,
	DIGIT18 = 18,
	DIGIT19 = 19,
	DIGIT20 = 20,
	DIGIT21 = 21,
	DIGIT22 = 22,
	DIGIT23 = 23,
	DIGIT24 = 24,
	DIGIT25 = 25,
	DIGIT26 = 26,
	DIGIT27 = 27,
	DIGIT28 = 28,
	DIGIT29 = 29,
	DIGIT30 = 30,
	DIGIT31 = 31,
	DIGIT32 = 32,
	DIGIT33 = 33,
	DIGIT34 = 34,
	DIGIT35 = 35,
	DIGIT36 = 36,
	DIGIT37 = 37,
	DIGIT38 = 38,
	DIGIT39 = 39,
	DIGIT40 = 40,
	DIGIT41 = 41,
	DIGIT42 = 42,
	DIGIT43 = 43,
	DIGIT44 = 44,
	DIGIT45 = 45,
	DIGIT46 = 46,
	DIGIT47 = 47,
	DIGIT48 = 48,
	DIGIT49 = 49,
	DIGIT50 = 50,
	DIGIT51 = 51,
	DIGIT52 = 52,
	DIGIT53 = 53,
	DIGIT54 = 54,
	DIGIT55 = 55,
	DIGIT56 = 56,
	DIGIT57 = 57,
	DIGIT58 = 58,
	DIGIT59 = 59,
	DIGIT60 = 60,
	DIGIT61 = 61,
	DIGIT62 = 62,
	DIGIT63 = 63
};

static const char* FRAME_PUNCT = ".";
enum FRAME : uint8_t {
	END = 0
};

  
void init();

extern uint8_t ABC[128];
extern uint64_t IS_BASE[2];

enum ACID : uint8_t {
   NONE = 0, // single-writer, strictly "state + linear op log", like MySQL
   DISTRIBUTED    = 1,    // multiple-writer, partial-order
   IDEMPOTENT    = 2,    // survives data duplication
   ID   = IDEMPOTENT | DISTRIBUTED,
   COMMUTATIVE    = 4, // arbitrary order (causality violations don't break convergence)
   CD   = COMMUTATIVE | DISTRIBUTED,
   CID  = CD | IDEMPOTENT,
   ASSOCIATIVE    = 8, // can form patches
   AD   = ASSOCIATIVE | DISTRIBUTED,
   AI   = IDEMPOTENT | ASSOCIATIVE,
   AID  = ASSOCIATIVE | ID,
   ACD  = COMMUTATIVE | AD,
   FULL = ASSOCIATIVE | CID
};


}

#endif
