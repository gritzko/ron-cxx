#ifndef RON_RDT_MERGE_HPP
#define RON_RDT_MERGE_HPP

#include "../ron/op.hpp"
#include "../ron/status.hpp"

namespace ron {

using less_t = bool (*)(const Op& a, const Op& b);

// asc-sorting iterator heap
template <typename Frame, less_t Less>
class MergeCursor {
    using Cursor = typename Frame::Cursor;
    using Frames = std::vector<Frame>;
    using Cursors = std::vector<Cursor>;
    using PCursor = typename Cursors::iterator;

    std::vector<PCursor> cursors_;

   public:
    MergeCursor() = default;

    explicit MergeCursor(Cursors& inputs) : MergeCursor{} {
        for (auto i = inputs.begin(); i != inputs.end(); i++) Add(i);
    }

    void Add(PCursor input) {
        if (!input->valid()) return;
        cursors_.push_back(input);
        pop((int)cursors_.size() - 1);
    }
    // no more ops
    bool empty() const { return size() == 0; }
    // returns the current op
    const Op& op() const { return cursors_.front()->op(); }
    const Frame& frame() const { return cursors_.front()->frame(); }
    const Cursor& current() const { return *cursors_.front(); }

   private:
    static inline int up(int idx) { return ((idx + 1) >> 1) - 1; }

    static inline int left(int idx) { return ((idx + 1) << 1) - 1; }

    static inline int right(int idx) { return (idx + 1) << 1; }

    inline const Op& op(int idx) const { return cursors_[idx]->op(); }

    inline int size() const { return (int)cursors_.size(); }

    inline bool less_than(int a, int b) const { return Less(op(a), op(b)); }

    inline void swap(int a, int b) { std::swap(cursors_[a], cursors_[b]); }

    void pop(int idx) {
        if (idx == 0) {
            return;
        }
        int u = up(idx);
        if (less_than(u, idx)) {
            return;
        }
        swap(idx, u);
        pop(u);
    }

    void push(int idx) {
        int l = left(idx);
        int r = right(idx);
        if (r < size() && less_than(r, idx)) {  // r is an option
            if (less_than(l, r)) {
                swap(l, idx);
                push(l);
            } else {
                swap(r, idx);
                push(r);
            }
        } else if (l < size() && less_than(l, idx)) {
            swap(l, idx);
            push(l);
        }
    }

    void eject() {
        if (!size()) {
            return;
        }
        cursors_[0] = cursors_.back();
        cursors_.pop_back();
        push(0);
    }

    bool step() {
        if (cursors_[0]->Next()) {
            push(0);
        } else {
            eject();
        }
        return size() > 0;
    }

   public:
    // advances to the next op
    // @return non-empty
    bool Next() {
        Uuid id = op().id();
        while (step() && op().id() == id)
            ;  // idempotency
        return size() > 0;
    }
    // returns the data buffer for the current cursor/op
    const std::string& data() const { return op().data(); }
    //
    Status Merge(typename Frame::Builder& output) {
        if (empty()) return Status::OK;
        do {
            output.AppendOp(*cursors_.front());
        } while (Next());
        return Status::OK;
    }
};

}  // namespace ron

#endif
