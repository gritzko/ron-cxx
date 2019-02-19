#ifndef RON_ENCDEC_H
#define RON_ENCDEC_H
#include <cstdint>

namespace ron {

template <uint8_t bit_width, const char* coding>
void encode(char* coded, const uint8_t* bytes, int bit_size) {
    uint32_t bits = 0;
    uint32_t bc = 0;
    static const uint32_t MASK = (1U << bit_width) - 1U;
    while (bit_size>0) {
        bits <<= 8;
        bc += 8;
        bits |= *bytes;
        ++bytes;
        bit_size-=8;
        while (bc >= bit_width) {
            bc -= bit_width;
            *coded = coding[(bits >> bc) & MASK];
            coded++;
        }
    }
    if (bc) {
        bits <<= bit_width - bc;
        *coded = coding[bits & MASK];
    }
}

template <int bit_width, const int8_t table[256]>
bool decode(uint8_t* bytes, const char* coded, uint32_t bit_size) {
    uint32_t bits = 0;
    uint32_t bc = 0;
    while (bit_size>0) {
        bits <<= bit_width;
        bc += bit_width;
        int8_t value = table[*coded];
        if (value < 0) return false;
        ++coded;
        bit_size -= bit_width;
        bits |= value;
        while (bc >= 8) {
            bc -= 8;
            *bytes = uint8_t((bits >> bc) & 0xff);
            bytes++;
        }
    }
    if (bc) {
        *bytes = uint8_t((bits << (8 - bc)) & 0xff);
    }
    return true;
}

}  // namespace ron
#endif
