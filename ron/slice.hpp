#ifndef RON_SLICE_HPP
#define RON_SLICE_HPP
#include <cassert>
#include <cstdint>
#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>
#include "portable_endian.hpp"

namespace ron {

/** An Unicode codepoint. */
using Codepoint = uint32_t;
using Codepoints = std::vector<Codepoint>;

using Char = unsigned char;

using CharRef = Char const*;

constexpr Codepoint CP_ERROR{0};

/** A byte string .ON strings are valid canonic UTF-8, but we treat them
    as byte buffers most of the time. For symbol-level work, use String16. */
using String = std::string;
using Strings = std::vector<String>;

/** It would be nice to use unsigned chars everywhere. It is quite a footgun
    that C chars are either signed or unsigned. Unfortunately, std::string
    appears in many APIs, so it is more work than it's worth. Use Slice
    for safe unsigned chars. */
// typedef std::basic_string<Char> String;

/** An UTF-16 string. Necessary for symbol-level operations. */
using String16 = std::u16string;
using Strings16 = std::vector<String16>;

/** Frame size or any other size limited by frame size (e.g. string length) */
using fsize_t = uint32_t;
#define FSIZE(s) static_cast<fsize_t>(s)

/** Max RON frame size is 1<<30 (a frame is atomically processed, so 1GB max) */
constexpr fsize_t FSIZE_MAX{1U << 30U};
constexpr fsize_t FSIZE_BITS{FSIZE_MAX - 1};

/** A [from,till) byte range. Limited to 2^30 bytes due to FSIZE_MAX. */
class Range {
    fsize_t limits_[2];

   public:
    Range(fsize_t from_offset, fsize_t till_offset)
#ifdef __LITTLE_ENDIAN
        : limits_ {
        from_offset, till_offset
    }
#elif
        : limits_ {
        till_offset, from_offset
    }
#endif
    {}

    Range(CharRef base, CharRef begin, CharRef end)
        : Range{fsize_t(begin - base), fsize_t(end - base)} {
        assert(valid());
    }

    static inline Range FroTo(fsize_t from_offset, fsize_t till_offset) {
        return Range{from_offset, till_offset};
    }
    static inline Range FroToL(long from_offset, long till_offset) {
        return Range{static_cast<fsize_t>(from_offset),
                     static_cast<fsize_t>(till_offset)};
    }
    static inline Range AtFor(fsize_t at_offset, fsize_t for_length) {
        return Range{at_offset, at_offset + for_length};
    }
    Range() : Range{0U, 0U} {}
    inline fsize_t begin() const { return limits_[LEAST_SIGNIFICANT]; }
    inline fsize_t end() const { return limits_[MOST_SIGNIFICANT]; }
    inline fsize_t safe_begin() const {
        assert(valid());
        return begin() & FSIZE_BITS;
    }
    inline fsize_t safe_end() const {
        assert(valid());
        return end() & FSIZE_BITS;
    }
    inline Range safe() const {
        return Range{begin() & FSIZE_BITS, end() & FSIZE_BITS};
    }
    inline fsize_t offset(fsize_t idx = 0) const {
        assert(begin() + idx < end());
        return begin() + idx;
    }
    inline fsize_t size() const {
        assert(valid());
        return end() - begin();
    }
    inline fsize_t length() const { return size(); }
    inline bool empty() const { return begin() == end(); }
    inline bool valid() const { return end() >= begin(); }
    inline void consume(fsize_t length) {
        limits_[LEAST_SIGNIFICANT] += length;
        assert(valid());
    }
    inline void shorten(fsize_t by_length) {
        limits_[MOST_SIGNIFICANT] -= by_length;
        assert(valid());
    }
    inline void EndAt(fsize_t offset) {
        limits_[MOST_SIGNIFICANT] = offset;
        assert(valid());
    }
    inline void resize(fsize_t new_size) {
        limits_[MOST_SIGNIFICANT] = limits_[LEAST_SIGNIFICANT] + new_size;
    }
    inline void operator++() { consume(1); }
    inline bool operator==(const Range& b) const {
        return *(uint64_t*)limits_ == *(uint64_t*)(b.limits_);
    }
    inline bool operator!=(const Range& b) const { return !(*this == b); }
};

/** A reference to a raw memory slice. Same function as rocksdb::Slice.
 * Can't use an iterator range cause have to reference raw buffers (file
 * reads, mmaps, whatever the db passes to us...).
 * A Slice does NOT own the memory! */
class Slice {
    CharRef buf_;
    Range range_;

   public:
    Slice(CharRef data, Range range) : buf_{data}, range_{range} {}

    Slice(CharRef buf, fsize_t size) : buf_{buf}, range_{0, size} {}

    Slice(const char* buf, size_t size)
        : Slice{reinterpret_cast<const Char*>(buf),
                static_cast<fsize_t>(size)} {
        assert(size <= FSIZE_MAX);
    }

    Slice(CharRef from, CharRef till)
        : Slice{from, static_cast<fsize_t>(till - from)} {
        assert(till >= from);
        assert(till - from <= FSIZE_MAX);
    }
    Slice() : Slice{nullptr, (fsize_t)0} {}
    Slice(const Slice& orig) = default;
    Slice(const String& data)
        : Slice{CharRef(data.data()), static_cast<fsize_t>(data.size())} {}

    Slice(const String& str, const Range& range)
        : buf_{reinterpret_cast<CharRef>(str.data())}, range_{range} {
        assert(str.size() >= range.end());
    }

    inline CharRef buffer() const { return buf_; }
    inline Range range() const { return range_; }
    inline const CharRef begin() const { return buf_ + range_.begin(); }
    inline const CharRef end() const { return buf_ + range_.end(); }
    inline Char at(fsize_t idx) const { return buf_[range_.offset(idx)]; }
    inline Char operator[](fsize_t idx) const { return at(idx); }
    inline void operator++() { ++range_; }
    inline void consume(fsize_t sz) { range_.consume(sz); }
    inline Char operator*() const { return *begin(); }
    inline fsize_t size() const { return range_.size(); }
    inline bool empty() const { return range_.empty(); }
    inline CharRef data() const { return buf_; }

    inline Slice reset() const { return Slice{buf_, Range{0, range_.end()}}; }
    inline Slice consumed() const {
        return Slice{buf_, Range{0, range_.begin()}};
    }
    inline void EndAt(fsize_t offset) { range_.EndAt(offset); }

    bool operator==(const Slice b) const {
        return size() == b.size() && memcmp(begin(), b.begin(), size()) == 0;
    }

    bool same(const Slice b) const {
        return begin() == b.begin() && end() == b.end();
    }

    size_t hash() const {
        static constexpr int SHIFT = sizeof(size_t) == 8 ? 3 : 2;
        static constexpr auto SZ_HASH_FN = std::hash<size_t>{};
        static constexpr auto CHAR_HASH_FN = std::hash<char>{};
        size_t ret = 0;
        fsize_t c = size() >> SHIFT;
        auto szbuf = reinterpret_cast<const size_t*>(begin());
        for (fsize_t i = 0; i < c; i++) {
            ret ^= SZ_HASH_FN(szbuf[i]);
        }
        for (fsize_t i = c << SHIFT; i < size(); i++) {
            ret ^= CHAR_HASH_FN(at(i));
        }
        return ret;
    }

    inline String str() const {
        return String{reinterpret_cast<const String::value_type*>(buf_),
                      range_.begin(), range_.end()};
    }

    inline Range range_of(Slice sub) const {
        assert(sub.begin() >= begin());
        assert(end() >= sub.end());
        return Range::AtFor(static_cast<fsize_t>(sub.begin() - begin()),
                            sub.size());
    }

    inline Slice slice(Range range) const {
        assert(size() >= range.end());
        Range subrange =
            Range::AtFor(range_.offset(range.begin()), range.size());
        return Slice{buf_, subrange};
    }

    inline Slice CutOff(Slice b) {
        assert(buf_ == b.buf_);
        return Slice{buf_, Range::FroTo(range_.begin(), b.range_.begin())};
    }

    inline void Resize(fsize_t new_size) { range_.resize(new_size); }
};

}  // namespace ron

namespace std {

template <>
struct hash<ron::Slice> {
    size_t operator()(ron::Slice const& s) const noexcept { return s.hash(); }
};

}  // namespace std
#endif  // CPP_SLICE_H
