#ifndef RON_SPANS_HPP
#define RON_SPANS_HPP
#include <vector>
#include "status.hpp"
#include "uuid.hpp"

namespace ron {


/** Chain span metadata: head id, length, reference/payload. */
class Span {
    Uuid head_;
    fsize_t len_;
    fsize_t ref_;  // use 1 bit for tombstone flag

   public:
    explicit Span(const Uuid& head_id, fsize_t backref = 0)
        : head_{head_id}, len_{1}, ref_{backref} {}

    inline Uuid head() const { return head_; } // TODO elim
    inline Uuid front() const { return head_; }
    inline Uuid back() const { return head_.inc(len_ - 1); }
    void extend (fsize_t by = 1) { len_ += by; }
    inline bool has(const Uuid& id) const {
        return id.origin() == head_.origin() && head_.value() <= id.value() &&
               head_.value().inc(len_) > id.value();
    }
    bool fits(const Uuid& id, const Uuid& ref) const {
        return ref == back() && id == back().inc();
    }
    bool fits(const Uuid& id) const {
        return id == back().inc();
    }
    inline fsize_t length() const { return len_; } // TODO elim
    inline fsize_t size() const { return len_; }
    inline fsize_t ref() const { return ref_; }
    inline void pop_back () { assert(len_>0); --len_; }
    inline bool push_back (const Uuid& id) {
        if (fits(id)) {
            len_++;
            return true;
        }
        return false;
    }
    inline bool empty() const { return len_==0; }
};


/** A vector of Uuids but stored as Spans, for performance reasons. */
class Uuids {
    using Spans = std::vector<Span>; 
    Spans spans_;
    fsize_t size_;
public:
    Uuids() : spans_{}, size_{0} {}
    void push_back (const Uuid& id) {
        ++size_;
        if (spans_.empty()) {
            spans_.emplace_back(id);
            return;
        }
        if (!spans_.back().push_back(id)) {
            spans_.emplace_back(id);
        }
    }
    inline void push_back (Span span) {
        spans_.push_back(span);
        size_+=span.size();
    }
    void pop_back () {
        assert(!empty());
        --size_;
        if (spans_.back().size()==1) {
            spans_.pop_back();
        } else {
            spans_.back().pop_back();
        }
    }
    inline fsize_t size() const { return size_; }
    inline bool empty() const { return size_==0; }
    const Uuid front() const { return spans_.front().front(); }
    const Uuid back() const { return spans_.back().back(); }

    class const_iterator {
        Spans::const_iterator i_;
        fsize_t off_;
        const_iterator(Spans::const_iterator span, fsize_t offset) : i_{span}, off_{offset} {}
        friend class Uuids;
    public:
        inline void operator ++ () {
            ++off_;
            if (off_>=i_->length()) {
                off_=0;
                ++i_;
            }
        }
        inline void operator -- () {
            if (off_>0) {
                --off_;
            } else {
                --i_;
                off_ = i_->length()-1;
            }
        }
        inline void operator += (fsize_t step) {}
        inline void operator -= (fsize_t step) {}
        inline Uuid operator * () const {
            return i_->head().inc(off_);
        }
        /** for the cases of incrementally increasing payload value */
        inline fsize_t ref_inc() const {
            return i_->ref() + off_;
        }
        inline const Span& span() const {
            return *i_;
        }
        inline bool operator == (const const_iterator& b) {
            return i_==b.i_ && off_==b.off_;
        }
    };

    const_iterator begin() const {
        return const_iterator{spans_.begin(), 0};
    }

    const_iterator end() const {
        return const_iterator{spans_.end(), 0};
    }
};

}

#endif
