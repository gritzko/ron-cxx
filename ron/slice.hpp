#ifndef RON_SLICE_HPP
#define RON_SLICE_HPP
#include <cassert>
#include <cstdint>
#include <cstring>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace ron {

/** An Unicode codepoint. */
using Codepoint = uint32_t;

using Char = unsigned char;

using CharRef = const Char*;

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
constexpr fsize_t FSIZE_MAX{1 << 30};
using frange_t = std::pair<fsize_t, fsize_t>;

/** A reference to a raw memory slice. Same function as rocksdb::Slice.
 * Can't use an iterator range cause have to reference raw buffers (file
 * reads, mmaps, whatever the db passes to us...).
 * A Slice does NOT own the memory! */
struct Slice {
    const Char* buf_;
    fsize_t size_;

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
    Slice(const String& str, const frange_t& range)
        : buf_{reinterpret_cast<CharRef>(str.data()) + range.first},
          size_{range.second} {
        assert(str.size() >= range.first + range.second);
    }
    Slice(Slice host, frange_t range)
        : Slice{host.buf_ + range.first, range.second} {
        assert(host.size_ >= range.first + range.second);
    }

    inline const Char* begin() const { return buf_; }

    inline const Char* end() const { return buf_ + size_; }

    inline Char operator[](fsize_t idx) const {
        assert(idx < size_);
        return buf_[idx];
    }

    inline void operator++() {
        assert(size_ > 0);
        ++buf_;
        --size_;
    }

    inline Char operator*() const {
        assert(size_ > 0);
        return *buf_;
    }

    inline const Char* data() const { return buf_; }
    inline fsize_t size() const { return size_; }

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

    inline frange_t range_of(Slice sub) const {
        assert(sub.begin() >= begin());
        assert(end() >= sub.end());
        return frange_t{sub.buf_ - buf_, sub.size_};
    }

    inline Slice slice(frange_t range) const {
        assert(size_ >= range.second + range.first);
        return Slice{buf_ + range.first, range.second};
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
