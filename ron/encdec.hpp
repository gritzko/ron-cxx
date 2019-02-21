#ifndef RON_ENCDEC_H
#define RON_ENCDEC_H
#include <cstdint>
#include <string>

namespace ron {

template <uint8_t bit_width, const char* coding>
void encode(std::string& coded, const uint8_t* raw, int bit_size) {
    uint32_t bits = 0;
    uint32_t bc = 0;
    static constexpr uint32_t MASK = (1U << bit_width) - 1U;
    while (bit_size>=8) {
        bits <<= 8;
        bc += 8;
        bits |= *raw;
        ++raw;
        bit_size-=8;
        while (bc >= bit_width) {
            bc -= bit_width;
            coded.push_back(coding[(bits >> bc) & MASK]);
        }
    }
    if (bc>0) {
        bits <<= bit_width - bc;
        coded.push_back(coding[bits & MASK]);
    }
}

template <int bit_width, const int8_t table[256]>
bool decode(std::string& raw, const char* coded, uint32_t bit_size) {
    uint32_t bits = 0;
    uint32_t bc = 0;
    while (bit_size>=bit_width) {
        bits <<= bit_width;
        bc += bit_width;
        int8_t value = table[*coded];
        if (value < 0) return false;
        ++coded;
        bit_size -= bit_width;
        bits |= value;
        while (bc >= 8) {
            bc -= 8;
            raw.push_back( uint8_t((bits >> bc) & 0xff) );
        }
    }
    if (bc>0) {
        raw.push_back( uint8_t((bits << (8 - bc)) & 0xff) );
    }
    return true;
}

}  // namespace ron
#endif
