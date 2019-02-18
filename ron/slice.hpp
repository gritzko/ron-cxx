#ifndef CPP_SLICE_H
#define CPP_SLICE_H
#include <cassert>
#include <cstdint>
#include <cstring>
#include <functional>
#include <utility>

namespace ron {

/** Frame size or any other size limited by frame size (e.g. string length) */
typedef uint32_t fsize_t;
/** Max RON frame size is 1<<30 (a frame is atomically processed, so 1GB max) */
constexpr fsize_t FSIZE_MAX{1 << 30};
typedef std::pair<fsize_t, fsize_t> frange_t;

struct slice_t {
    const char* buf_;
    fsize_t size_;

    explicit slice_t(const char* buf, fsize_t size = 0)
        : buf_{buf}, size_{size} {}
    explicit slice_t(const uint8_t* buf, size_t size = 0)
        : buf_{(const char*)buf}, size_{static_cast<fsize_t>(size)} {
        assert(size < FSIZE_MAX);
    }
    slice_t() : buf_{nullptr}, size_{0} {}
    slice_t(const slice_t& orig) : buf_{orig.buf_}, size_{orig.size_} {}
    slice_t(const std::string& data)
        : slice_t{data.data(), static_cast<fsize_t>(data.size())} {}
    slice_t(const std::string& str, const frange_t& range)
        : buf_{str.data() + range.first}, size_{range.second} {
        assert(str.size() >= range.first + range.second);
    }
    slice_t(slice_t host, frange_t range)
        : slice_t{host.buf_ + range.first, range.second} {
        assert(host.size_ >= range.first + range.second);
    }

    inline const char* begin() const { return buf_; }

    inline const char* end() const { return buf_ + size_; }

    inline char operator[](fsize_t idx) const {
        assert(idx < size_);
        return buf_[idx];
    }

    inline fsize_t size() const { return size_; }

    bool operator==(const slice_t b) const {
        return size() == b.size() && strncmp(buf_, b.buf_, size()) == 0;
    }

    bool same(const slice_t b) const {
        return buf_ == b.buf_ && size_ == b.size_;
    }

    size_t hash() const {
        static constexpr int shift = sizeof(size_t) == 8 ? 3 : 2;
        static constexpr auto sz_hash_fn = std::hash<size_t>{};
        static constexpr auto char_hash_fn = std::hash<char>{};
        size_t ret = 0;
        int c = size_ >> shift;
        auto* szbuf = (size_t*)buf_;
        for (int i = 0; i < c; i++) {
            ret ^= sz_hash_fn(szbuf[i]);
        }
        for (int i = c << shift; i < size_; i++) {
            ret ^= char_hash_fn(buf_[i]);
        }
        return ret;
    }

    inline void begin(const char* to) {
        buf_ = to;
        size_ = 0;
    }

    inline void end(const char* to) {
        assert(to >= buf_);
        assert(to - buf_ < (1 << 30));
        size_ = fsize_t(to - buf_);
    }

    inline std::string str() const { return std::string{buf_, size_}; }

    inline frange_t range_of(slice_t sub) const {
        assert(sub.begin() >= begin());
        assert(end() >= sub.end());
        return frange_t{sub.buf_ - buf_, sub.size_};
    }

    inline slice_t slice(frange_t range) const {
        assert(size_ >= range.second + range.first);
        return slice_t{buf_ + range.first, range.second};
    }
};

}  // namespace ron

namespace std {

template <>
struct hash<ron::slice_t> {
    size_t operator()(ron::slice_t const& s) const noexcept { return s.hash(); }
};

}  // namespace std
#endif  // CPP_SLICE_H
