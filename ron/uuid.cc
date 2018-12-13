#include "uuid.hpp"
#include <cinttypes>

namespace ron {

constexpr uint8_t Word::OFFSET6[10];
constexpr uint64_t Word::LOWER6[11];
const Word Word::NEVER{Word::MAX_VALUE};
const Uuid Uuid::ZERO{};
const Uuid Uuid::FATAL{Word::MAX_VALUE, Word::MAX_VALUE};

size_t Word::write_base64(char* to) const {
    uint64_t val = _64;
    size_t len = 0;
    do {
        to[len] = BASE_PUNCT[0x3fU & (val >> OFFSET6[len])];
        len++;
    } while (val & LOWER6[len]);
    return len;
}

bool Word::is_all_digits() const {
    char letters[BASE64_WORD_LEN];
    size_t len = write_base64(letters);
    for (int i = 0; i < len; i++)
        if (!std::isdigit(letters[i])) return false;
    return true;
}

std::string Uuid::str() const {
    char ret[Word::BASE64_WORD_LEN * 2 + 2 + 1];
    size_t len = 0;
    int vrt = variety();
    if (vrt) {
        ret[len++] = BASE_PUNCT[vrt];
        ret[len++] = '/';
    }
    len += words_.first.write_base64(ret + len);
    int schm = version();
    if (schm != 0 || !origin().is_zero()) {
        ret[len++] = UUID_PUNCT[schm];
        len += words_.second.write_base64(ret + len);
    }
    return std::string(ret, len);
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

const int8_t ABC16[256] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,0,1,2,3,4,5,6,7,8,9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,10,11,12,13,14,15,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};

}  // namespace ron
