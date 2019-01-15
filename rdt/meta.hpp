#ifndef rdt_meta_hpp
#define rdt_meta_hpp

#include <unordered_map>
#include "rdt/merge.hpp"
#include "ron/status.hpp"

namespace ron {

template <class Frame>
class MetaRDT {
    static bool less_than(const Op &a, const Op &b) {
        return a.id() == b.id() ? a.ref() > b.ref() : a.id() < b.id();
    }
    typedef MergeCursor<Frame, less_than> MergeCursor;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef typename Frame::Cursors Cursors;

   public:
    Status Merge(typename Frame::Builder &output, Cursors &inputs) const {
        MergeCursor m{inputs};
        m.Merge(output);
        return Status::OK;
    }

    Status GC(Builder &output, const Frame &input) const {
        Cursor c = input.cursor();
        while (c.valid()) {
            output.AppendOp(c);
            c.Next();
        }
        return Status::OK;
    }

    Status MergeGC(Builder &output, Cursors &inputs) const {
        return Merge(output, inputs);
    }
};

}  // namespace ron

#endif
