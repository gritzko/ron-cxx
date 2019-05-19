#ifndef ron_uuid_hpp
#define ron_uuid_hpp
#include <stdint.h>
#include <iostream>
#include <string>
#include <vector>
#include "const.hpp"
#include "portable_endian.hpp"
#include "slice.hpp"

namespace ron {

enum case_t : uint8_t { NUMERIC = 0, SNAKE = 1, CAPS = 2, CAMEL = 3 };
const String CASE_NAMES[]{"numeric", "snake_case", "ALL~CAPS", "CamelCase"};

union Word {
    uint64_t _64;
    uint8_t _8[8];

    Word(uint64_t value = 0) : _64{value} {}
    Word(uint8_t flags, const Slice& data) {
        _64 = flags & 0xfU;
        int i = 0;
        while (i < data.size()) {
            _64 <<= Word::BASE64_BITS;
            _64 |= ABC[data[i]];  // TODO inapprop chars
            i++;
        }
        while (i < Word::MAX_BASE64_SIZE) {
            _64 <<= Word::BASE64_BITS;
            i++;
        }
    }
    Word(const String& word) : Word{0, Slice{word}} {}
    explicit Word(const char* word)
        : Word{0, Slice{word, (fsize_t)strlen(word)}} {}
    Word(ATOM atype, fsize_t offset, fsize_t length) {
        _64 = atype;
        _64 <<= 30U;
        _64 |= offset;
        _64 <<= 30U;
        _64 |= length;
    }
    Word(FLAGS flags, frange_t range)
        : _64{(uint64_t(flags) << 60U) | (uint64_t(range.first) << 30U) |
              range.second} {}

    explicit operator uint64_t() const { return _64; }

    // payload bit size
    static constexpr uint PBS = 60;
    static constexpr int BASE64_BITS = 6;
    static constexpr uint BASE64_WORD_LEN = PBS / BASE64_BITS;
    // max base64 char size
    static constexpr int MAX_BASE64_SIZE = PBS / BASE64_BITS;
    // flag bit size
    static constexpr uint FBS = 64U - PBS;
    static constexpr uint64_t ONE = 1;
    static constexpr uint64_t MAX_VALUE = (ONE << PBS) - 1U;
    static const Word ZERO;
    static const Word NEVER;
    static constexpr uint64_t MAX_VALUE_30 = (1U << 30U) - 1U;
    static constexpr uint8_t OFFSET6[10] = {
        PBS - (6 * 1), PBS - (6 * 2), PBS - (6 * 3), PBS - (6 * 4),
        PBS - (6 * 5), PBS - (6 * 6), PBS - (6 * 7), PBS - (6 * 8),
        PBS - (6 * 9), PBS - (6 * 10)};
    static constexpr uint64_t PAYLOAD_BITS = (ONE << PBS) - 1;
    static constexpr uint64_t FLAG_BITS = UINT64_MAX - PAYLOAD_BITS;
    static constexpr uint64_t LOWER6[11] = {(ONE << (PBS - 0 * 6)) - 1,
                                            (ONE << (PBS - 1 * 6)) - 1,
                                            (ONE << (PBS - 2 * 6)) - 1,
                                            (ONE << (PBS - 3 * 6)) - 1,
                                            (ONE << (PBS - 4 * 6)) - 1,
                                            (ONE << (PBS - 5 * 6)) - 1,
                                            (ONE << (PBS - 6 * 6)) - 1,
                                            (ONE << (PBS - 7 * 6)) - 1,
                                            (ONE << (PBS - 8 * 6)) - 1,
                                            (ONE << (PBS - 9 * 6)) - 1,
                                            0};

    inline fsize_t get30(int pos) const {
        return fsize_t(_64 >> (pos ? 30U : 0U)) & ((1U << 30U) - 1U);
    }
    inline fsize_t higher() const { return (_64 & PAYLOAD_BITS) >> 30; }
    inline fsize_t lower() const { return _64 & (PAYLOAD_BITS >> 30); }
    inline uint8_t flags() const { return _8[7] >> 4U; }
    inline void zero() { _64 = 0U; }
    void write_base64(String& to) const;
    inline uint64_t payload() const { return _64 & MAX_VALUE; }
    inline bool is_zero() const { return _64 == 0U; }
    inline Word inc(uint64_t by = 1UL) const { return Word{_64 + by}; }
    inline Word dec() const { return Word{_64 - 1U}; }
    inline bool operator<(const Word& b) const { return _64 < b._64; }
    inline bool operator>(const Word& b) const { return _64 > b._64; }
    inline bool operator>=(const Word& b) const { return _64 >= b._64; }
    inline bool operator<=(const Word& b) const { return _64 <= b._64; }
    inline bool operator==(const Word& b) const { return _64 == b._64; }
    inline bool operator!=(const Word& b) const { return _64 != b._64; }
    inline Word operator+(const Word& a) const { return Word{_64 + a._64}; }
    inline Word operator+(const uint64_t i) const { return Word{_64 + i}; }
    inline size_t hash() const {
        static constexpr auto _64_hash_fn = std::hash<uint64_t>{};
        return _64_hash_fn(_64);
    }
    inline uint64_t be() const { return htobe64(_64); }
    String str() const {
        String letters{};
        letters.reserve(BASE64_WORD_LEN);
        write_base64(letters);
        return letters;
    }
    bool is_all_digits() const;
    inline frange_t range() const {
        return frange_t{(_64 >> 30U) & Word::MAX_VALUE_30,
                        _64 & Word::MAX_VALUE_30};
    }
    inline int64_t integer() const { return (int64_t)_64; }
    inline double number() const { return *(double*)&_64; }
    inline static Word random() {
        auto i = (uint64_t)rand();
        i <<= 30;
        i ^= (uint64_t)rand();
        return Word{i & MAX_VALUE};
    }
    explicit Word(double val) : _64{*(uint64_t*)&val} {}
    inline explicit operator double() const { return *(double*)this; }
    explicit Word(int64_t val) : _64{*(uint64_t*)&val} {}
    inline explicit operator int64_t() const { return *(int64_t*)this; }
    case_t base64_case() const;
};

enum half_t { VALUE = 0, ORIGIN = 1 };

struct Atom {
    std::pair<Word, Word> words_;
    Atom() : words_{0, 0} {}
    Atom(Word value, Word origin) : words_{value, origin} {}
    Atom(uint64_t value, uint64_t origin) : words_{Word{value}, Word{origin}} {}
    inline Word value() const { return words_.first; }
    inline Word origin() const { return words_.second; }
    // value flag bits
    inline uint8_t vfb() const { return value().flags(); }
    // origin flag bits
    inline uint8_t ofb() const { return origin().flags(); }
    inline Word& word(int i) { return i ? words_.second : words_.first; }
    inline Word& operator[](half_t half) {
        return half ? words_.second : words_.first;
    }
    inline const Word& operator[](half_t half) const {
        return half ? words_.second : words_.first;
    }
    inline VARIANT variant() const { return VARIANT(ofb() >> 2U); }
    static Atom String(frange_t range) {
        return Atom{0, Word{STRING_ATOM, range}};
    }
    static Atom Float(double value, frange_t range) {
        return Atom{Word{value}, Word{FLOAT_ATOM, range}};
    }
    static Atom Integer(int64_t value, frange_t range) {
        return Atom{Word{value}, Word{INT_ATOM, range}};
    }
    inline ATOM type() const {
        uint8_t fb = ofb();
        if ((fb >> 2U) == 0) return ATOM::UUID;
        return (ATOM)(fb & 3U);
    }
};

struct Uuid : public Atom {
    Uuid() : Atom{0, 0} {}
    Uuid(Word value, Word origin) : Atom{value, origin} {}
    explicit Uuid(Slice data) noexcept;
    // pre-parsed Uuid, got hints and correctness guarantee
    Uuid(char variety, const Slice& value, char version, const Slice& origin)
        : Uuid{Word{ABC[(int)variety], value},
               Word{ABC[(int)version], origin}} {}
    explicit Uuid(const ron::String& buf) : Uuid{Slice{buf}} {}
    explicit Uuid(const char* buf)
        : Uuid{Slice{buf, static_cast<fsize_t>(strlen(buf))}} {}
    explicit Uuid(const Atom& a) : Atom{a} {}
    inline enum UUID version() const { return (enum UUID)(ofb() & 3U); }
    inline uint8_t variety() const { return vfb(); }
    inline Word& word(int a, int i) { return Atom::word(i); }
    void write_base64(ron::String& to) const;
    ron::String str() const;
    inline bool zero() const { return value() == 0; }
    inline bool is_ambiguous() const {
        return origin().is_zero() && value().is_all_digits();
    }
    inline bool is_error() const { return origin() == Word::MAX_VALUE; }
    static Uuid Time(Word value, Word origin) {
        return Uuid{value, (uint64_t(origin) & Word::PAYLOAD_BITS) |
                               (uint64_t(UUID::TIME) << Word::PBS)};
    }
    static Uuid Derived(Word value, Word origin) {
        return Uuid{value, (uint64_t(origin) & Word::PAYLOAD_BITS) |
                               (uint64_t(UUID::DERIVED) << Word::PBS)};
    }
    inline Uuid derived() const { return Derived(value(), origin()); }
    inline Uuid event() const { return Time(value(), origin()); }
    inline Uuid inc(uint64_t by = 1UL) const {
        return Uuid{value().inc(by), origin()};
    }
    inline Uuid dec() const { return Uuid{value().dec(), origin()}; }
    inline bool operator<(const Uuid& b) const { return words_ < b.words_; }
    inline bool operator>(const Uuid& b) const { return words_ > b.words_; }
    inline bool operator==(const Uuid& b) const { return words_ == b.words_; }
    inline bool operator==(const Atom& b) const { return words_ == b.words_; }
    inline bool operator==(const char* b) const { return *this == Uuid{b}; }
    inline bool operator!=(const Uuid& b) const { return words_ != b.words_; }
    inline bool operator<=(const Uuid& b) const { return words_ <= b.words_; }
    inline bool operator>=(const Uuid& b) const { return words_ >= b.words_; }
    inline Uuid operator+(uint64_t i) const {
        return Uuid{value() + i, origin()};
    }
    inline bool operator==(const ron::String& str) const {
        return *this == Uuid{str};
    }

    /** Nil UUID as per RFC4122 */
    static const Uuid NIL;
    static const Uuid FATAL;
    static const Uuid NEVER;
    static const Uuid COMMENT;

    inline static Uuid Parse(char variety, const Slice& value, char version,
                             const Slice& origin) {
        return Uuid{Word{ABC[(int)variety], value},
                    Word{ABC[(int)version], origin}};
    }

    static Word HybridTime(time_t seconds, long int nanos = 0);
    static Word Now();
};

using uint64pair = std::pair<uint64_t, uint64_t>;

using Spec = std::pair<Uuid, Uuid>;
using Atoms = std::vector<Atom>;
using Uuids = std::vector<Uuid>;

}  // namespace ron

namespace std {

template <>
struct hash<ron::Word> {
    size_t operator()(ron::Word const& word) const noexcept {
        return word.hash();
    }
};

template <>
struct hash<ron::Uuid> {
    size_t operator()(ron::Uuid const& uuid) const noexcept {
        return uuid.value().hash() ^ (uuid.origin().hash() << 1U);
    }
};

inline ostream& operator<<(ostream& to, ron::Uuid uuid) {
    return to << uuid.str();
}

}  // namespace std

#endif
