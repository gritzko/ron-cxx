#ifndef RON_CT_HPP
#define RON_CT_HPP
#include <vector>
#include "status.hpp"
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
    inline Uuid back() const { return head_.inc(len_ - 1); }
    void extend(fsize_t by = 1) { len_ += by; }
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
    inline fsize_t length() const { return len_; }
    inline fsize_t ref() const { return ref_; }
    inline void pop_back () { assert(len_>0); --len_; }
    inline bool empty() const { return len_==0; }
};

typedef std::vector<Span> Spans;

/** Causal tree metadata, stored as a vector */
class CT {
    typedef typename Spans::const_iterator SpIter;

    Spans spans_;
    fsize_t len_;

   public:
    explicit CT() : spans_{}, len_{0} {}

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
                --off_;
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
                // FIXME etc
                assert(false);
            }
        }
        inline void operator++() { Next(); }
        inline void operator--() { Prev(); }
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
        } else if (spans_.back().fits(id, ref)) {
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

class Path {
    Spans spans_;
    fsize_t size_;

   public:
    explicit Path(const Uuid& id) : spans_{}, size_{1} {
        spans_.push_back(Span{id});
    }
    inline fsize_t size() const { return size_; }
    inline void pop_back() {
        assert(!spans_.empty());
        spans_.back().pop_back();
        if (spans_.back().empty()) spans_.pop_back();
        --size_;
    }
    inline void push_back(const Uuid& id) {
        if (spans_.empty() || !spans_.back().fits(id)) {
            spans_.push_back(Span{id});
        } else {
            spans_.back().extend(1);
        }
        size_++;
    }
    inline Uuid back() const { return spans_.back().back(); }
};

/** A root-to-leave path in a causal tree. */
class CTPath {
    Path path_;
    fsize_t pos_;

   public:
    explicit CTPath(const Uuid& id) : path_{id}, pos_{0} {}
    Status AddNext(const Uuid& id, const Uuid& ref) {
        while (path_.size() && path_.back() > ref) path_.pop_back();
        if (-1 == depth() || path_.back() != ref)
            return Status::BADARGS.comment("not a single-root causal tree!");
        path_.push_back(id);
        ++pos_;
        return Status::OK;
    }
    template <typename Cursor>
    Status AddNext(const Cursor& c) {
        return AddNext(c.id(), c.ref());
    }
    inline fsize_t depth() const { return path_.size()-1; }
    inline fsize_t position() const { return pos_; }
};

const Uuid RM_UUID;
const Uuid UN_UUID;

template <typename Cursor>
class CTVisibilityMap {
    std::vector<bool> vm_;
    CTPath path_;

    inline fsize_t depth() const { return path_.depth(); }

    Status ScanSubtree(Cursor& cur) {
        std::vector<bool> rms;
        fsize_t start = depth();
        Status ok{};
        while (cur.valid() && depth() > start && ok) {
            if (cur.has(2, RM_UUID) || cur.has(2, UN_UUID)) {
                ok = ScanRmUnSubtree(cur, rms);
                // mark the stack
                for (fsize_t i, j; ok && i < rms.size(); i++) vm_[i] = rms[j];
                rms.clear();
            } else {
                cur.Next();
                ok = path_.AddNext(cur);
            }
        }
        return ok;
    }

    Status ScanRmUnSubtree(Cursor& cur, std::vector<bool>& rms) {
        fsize_t start = depth();
        Status ok{};
        while (cur.valid() && ok && depth() > start) {
            if (cur.has(2, UN_UUID)) {
                fsize_t un_depth;
                ok = ScanUnSubtree(cur, un_depth);
                // mark the stack
            } else {
                cur.Next();
                ok = path_.AddNext(cur);
                // apply
            }
        }
        return ok;
    }

    Status ScanUnSubtree(Cursor& cur, fsize_t& un_depth) {
        fsize_t start = depth();
        Status ok{};
        while (cur.valid() && ok && cur.has(2, UN_UUID) && depth() > start) {
            // track the tree; strictly one subtree!
            cur.Next();
            ok = path_.AddNext(cur);
        }
        return ok;
    }

    Status ScanTrashSubtree(Cursor& cur) {
        return Status::OK;
    }

   public:
    explicit CTVisibilityMap(Cursor& tree) : vm_{}, path_{tree.id()} {
        tree.Next();
        ScanSubtree(tree);
    }

    inline const std::vector<bool>& visibility() const { return vm_; }
};

}  // namespace ron

#endif
