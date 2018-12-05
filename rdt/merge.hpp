#ifndef rdt_merge_hpp
#define rdt_merge_hpp

#include "ron/op.hpp"
#include "ron/status.hpp"

namespace ron {

typedef bool (*less_t)(const Op& a, const Op& b);

// asc-sorting iterator heap
template<typename Frame, less_t less_fn>
class MergeCursor {

    typedef std::vector<Frame> Frames;
    typedef typename Frame::Cursor Cursor;

    std::vector<typename Frame::Cursor*> cursors_;

public:
    MergeCursor () : cursors_{} {}
    MergeCursor (const Frames& inputs) : MergeCursor{} {
        for(int i=0; i<inputs.size(); i++) {
            Add(inputs[i]);
        }
    }
    // add a frame to merge
    void Add (const Frame& input) {
        cursors_.push_back(new Cursor{input});
        pop((int)cursors_.size()-1);
    }
    // no more ops
    bool empty() const { return size()==0; }
    // returns the current op
    const Op& op() const { return cursors_[0]->op(); }
    const Frame& frame() const { return cursors_[0]->frame(); }

private:
    static inline int up (int idx) {
        return ((idx+1)>>1) - 1;
    }
    static inline int left (int idx) {
        return ((idx+1)<<1) - 1;
    }
    static inline int right (int idx) {
        return (idx+1)<<1;
    }

    inline const Op& op (int idx) const {
        return cursors_[idx]->op();
    }
    inline int size () const {
        return (int)cursors_.size();
    }
    inline bool less_than(int a, int b) const {
        return less_fn(op(a), op(b));
    }

    inline void swap (int a, int b) {
        std::swap(cursors_[a], cursors_[b]);
    }

    void pop (int idx) {
        if (!idx) return;
        int u = up(idx);
        if (less_than(u, idx)) return;
        swap(idx, u);
        pop(u);
    }

    void push (int idx) {
        int l = left(idx);
        int r = right(idx);
        if (r<size() && less_than(r, idx)) { // r is an option
            if (less_than(l, r)) {
                swap(l,idx);
                push(l);
            } else {
                swap(r,idx);
                push(r);
            }
        } else if (l<size() && less_than(l, idx)) {
            swap(l, idx);
            push(l);
        }
    }

    void eject () {
        if (!size()) return;
        delete cursors_[0];
        cursors_[0] = cursors_[cursors_.size()-1];
        cursors_.pop_back();
        push(0);
    }

    bool step () {
        if (cursors_[0]->Next()) {
            push(0);
        } else {
            eject();
        }
        return size()>0;
    }

public:
    // advances to the next op
    // @return non-empty
    bool Next() {
        Uuid id = op().id();
        while (step() && op().id()==id); // idempotency
        return size()>0;
    }
    // returns the data buffer for the current cursor/op
    const std::string& data() const { return op().data(); }
    //
    Status Merge(typename Frame::Builder& output) {
        if (empty()) return 0;
        do {
            output.AppendOp(*cursors_[0]);
        } while (Next());
        return 0;
    }
};
    
}

#endif
