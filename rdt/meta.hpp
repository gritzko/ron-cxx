#ifndef rdt_meta_hpp
#define rdt_meta_hpp

#include <unordered_map>
#include "../ron/status.hpp"
#include "merge.hpp"

namespace ron {

// FIXME(gritzko) do we need to merge meta records at all?
// maybe to update span lengths? chain-based records are likely more efficient
template <class Frame>
class MetaRDT {
    using Builder = typename Frame::Builder;
    using Cursor = typename Frame::Cursor;
    using Cursors = typename Frame::Cursors;

    static bool less_than(const Cursor &a, const Cursor &b) {
        return a.id() == b.id() ? a.ref() > b.ref() : a.id() < b.id();
    }
    typedef MergeCursor<Frame, less_than> MCursor;

   public:
    Status Merge(typename Frame::Builder &output, Cursors &inputs) const {
        MCursor m{inputs};
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
