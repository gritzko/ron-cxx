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

/** Max RON frame size is 1<<30 (a frame is atomically processed, so 1GB max) */
constexpr fsize_t FSIZE_MAX{1U << 30U};
constexpr fsize_t FSIZE_BITS{FSIZE_MAX - 1};

/** A [from,till) byte range. Limited to 2^30 bytes due to FSIZE_MAX. */
class Range {
    fsize_t limits_[2];

public:

    Range(fsize_t from_offset, fsize_t till_offset)
#ifdef __LITTLE_ENDIAN
        : limits_{from_offset, till_offset}
#elif
        : limits_{till_offset, from_offset}
#endif
        {}

    static inline Range FroTo(fsize_t from_offset, fsize_t till_offset) {
        return Range{from_offset, till_offset};
    }
    static inline Range AtFor(fsize_t at_offset, fsize_t for_length) {
        return Range{at_offset, at_offset + for_length};
    }
    Range() : Range{0, 0} {}
    inline fsize_t from() const { return limits_[LEAST_SIGNIFICANT]; }
    inline fsize_t till() const { return limits_[MOST_SIGNIFICANT]; }
    inline fsize_t safe_from() const {
        assert(valid());
        return from() & FSIZE_BITS;
    }
    inline fsize_t safe_till() const {
        assert(valid());
        return till() & FSIZE_BITS;
    }
    inline fsize_t offset() const { return from(); }
    inline fsize_t begin() const { return from(); }
    inline fsize_t end() const { return till(); }
    inline fsize_t length() const { return till() - from(); }
    inline fsize_t size() const { return till() - from(); }
    inline bool empty() const { return till() == from(); }
    inline bool valid() const { return till() >= from(); }
};

/** A reference to a raw memory slice. Same function as rocksdb::Slice.
 * Can't use an iterator range cause have to reference raw buffers (file
 * reads, mmaps, whatever the db passes to us...).
 * A Slice does NOT own the memory! */
struct Slice {
    CharRef buf_;
    fsize_t size_;
    //Range range_;

    explicit Slice(const Char* buf, fsize_t size) : buf_{buf}, size_{size} {}
    explicit Slice(const char* buf, size_t size)
        : buf_{reinterpret_cast<const Char*>(buf)},
          size_{static_cast<fsize_t>(size)} {
        assert(size <= FSIZE_MAX);
    }
    Slice(const Char* from, const Char* till)
        : buf_{from}, size_{static_cast<fsize_t>(till - from)} {
        assert(till >= from);
        assert(till - from <= FSIZE_MAX);
    }
    Slice() : buf_{nullptr}, size_{0} {}
    Slice(const Slice& orig) = default;
    Slice(const String& data)
        : Slice{CharRef(data.data()), static_cast<fsize_t>(data.size())} {}
    Slice(const String& str, const Range& range)
        : buf_{reinterpret_cast<CharRef>(str.data()) + range.offset()},
          size_{range.length()} {
        assert(str.size() >= range.offset() + range.length());
    }
    Slice(Slice host, Range range)
        : Slice{host.buf_ + range.offset(), range.length()} {
        assert(host.size_ >= range.end());
    }

    inline const CharRef begin() const { return buf_; }

    inline const CharRef end() const { return buf_ + size_; }

    inline Char operator[](fsize_t idx) const {
        assert(idx < size_);
        return buf_[idx];
    }

    inline void operator++() {
        assert(size_ > 0);
        ++buf_;
        --size_;
    }

    inline void advance(fsize_t sz) {
        assert(sz <= size_);
        buf_ += sz;
        size_ -= sz;
    }

    inline Char operator*() const {
        assert(size_ > 0);
        return *buf_;
    }

    inline const Char* data() const { return buf_; }
    inline fsize_t size() const { return size_; }
    inline bool empty() const { return size_ == 0; }

    bool operator==(const Slice b) const {
        return size() == b.size() && memcmp(buf_, b.buf_, size()) == 0;
    }

    bool same(const Slice b) const {
        return buf_ == b.buf_ && size_ == b.size_;
    }

    size_t hash() const {
        static constexpr int SHIFT = sizeof(size_t) == 8 ? 3 : 2;
        static constexpr auto SZ_HASH_FN = std::hash<size_t>{};
        static constexpr auto CHAR_HASH_FN = std::hash<char>{};
        size_t ret = 0;
        fsize_t c = size_ >> SHIFT;
        auto szbuf = reinterpret_cast<const size_t*>(buf_);
        for (fsize_t i = 0; i < c; i++) {
            ret ^= SZ_HASH_FN(szbuf[i]);
        }
        for (fsize_t i = c << SHIFT; i < size_; i++) {
            ret ^= CHAR_HASH_FN(buf_[i]);
        }
        return ret;
    }

    inline String str() const {
        return String{reinterpret_cast<const String::value_type*>(buf_), size_};
    }

    inline Range range_of(Slice sub) const {
        assert(sub.begin() >= begin());
        assert(end() >= sub.end());
        return Range::AtFor(static_cast<fsize_t>(sub.buf_ - buf_), sub.size_);
    }

    inline Slice slice(Range range) const {
        assert(size_ >= range.end());
        return Slice{buf_ + range.offset(), range.length()};
    }
};

}  // namespace ron

namespace std {

template <>
struct hash<ron::Slice> {
    size_t operator()(ron::Slice const& s) const noexcept { return s.hash(); }
};

}  // namespace std
#endif  // CPP_SLICE_H
