#ifndef RON_RDT_MERGE_HPP
#define RON_RDT_MERGE_HPP

#include "../ron/status.hpp"

namespace ron {

// asc-sorting iterator heap
template <typename Frame, typename Frame::Cursor::Comparator Less>
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
    const Atoms& op() const { return cursors_.front()->op(); }
    const Frame& frame() const { return cursors_.front()->frame(); }
    const Cursor& current() const { return *cursors_.front(); }

   private:
    static inline fsize_t up(fsize_t idx) { return ((idx + 1) >> 1U) - 1; }

    static inline fsize_t left(fsize_t idx) { return ((idx + 1) << 1U) - 1; }

    static inline fsize_t right(fsize_t idx) { return (idx + 1) << 1U; }

    inline const Cursor& op(fsize_t idx) const { return cursors_[idx]; }

    inline fsize_t size() const { return (fsize_t)cursors_.size(); }

    inline bool less_than(fsize_t a, fsize_t b) const {
        return Less(*cursors_[a], *cursors_[b]);
    }

    inline void swap(fsize_t a, fsize_t b) {
        std::swap(cursors_[a], cursors_[b]);
    }

    void pop(fsize_t idx) {
        if (idx == 0) {
            return;
        }
        fsize_t u = up(idx);
        if (less_than(u, idx)) {
            return;
        }
        swap(idx, u);
        pop(u);
    }

    void push(fsize_t idx) {
        fsize_t l = left(idx);
        fsize_t r = right(idx);
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
        Uuid id{op()[OP_ID_IDX]};
        while (step() && Uuid{op()[OP_ID_IDX]} == id)
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
        output.EndChunk();
        return Status::OK;
    }
};

}  // namespace ron

#endif
