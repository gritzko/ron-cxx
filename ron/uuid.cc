#include "uuid.hpp"
#include <cinttypes>

namespace ron {

constexpr uint8_t Word::OFFSET6[10];
constexpr uint64_t Word::LOWER6[11];
const Word Word::NEVER{Word::MAX_VALUE};
const Uuid Uuid::NIL{0, 0};
const Uuid Uuid::FATAL{Word::MAX_VALUE, Word::MAX_VALUE};
const Uuid Uuid::NEVER = Uuid::Time(Word::MAX_VALUE, Word::MAX_VALUE);
const Uuid Uuid::COMMENT{"~"};

inline case_t char_case(uint64_t v) {
    constexpr uint64_t CAPS_MASK = ((1UL << 36) - (1UL << 10)) | (1UL << 63);
    constexpr uint64_t SNAKE_MASK = ((1UL << 63) - (1UL << 36));
    int idx = v & 63;
    return case_t((((CAPS_MASK >> idx) & 1) << 1) | ((SNAKE_MASK >> idx) & 1));
}

case_t Word::base64_case() const {
    case_t ret = NUMERIC;
    uint64_t u = _64;
    for (int i = 0; i < 10 && u != 3; ++i, u >>= 6) {
        ret = case_t(ret | char_case(u));
    }
    return ret;
}

void Word::write_base64(String& to) const {
    size_t len = 0;
    do {
        to.push_back(BASE_PUNCT[0x3fU & (_64 >> OFFSET6[len])]);
        len++;
    } while (_64 & LOWER6[len]);
}

bool Word::is_all_digits() const {
    size_t len = 0;
    do {
        Char next = BASE_PUNCT[0x3fU & (_64 >> OFFSET6[len])];
        if (!isdigit(next)) return false;
        len++;
    } while (_64 & LOWER6[len]);
    return true;
}

void Uuid::write_base64(ron::String& ret) const {
    size_t len = 0;
    int vrt = variety();
    if (vrt) {
        ret.push_back(BASE_PUNCT[vrt]);
        ret.push_back('/');
    }
    words_.first.write_base64(ret);
    int schm = version();
    if (schm != 0 || !origin().is_zero()) {
        ret.push_back(UUID_PUNCT[schm]);
        words_.second.write_base64(ret);
    }
}

String Uuid::str() const {
    ron::String ret;
    ret.reserve(Word::BASE64_WORD_LEN * 2 + 2 + 1);
    write_base64(ret);
    return ret;
}

Word Uuid::HybridTime(time_t seconds, long int nanos) {
    tm* t = gmtime(&seconds);
    uint64_t ret = 1900U + t->tm_year - 2010U;
    ret *= 12;
    ret += t->tm_mon;
    ret <<= 6;
    ret |= t->tm_mday - 1;
    ret <<= 6;
    ret |= t->tm_hour;
    ret <<= 6;
    ret |= t->tm_min;
    ret <<= 6;
    ret |= t->tm_sec;
    ret <<= 24;
    ret |= nanos / 100;
    return ret;
}

#include <sys/time.h>

Word Uuid::Now() {
    timeval tv;
    gettimeofday(&tv, nullptr);
    return HybridTime(tv.tv_sec, long(tv.tv_usec) * 1000);
}

const uint8_t ABC[128] = {
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 2,   255, 1,   0,   1,   255, 2,   0,   5,   0,   2,   1,
    3,   0,   255, 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,   3,   0,
    255, 1,   0,   3,   2,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
    20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,
    35,  1,   255, 4,   3,   36,  0,   37,  38,  39,  40,  41,  42,  43,  44,
    45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
    60,  61,  62,  2,   255, 3,   63,  255};
const uint64_t IS_BASE[2] = {0x03ff000000000000, 0x47fffffe87fffffe};

const int8_t ABC64[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, -1, -1, -1, -1,
    36, -1, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53,
    54, 55, 56, 57, 58, 59, 60, 61, 62, -1, -1, -1, 63, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1};

const int8_t ABC16[256] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,
    9,  -1, -1, -1, -1, -1, -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1};

}  // namespace ron
