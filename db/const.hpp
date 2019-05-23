#ifndef RON_DB_CONST_HPP
#define RON_DB_CONST_HPP

#include "../ron/uuid.hpp"
#include "key.hpp"

namespace ron {

enum MAP : uint8_t {
    CHAIN_MAP,
    META_MAP,
    OP_MAP,
    YARN_MAP,
    SHA2_MAP,
    PREV_MAP,
    HEAD_MAP,
    OBJ_MAP,
    CSV_MAP,
    TXT_MAP,
    JSON_MAP,
    MAP_COUNT
};

static const uint64_t CHAIN_MAP_NAME{715112314629521408UL};
static const Uuid CHAIN_MAP_ID{CHAIN_MAP_NAME, 0};
static const uint64_t META_MAP_NAME{894494834235015168UL};
static const Uuid META_MAP_ID{META_MAP_NAME, 0};
static const uint64_t OP_MAP_NAME{933371022772535296UL};
static const Uuid OP_MAP_ID{OP_MAP_NAME, 0};
static const uint64_t YARN_MAP_NAME{1109533813702131712UL};
static const Uuid YARN_MAP_ID{YARN_MAP_NAME, 0};
static const uint64_t SHA2_MAP_NAME{1003339682156642304UL};
static const Uuid SHA2_MAP_ID{SHA2_MAP_NAME, 0};
static const uint64_t PREV_MAP_NAME{952132676872044544UL};
static const Uuid PREV_MAP_ID{PREV_MAP_NAME, 0};
static const uint64_t HEAD_MAP_NAME{804339484962324480UL};
static const Uuid HEAD_MAP_ID{HEAD_MAP_NAME, 0};
static const uint64_t OBJ_MAP_NAME{929632683238096896UL};
static const Uuid OBJ_MAP_ID{OBJ_MAP_NAME, 0};
static const uint64_t CSV_MAP_NAME{718297752286527488UL};
static const Uuid CSV_MAP_ID{CSV_MAP_NAME, 0};
static const uint64_t TXT_MAP_NAME{1025941105738252288UL};
static const Uuid TXT_MAP_ID{TXT_MAP_NAME, 0};
static const uint64_t JSON_MAP_NAME{844371191501160448UL};
static const Uuid JSON_MAP_ID{JSON_MAP_NAME, 0};

static const Uuid MAP_IDS[] = {CHAIN_MAP_ID, META_MAP_ID, OP_MAP_ID,
                               YARN_MAP_ID,  SHA2_MAP_ID, PREV_MAP_ID,
                               HEAD_MAP_ID,  OBJ_MAP_ID,  CSV_MAP_ID,
                               TXT_MAP_ID,   JSON_MAP_ID, Uuid::FATAL};

inline Uuid map2uuid(MAP e) { return MAP_IDS[e]; }

inline MAP uuid2map(const Uuid& u) {
    switch (uint8_t(u.value.as_u64 >> 52)) {
        case uint8_t(CHAIN_MAP_NAME >> 52):
            return u == CHAIN_MAP_ID ? CHAIN_MAP : MAP_COUNT;
        case uint8_t(META_MAP_NAME >> 52):
            return u == META_MAP_ID ? META_MAP : MAP_COUNT;
        case uint8_t(OP_MAP_NAME >> 52):
            return u == OP_MAP_ID ? OP_MAP : MAP_COUNT;
        case uint8_t(YARN_MAP_NAME >> 52):
            return u == YARN_MAP_ID ? YARN_MAP : MAP_COUNT;
        case uint8_t(SHA2_MAP_NAME >> 52):
            return u == SHA2_MAP_ID ? SHA2_MAP : MAP_COUNT;
        case uint8_t(PREV_MAP_NAME >> 52):
            return u == PREV_MAP_ID ? PREV_MAP : MAP_COUNT;
        case uint8_t(HEAD_MAP_NAME >> 52):
            return u == HEAD_MAP_ID ? HEAD_MAP : MAP_COUNT;
        case uint8_t(OBJ_MAP_NAME >> 52):
            return u == OBJ_MAP_ID ? OBJ_MAP : MAP_COUNT;
        case uint8_t(CSV_MAP_NAME >> 52):
            return u == CSV_MAP_ID ? CSV_MAP : MAP_COUNT;
        case uint8_t(TXT_MAP_NAME >> 52):
            return u == TXT_MAP_ID ? TXT_MAP : MAP_COUNT;
        case uint8_t(JSON_MAP_NAME >> 52):
            return u == JSON_MAP_ID ? JSON_MAP : MAP_COUNT;
        default:
            return MAP_COUNT;
    }
}

}  // namespace ron

#endif
