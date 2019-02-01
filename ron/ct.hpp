#ifndef RON_CT_HPP
#define RON_CT_HPP
#include <vector>
#include "uuid.hpp"

namespace ron {

/** Chain span metadata: head id, length, reference as an offset. */
class Span {
    Uuid head_;
    fsize_t len_;
    fsize_t ref_;  // use 1 bit for tombstone flag

public:
    explicit Span(const Uuid& head_id, fsize_t backref = 0)
        : head_{head_id}, len_{1}, ref_{backref} {}

    inline Uuid head() const { return head_; }
    inline Uuid last() const { return head_.inc(len_ - 1); }
    void extend(fsize_t by = 1) { len_ += by; }
    inline bool has(const Uuid& id) const {
        return id.origin() == head_.origin() && head_.value() <= id.value() &&
               head_.value().inc(len_) > id.value();
    }
    bool appends(const Uuid& id, const Uuid& ref) const {
        return ref==last() && id==last().inc();
    }
    inline fsize_t length() const { return len_; }
    inline fsize_t ref() const { return ref_; }
};


/** Causal tree metadata, stored as a vector */
class CT {
    typedef std::vector<Span> Spans;
    typedef typename Spans::const_iterator SpIter;

    Spans spans_;
    fsize_t len_;

   public:
    explicit CT()
        : spans_{}, len_{0} {}

    class const_iterator {

        SpIter span_;
        fsize_t off_;
        fsize_t pos_;

        const_iterator(const SpIter& ci, fsize_t offset = 0, fsize_t pos = 0)
            : span_{ci}, off_{offset}, pos_{pos} {}

        friend class CT;

       public:
        inline bool at_head() const { return off_ == 0; }
        inline bool at_tail() const { return off_ + 1 == span_->length(); }
        inline explicit operator fsize_t() const { return pos_; }

        void Next() {
            ++pos_;
            if (at_tail())
                NextSpan();
            else
                ++off_;
        }
        void NextSpan() {
            pos_ += span_->length() - off_;
            ++span_;
            off_ = 0;
        }
        void Prev() {
            --pos_;
            if (at_head()) {
                PrevSpan();
            } else {
                ++off_;
            }
        }
        void PrevSpan() {
            pos_ -= off_ + 1;
            --span_;
            off_ = span_->length() - 1;
        }
        void CausallyPrev() {
            if (!at_head()) {
                --off_;
                --pos_;
            } else {
                fsize_t diff = pos_ - span_->ref();
                // etc
            }
        }
        inline void operator++() { Next(); }
        inline Uuid operator*() { return span_->head().inc(off_); }
        bool operator==(const const_iterator& b) const {
            return span_ == b.span_ && off_ == b.off_;
        }
    };

    const_iterator find(const Uuid& id, bool reverse = false) const {
        return const_iterator{spans_.begin(), 0};
    }

    void Append(const Uuid& id, const Uuid& ref) {
        if (spans_.empty()) {
            spans_.push_back(Span{id});
        } else if (spans_.back().appends(id, ref)) {
            spans_.back().extend(1);
        } else {
            const_iterator backref = find(ref);
            spans_.push_back(Span{id, (fsize_t)backref});
        }
        len_++;
    }

    inline const_iterator begin() const {
        return const_iterator{spans_.begin(), 0};
    }

    inline const_iterator end() const {
        return const_iterator{spans_.end(), 0};
    }

    template <class Cursor>
    inline void Append(Cursor& cur) {
        Append(cur.id(), cur.ref());
    }

    template <class Cursor>
    void AppendAll(Cursor& cur) {
        while (cur.valid()) {
            Append<Cursor>(cur);
            cur.Next();
        }
    }
};


}  // namespace ron

#endif
