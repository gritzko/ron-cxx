#ifndef RON_ENCDEC_H
#define RON_ENCDEC_H
#include <stdint.h>

namespace ron {

    template<int bit_width, const char* coding>
    void encode(const uint8_t* bytes, int length, char* coded) {
        uint32_t bits = 0;
        int bc = 0;
        static const int MASK = (1<<bit_width)-1;
        for (int i=0; i<length; i++) {
            bits <<= 8;
            bc += 8;
            bits |= bytes[i];
            while (bc>=bit_width) {
                bc -= bit_width;
                *coded = coding[(bits>>bc)&MASK];
                coded++;
            }
        }
        if (bc) {
            bits <<= bit_width-bc;
            *coded = coding[ bits&MASK ];
        }
    }

    template<int bit_width, const int8_t table[256]>
    bool decode(const char* coded, int length, uint8_t* bytes) {
        uint32_t bits = 0;
        int bc = 0;
        for (int i=0; i<length; i++) {
            bits <<= bit_width;
            bc += bit_width;
            int8_t value = table[coded[i]];
            if (value<0) return false;
            bits |= value;
            while (bc>=8) {
                bc -= 8;
                *bytes = uint8_t( (bits>>bc) & 0xff );
                bytes++;
            }
        }
        return true;
    }

}
#endif