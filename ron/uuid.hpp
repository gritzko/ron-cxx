#ifndef ron_uuid_hpp
#define ron_uuid_hpp
#include <cstdint>
#include <string>
#include <utility>
#include <stdint-gcc.h>
#include "const.hpp"
#include <iostream>


namespace ron {

typedef uint32_t fsize_t;
typedef std::pair<fsize_t, fsize_t> frange_t;

union Word {

    uint64_t _64[1];
    uint32_t _32[2];
    uint16_t _16[4];
    uint8_t _8[8];

    Word (uint64_t value=0) : _64{value} {}
    Word (const std::string& word);

    explicit operator unsigned long () const { return _64[0]; }

    // payload bit size
    static constexpr int PBS = 60;
    // flag bit size
    static constexpr int FBS = 64-PBS;
    static constexpr uint64_t MAX_VALUE = (1L<<PBS)-1;
    static constexpr int8_t OFFSET6[10] = {
        PBS-(6*1),
        PBS-(6*2),
        PBS-(6*3),
        PBS-(6*4),
        PBS-(6*5),
        PBS-(6*6),
        PBS-(6*7),
        PBS-(6*8),
        PBS-(6*9),
        PBS-(6*10)
    };
    static constexpr uint64_t PAYLOAD_BITS = (1L<<PBS)-1;
    static constexpr uint64_t FLAG_BITS = UINT64_MAX - PAYLOAD_BITS;
    static constexpr uint64_t LOWER6[11] = {
        (1L<<(PBS-0*6))-1,
        (1L<<(PBS-1*6))-1,
        (1L<<(PBS-2*6))-1,
        (1L<<(PBS-3*6))-1,
        (1L<<(PBS-4*6))-1,
        (1L<<(PBS-5*6))-1,
        (1L<<(PBS-6*6))-1,
        (1L<<(PBS-7*6))-1,
        (1L<<(PBS-8*6))-1,
        (1L<<(PBS-9*6))-1,
        0
    };

    inline void put6 (int pos, uint8_t value) {
        _64[0] |= uint64_t(value) << (pos*6);
    }
    inline void set8 (int pos, uint8_t value) {
        _8[0x7-pos] = value;
    }
    inline void set16 (int pos, uint16_t value) {
        _16[0x3-pos] = value;
    }
    inline void set32 (int pos, uint32_t value) {
        _32[0x1-pos] = value;
    }
    inline void set64 (uint64_t value) {
        _64[0] = value;
    }
    inline void set_flags (uint8_t value) {
        _64[0] |= uint64_t(value) << PBS;
    }
    inline uint8_t get8 (int pos) const {
        return _8[0x7-pos];
    }
    inline uint8_t get16 (int pos) const {
        return _16[0x3-pos];
    }
    inline uint8_t get32 (int pos) const {
        return _32[0x1-pos];
    }
    inline fsize_t get30 (int pos) const {
        return (_64[0]>>(pos?30:0)) & ((1<<30)-1);
    }
    inline uint8_t get64 () const {
        return _64[0];
    }
    inline uint8_t flags () const {
        return get8(0)>>4;
    }
    inline uint8_t get6(int pos) const {
        return 0x3f & (_64[0]>>(pos*6));
    }
    inline void zero() { _64[0]=0; }
    inline void zero_flags() { _64[0]&=PAYLOAD_BITS; }
    inline void zero_payload() {
        _64[0]&=FLAG_BITS;
    }
    int writeBase64(std::string& str) const;
    inline bool is_zero() const { return _64[0]==0; }
    inline Word inc() const { return Word{_64[0]+1}; }
    inline bool operator < (const Word& b) const {
        return _64[0] < b._64[0];
    }
    inline bool operator > (const Word& b) const {
        return _64[0] > b._64[0];
    }
    inline bool operator == (const Word& b) const {
        return _64[0] == b._64[0];
    }
    inline bool operator != (const Word& b) const {
        return _64[0] != b._64[0];
    }
};


struct Cell {
    std::pair<Word,Word> words_;
    Cell () : words_{0,0} {}
    Cell (Word value, Word origin) : words_{value, origin} {}
    inline Word value() const { return words_.first; }
    inline Word origin() const { return words_.second; }
    inline Word& word(int i) {
        return i ? words_.second : words_.first;
    }
};


struct Uuid : public Cell {
    Uuid () : Cell{0,0} {}
    Uuid (Word value, Word origin) : Cell{value,origin} {}
    Uuid (const std::string& buf);
    // value flag bits
    inline uint8_t vfb () const {
        return value().flags();
    }
    // origin flag bits
    inline uint8_t ofb () const {
        return origin().flags();
    }
    inline uint8_t variant () const {
        return ofb()>>2;
    }
    inline uint8_t version () const {
        // TODO rfc4122
        return ofb() & 3;
    }
    inline uint8_t variety () const {
        return vfb();
    }
    inline Word& word(int a, int i) { return Cell::word(i); }
    std::string str() const;
    static Uuid Time(Word value, Word origin) {
        return Uuid{value, (uint64_t(origin)&Word::PAYLOAD_BITS) |
                (uint64_t(UUID::TIME)<<Word::PBS)};
    }
    static Uuid Derived(Word value, Word origin) {
        return Uuid{value, (uint64_t(origin)&Word::PAYLOAD_BITS) |
                           (uint64_t(UUID::DERIVED)<<Word::PBS)};
    }
    inline Uuid derived() const {
        return Derived(value(), origin());
    }
    inline Uuid inc () const {
        return Uuid{value().inc(), origin()};
    }
    inline bool operator < (const Uuid& b) const {
        return words_ < b.words_;
    }
    inline bool operator > (const Uuid& b) const {
        return words_ > b.words_;
    }
    inline bool operator == (const Uuid& b) const {
        return words_ == b.words_;
    }
    inline bool operator != (const Uuid& b) const {
        return words_ != b.words_;
    }
};


struct Atom : public Cell {
    frange_t range() const;
    int64_t int_value() const {
        return (int64_t) (unsigned long) value();
    }
    Uuid uuid_value() const;
    double float_value() const {
        return (double) (unsigned long) value();
    }
    ATOM type() const {
        return ATOM(origin().flags()&3);
    }
    std::string str(const char* buf=0) const;
};

std::string unescape (const char* buf, fsize_t size);

std::string escape (const char* buf, fsize_t size);

inline std::string escape (const std::string& str) {
    return unescape(str.data(), str.size());
}


} // namespace ron


#endif
