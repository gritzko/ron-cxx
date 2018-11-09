#ifndef ron_uuid_hpp
#define ron_uuid_hpp
#include <cstdint>
#include <string>
#include "const.hpp"
#include <iostream>
#include "slice.hpp"

namespace ron {


union Word {

    uint64_t _64;
    uint32_t _32[2];
    uint16_t _16[4];
    uint8_t _8[8];

    Word (uint64_t value=0) : _64{value} {}
    Word (uint8_t flags, slice_t data) { 
        _64 = flags & 0xf;
        int i=0;
        while(i<data.size()) {
            _64 <<= Word::BASE64_BITS;
            _64 |= ABC[data[i]]; // TODO inapprop chars
            i++;
        }
        while (i<Word::MAX_BASE64_SIZE) {
            _64 <<= Word::BASE64_BITS;
            i++;
        }
    }
    Word (const char* word, fsize_t size);
    Word (const std::string& word) : Word{word.data(), (fsize_t)word.size()} {}
    explicit Word (const char* word) : Word{word, (fsize_t)strlen(word)} {}
    Word (ATOM atype, fsize_t offset, fsize_t length) {
        _64 = atype;
        _64 <<= 30;
        _64 |= offset;
        _64 <<= 30;
        _64 |= length;
    }
    Word (ATOM atype, frange_t range) : 
        _64{(uint64_t(atype)<<60)|(uint64_t(range.first)<<30)|range.second} {}

    explicit operator uint64_t () const { return _64; }

    // payload bit size
    static constexpr int PBS = 60;
    static constexpr int BASE64_BITS = 6;
    // max base64 char size
    static constexpr int MAX_BASE64_SIZE = PBS/BASE64_BITS;
    //static const Word ZERO;
    // flag bit size
    static constexpr int FBS = 64-PBS;
    static constexpr uint64_t MAX_VALUE = (1L<<PBS)-1;
    static constexpr uint64_t MAX_VALUE_30 = (1L<<30)-1;
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
        _64 |= uint64_t(value) << (pos*6);
    }
    inline void put2 (int pos, uint8_t value) {
        _64 |= uint64_t(value&3) << (pos<<1);
    }
    inline void put4 (int pos, uint8_t value) {
        _64 |= uint64_t(value&0xf) << (pos<<2);
    }
    inline void put30 (int pos, uint32_t value) {
        _64 |= uint64_t(value) << (pos?30:0);
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
        _64 = value;
    }
    inline void set_flags (uint8_t value) {
        _64 |= uint64_t(value) << PBS;
    }
    inline uint8_t get8 (int pos) const {
        return _8[0x7-pos];
    }
    inline uint16_t get16 (int pos) const {
        return _16[0x3-pos];
    }
    inline uint32_t get32 (int pos) const {
        return _32[0x1-pos];
    }
    inline fsize_t get30 (int pos) const {
        return fsize_t(_64>>(pos?30:0)) & ((1<<30)-1);
    }
    inline uint64_t get64 () const {
        return _64;
    }
    inline uint8_t flags () const {
        return get8(0)>>4;
    }
    inline uint8_t get6(int pos) const {
        return uint8_t(0x3f & (_64>>(pos*6)));
    }
    inline void zero() { _64=0; }
    inline void zero_flags() { _64&=PAYLOAD_BITS; }
    inline void zero_payload() {
        _64&=FLAG_BITS;
    }
    int write_base64(std::string &str) const;
    inline uint64_t payload() const { return _64&MAX_VALUE; }
    inline bool is_zero() const { return _64==0; }
    inline Word inc() const { return Word{_64+1}; }
    inline bool operator < (const Word& b) const {
        return _64 < b._64;
    }
    inline bool operator > (const Word& b) const {
        return _64 > b._64;
    }
    inline bool operator == (const Word& b) const {
        return _64 == b._64;
    }
    inline bool operator != (const Word& b) const {
        return _64 != b._64;
    }
    inline size_t hash () const {
        static constexpr auto _64_hash_fn = std::hash<uint64_t>{};
        return _64_hash_fn(_64);
    }

};

enum half_t { VALUE=0, ORIGIN=1 } ;

struct Atom {
    std::pair<Word,Word> words_;
    Atom () : words_{0,0} {}
    Atom (Word value, Word origin) : words_{value, origin} {}
    inline Word value() const { return words_.first; }
    inline Word origin() const { return words_.second; }
    // value flag bits
    inline uint8_t vfb () const {
        return value().flags();
    }
    // origin flag bits
    inline uint8_t ofb () const {
        return origin().flags();
    }
    inline Word& word(int i) {
        return i ? words_.second : words_.first;
    }
    inline Word& operator[] (half_t half) {
        return half ? words_.second : words_.first;
    }
    inline const Word& operator[] (half_t half) const {
        return half ? words_.second : words_.first;
    }
    inline VARIANT variant () const {
        return VARIANT(ofb()>>2);
    }
    static Atom String (frange_t range) {
        return Atom{0, Word{ATOM::STRING, range}};
    }
    static Atom Float (frange_t range) {
        return Atom{0, Word{ATOM::FLOAT, range}};
    }
    static Atom Integer (frange_t range) {
        return Atom{0, Word{ATOM::INT, range}};
    }
    inline frange_t range() const {
        const uint64_t& w = words_.second._64;
        return frange_t{(w>>30)&Word::MAX_VALUE_30, w&Word::MAX_VALUE_30};
    }
};


struct Uuid : public Atom {
    Uuid () : Atom{0,0} {}
    Uuid (Word value, Word origin) : Atom{value,origin} {}
    Uuid (slice_t data);
    // pre-parsed Uuid, got hints and correctness guarantee
    Uuid (char variety, slice_t value, char version, slice_t origin) :
        Uuid{
            Word{ABC[variety], value},
            Word{ABC[version], origin}
        } {}
    Uuid (const std::string& buf) : Uuid{slice_t{buf}} {}
    Uuid (const char* buf) : 
        Uuid{slice_t{buf, static_cast<fsize_t>(strlen(buf))}} {}
    inline enum UUID version () const {
        return (enum UUID)(ofb() & 3);
    }
    inline uint8_t variety () const {
        return vfb();
    }
    inline Word& word(int a, int i) { return Atom::word(i); }
    std::string str() const;
    inline bool zero() const { return value()==0; }
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
    inline bool operator == (const char* b) const {
        return *this == Uuid{b};
    }
    inline bool operator != (const Uuid& b) const {
        return words_ != b.words_;
    }
    
    static const Uuid ZERO;
    static const Uuid FATAL;

    static Uuid Parse(char variety, slice_t value, char version, slice_t origin) {
        return Uuid{ Word{ABC[variety], value}, Word{ABC[version], origin} };
    }

};


struct Value : public Atom {
    inline static Word range_word(ATOM type, frange_t range) {
        uint64_t rw = (uint64_t(type)<<60) | (uint64_t(range.first)<<30) | range.second;
        Word w = Word{rw};
        return w;
    }
    Value (Word value, ATOM type, frange_t range) : Atom{value, range_word(type, range)} {
    }
    Value () : Atom{} {}
    frange_t range() const {
        return frange_t{origin().get30(1), origin().get30(0)};
    }
    int64_t int_value() const {
        return (int64_t) (unsigned long) value();
    }
    const Uuid& uuid_value() const {
        return (const Uuid&)(*this);
    }
    double float_value() const {
        return (double) (unsigned long) value();
    }
    std::string string_value(const std::string& back_buf) const {
        frange_t rng = range();
        return back_buf.substr(rng.first, rng.second);
    }
    ATOM type() const {
        return ATOM(origin().flags()&3);
    }
    std::string str(const char* buf=0) const;

    static Value String (frange_t range) {
        return Value{0, ATOM::STRING, range};
    }
};

// parser debugging 
void report(const char* pb, const char* p, const char* event);

} // namespace ron

namespace std {

    template <>
    struct hash<ron::Uuid> {
        size_t operator()(ron::Uuid const& uuid) const noexcept {
            return uuid.value().hash() ^ (uuid.origin().hash()<<1);
        }
    };

}

#endif
