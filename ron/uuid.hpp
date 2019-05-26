#ifndef ron_uuid_hpp
#define ron_uuid_hpp
#include <stdint.h>
#include <string>
#include <vector>
#include "const.hpp"
#include "portable_endian.hpp"
#include "slice.hpp"

namespace ron {

enum case_t : uint8_t { NUMERIC = 0, SNAKE = 1, CAPS = 2, CAMEL = 3 };
const String CASE_NAMES[]{"numeric", "snake_case", "ALL~CAPS", "CamelCase"};

union Word {
    uint64_t as_u64;
    uint32_t as_u32[2];
    uint8_t as_u8[8];
    int64_t as_integer;
    double as_float;
    Codepoint as_codepoint[2];
    Range as_range;
    fsize_t as_size[2];
    struct {
        Codepoint cp;
        fsize_t cp_size;
    };

    Word(uint64_t value = 0) noexcept : as_u64{value} {}
#ifdef __LITTLE_ENDIAN
    Word(uint32_t higher, uint32_t lower) noexcept : as_u32{lower, higher} {}
#elif
    Word(uint32_t higher, uint32_t lower) noexcept : _32{higher, lower} {}
#endif
    /** A trusty parsing constructor; expects a valid Base64x64 value. */
    explicit Word(uint8_t flags, const Slice& data) noexcept
        : as_u64{flags & 0xfU} {
        assert(data.size() <= MAX_BASE64_SIZE);
        int i = 0;
        while (i < data.size()) {
            as_u64 <<= Word::BASE64_BITS;
            as_u64 |= ABC[data[i]];
            i++;
        }
        while (i < Word::MAX_BASE64_SIZE) {
            as_u64 <<= Word::BASE64_BITS;
            i++;
        }
    }
    /** A trusty parsing constructor. */
    explicit Word(const String& word) : Word{0, Slice{word}} {}
    /** A trusty parsing constructor. */
    explicit Word(const char* word) noexcept
        : Word{0, Slice{word, (fsize_t)strlen(word)}} {}

    explicit Word(Range range) noexcept : as_range{range} {}

    explicit operator uint64_t() const { return as_u64; }

    // payload bit size
    static constexpr uint PBS = 60;
    static constexpr uint BASE64_BITS = 6;
    static constexpr uint BASE64_WORD_LEN = PBS / BASE64_BITS;
    // max base64 char size
    static constexpr int MAX_BASE64_SIZE = PBS / BASE64_BITS;
    // flag bit size
    static constexpr uint FBS = 64U - PBS;
    static constexpr uint64_t ONE = 1;
    static constexpr uint64_t MAX_VALUE = (ONE << PBS) - 1U;
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

    inline uint8_t flags() const { return as_u8[7] >> 4U; }
    inline void zero() { as_u64 = 0U; }
    void write_base64(String& to) const;
    inline uint64_t payload() const { return as_u64 & MAX_VALUE; }
    inline bool is_zero() const { return as_u64 == 0U; }
    inline Word inc(uint64_t by = 1UL) const { return Word{as_u64 + by}; }
    inline Word dec() const { return Word{as_u64 - 1U}; }
    inline bool operator<(const Word& b) const { return as_u64 < b.as_u64; }
    inline bool operator>(const Word& b) const { return as_u64 > b.as_u64; }
    inline bool operator>=(const Word& b) const { return as_u64 >= b.as_u64; }
    inline bool operator<=(const Word& b) const { return as_u64 <= b.as_u64; }
    inline bool operator==(const Word& b) const { return as_u64 == b.as_u64; }
    inline bool operator!=(const Word& b) const { return as_u64 != b.as_u64; }
    inline Word operator+(const Word& a) const {
        return Word{as_u64 + a.as_u64};
    }
    inline Word operator+(const uint64_t i) const { return Word{as_u64 + i}; }
    inline void operator++() { ++as_u64; }
    inline Word operator|(uint64_t mask) const { return Word{as_u64 | mask}; }
    inline Word operator&(uint64_t mask) const { return Word{as_u64 & mask}; }

    inline size_t hash() const {
        static constexpr auto _64_hash_fn = std::hash<uint64_t>{};
        return _64_hash_fn(as_u64);
    }
    inline uint64_t be() const { return htobe64(as_u64); }
    inline Range range() const { return as_range.safe(); }
    String str() const {
        String letters{};
        letters.reserve(BASE64_WORD_LEN);
        write_base64(letters);
        return letters;
    }
    inline static Word random() {
        auto i = (uint64_t)rand();
        i <<= 30;
        i ^= (uint64_t)rand();
        return Word{i & MAX_VALUE};
    }

    explicit Word(double val) : as_u64{*(uint64_t*)&val} {}
    explicit Word(int64_t val) : as_u64{*(uint64_t*)&val} {}

    case_t base64_case() const;
    bool is_all_digits() const;
};

const Word NEVER{uint64_t(63UL << 54U)};
const Word ZERO{0UL};

enum half_t { VALUE = 0, ORIGIN = 1 };

struct Atom {
    Word origin, value;

    Atom(Word val, Word orig) : value{val}, origin{orig} {}
    Atom() : Atom{ZERO, ZERO} {}
    Atom(uint64_t value, uint64_t origin) : Atom{Word{value}, Word{origin}} {}

    static inline Atom String(Codepoint cp, Range range, fsize_t cp_size) {
        return Atom{Word{cp_size, cp}, Word{range} | STRING_FLAGS};
    }
    static inline Atom Integer(Integer i, Range range) {
        return Atom{Word{i}, Word{range} | INT_FLAGS};
    }

    Atom(ATOM type, Range range)
        : Atom{ZERO, Word{range} | (uint64_t(type) << 62U)} {}
    inline ATOM type() const { return (ATOM)(origin.as_u64 >> 62U); }
    inline Word safe_origin() const {
        constexpr uint64_t mask = (uint64_t(FSIZE_BITS) << 32) | FSIZE_BITS;
        return Word{origin.as_u64 & mask};
    }
    inline bool operator==(const Atom& b) const {
        return value == b.value && origin == b.origin;
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
    inline enum UUID version() const {
        return (enum UUID)(origin.flags() & 3U);
    }
    inline uint8_t variety() const { return value.flags(); }
    void write_base64(ron::String& to) const;
    ron::String str() const;
    inline bool zero() const { return value == ZERO; }
    inline bool is_ambiguous() const {
        return origin.is_zero() && value.is_all_digits();
    }
    inline bool is_error() const { return origin == Word::MAX_VALUE; }
    static Uuid Time(Word value, Word origin) {
        return Uuid{value, (uint64_t(origin) & Word::PAYLOAD_BITS) |
                               (uint64_t(UUID::TIME) << Word::PBS)};
    }
    static Uuid Derived(Word value, Word origin) {
        return Uuid{value, (uint64_t(origin) & Word::PAYLOAD_BITS) |
                               (uint64_t(UUID::DERIVED) << Word::PBS)};
    }
    inline Uuid derived() const { return Derived(value, origin); }
    inline Uuid event() const { return Time(value, origin); }
    inline Uuid inc(uint64_t by = 1UL) const {
        return Uuid{value.inc(by), origin};
    }
    inline Uuid dec() const { return Uuid{value.dec(), origin}; }

    inline bool operator<(const Uuid& b) const {
        return value < b.value || (value == b.value && origin < b.origin);
    }
    inline bool operator>(const Uuid& b) const { return b < *this; }
    inline bool operator==(const Uuid& b) const {
        return value == b.value && origin == b.origin;
    }
    inline bool operator!=(const Uuid& b) const { return !(*this == b); }
    inline bool operator<=(const Uuid& b) const { return !(b < *this); }
    inline bool operator>=(const Uuid& b) const { return !(*this < b); }

    inline bool operator==(const char* b) const { return *this == Uuid{b}; }
    inline Uuid operator+(uint64_t i) const { return Uuid{value + i, origin}; }
    inline bool operator==(const ron::String& str) const {
        return *this == Uuid{str};
    }
    inline void operator++() { ++value; }

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

    std::string ToString() const;
};

using uint64pair = std::pair<uint64_t, uint64_t>;

using Spec = std::pair<Uuid, Uuid>;
using Atoms = std::vector<Atom>;
using Uuids = std::vector<Uuid>;

constexpr int OP_ID_IDX{0};
constexpr int OP_REF_IDX{1};
#define A2U(a) static_cast<Uuid>(a)

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
        return uuid.value.hash() ^ (uuid.origin.hash() << 1U);
    }
};

inline ostream& operator<<(ostream& to, ron::Uuid uuid) {
    return to << uuid.str();
}

}  // namespace std

#endif
