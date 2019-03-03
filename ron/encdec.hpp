#ifndef RON_ENCDEC_H
#define RON_ENCDEC_H
#include <cstdint>
#include <string>
#include "slice.hpp"

namespace ron {

template <uint8_t bit_width, const char* coding>
void encode(std::string& coded, const uint8_t* raw, int bit_size) {
    uint32_t bits = 0;
    uint32_t bc = 0;
    static constexpr uint32_t MASK = (1U << bit_width) - 1U;
    while (bit_size >= 8) {
        bits <<= 8;
        bc += 8;
        bits |= *raw;
        ++raw;
        bit_size -= 8;
        while (bc >= bit_width) {
            bc -= bit_width;
            coded.push_back(coding[(bits >> bc) & MASK]);
        }
    }
    if (bc > 0) {
        bits <<= bit_width - bc;
        coded.push_back(coding[bits & MASK]);
    }
}

template <int bit_width, const int8_t table[256]>
bool decode(std::string& raw, const char* coded, uint32_t bit_size) {
    uint32_t bits = 0;
    uint32_t bc = 0;
    while (bit_size >= bit_width) {
        bits <<= bit_width;
        bc += bit_width;
        int8_t value = table[*coded];
        if (value < 0) return false;
        ++coded;
        bit_size -= bit_width;
        bits |= value;
        while (bc >= 8) {
            bc -= 8;
            raw.push_back(uint8_t((bits >> bc) & 0xff));
        }
    }
    if (bc > 0) {
        raw.push_back(uint8_t((bits << (8 - bc)) & 0xff));
    }
    return true;
}

/**
                                UTF-8 / UTF-16

One may wonder, why do we take the labor of implementing UTF-8/UTF-16
conversions? First of all, the C++ UTF support is a story of pain and misery
   https://stackoverflow.com/questions/42946335/deprecated-header-codecvt-replacement
Second, RON has the requirement of bitwise identical results, so this kind of
 control is a plus. In particular, RON has every reason to aggressively enforce
 canonized UTF-8 encoding (e.g. prevent two-byte codepoints from taking three
bytes). Third, RON(text) has to decode escapes as well as UTF. Just to prevent
double pass and double allocation, the custom code is preferred. UTF8<->UTF16
recoding is known to be a major performance sink. Again, RON's raison d'être is
replica convergence. We intend to transfer, merge and recode data while the
hashes must stays the same. There is no other choice than to have a very formal
model and to exercise bit-level control. For the perspective on the multitude of
potential issues, please read: http://seriot.ch/parsing_json.php Given all of
the above, paranoid control is a must! Finally, UTF-8/UTF-16 *encoding* is not
that much of code. We don't care how letters look typographically; we only care
about the bits.
*/

inline void utf8append(std::string& to, Codepoint cp) {
    if (cp < 128) {
        to.push_back(static_cast<Char>(cp));
    } else if (cp < 2048) {
        to.push_back((cp >> 6) | (128 + 64));
        to.push_back((cp & 63) | 128);
    } else if (cp < 65536) {
        to.push_back((cp >> 12) | (128 + 64 + 32));
        to.push_back(((cp >> 6) & 63) | 128);
        to.push_back((cp & 63) | 128);
    } else {
        to.push_back(((cp >> 18) & 7) | (128 + 64 + 32 + 16));
        to.push_back(((cp >> 12) & 63) | 128);
        to.push_back(((cp >> 6) & 63) | 128);
        to.push_back((cp & 63) | 128);
    }
}

inline void utf16append(std::u16string& to, Codepoint cp) {
    if (cp < 0xd7ff || (cp < 0x10000 && cp >= 0xe000)) {
        to.push_back(static_cast<char16_t>(cp));
    } else {
        to.push_back(static_cast<char16_t>(0xd800 + (cp >> 10)));
        to.push_back(static_cast<char16_t>(0xdc00 + (cp & 1023)));
    }
}

inline Codepoint utf8read(std::string::const_iterator& at,
                          std::string::const_iterator end) {  // TODO known-size
    uint8_t head = *at;
    ++at;
    if (head < 128) {  // latin fast path
        return head;
    }
    Codepoint ret{0};
    int sz;
    if (head <= 0xdf) {
        sz = 2;
        ret = head & 0x1f;
    } else if (head <= 0xef) {
        sz = 3;
        ret = head & 0xf;
    } else if (head <= 0xf7) {
        sz = 4;
        ret = head & 0x7;
    } else {
        return CP_ERROR;
    }
    if (end - at < sz - 1) {  // has bytes?
        while (at < end) {
            ++at;
        }
        return CP_ERROR;
    }
    switch (sz) {
        case 4:
            ret <<= 6;
            ret |= *at & 0x3f;
            ++at;
        case 3:
            ret <<= 6;
            ret |= *at & 0x3f;
            ++at;
        case 2:
            ret <<= 6;
            ret |= *at & 0x3f;
            ++at;
    }
    return ret;
}

inline Codepoint utf16read(std::u16string::const_iterator& at,
                           std::u16string::const_iterator end,
                           bool check = true) {
    Codepoint ret = *at;
    ++at;
    if (ret < 0xd800) {
        return ret;
    }
    if (ret >= 0xe000) {
        return ret;
    }
    ret -= 0xd800;
    uint16_t next = *at;
    ret <<= 10;
    ret |= next - 0xdc00;
    ++at;
    return ret;
}

void utf8utf16(std::u16string& to, const std::string& from);

}  // namespace ron
#endif
