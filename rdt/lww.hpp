#ifndef rdt_lww_hpp
#define rdt_lww_hpp

#include <unordered_map>
#include "rdt/merge.hpp"

namespace ron {

template <class Frame>
class LastWriteWinsRDT {
    static bool less_than(const Op &a, const Op &b) { return a.id() < b.id(); }
    typedef MergeCursor<Frame, less_than> merger;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef typename Frame::Cursors Cursors;

   public:
    Status Merge(typename Frame::Builder &output, Cursors &inputs) const {
        merger m{inputs};
        m.Merge(output);
        return Status::OK;
    }

    // TODO this impl will not match escaped keys, e.g. '\006bey' for 'key'.
    // Either way, the latest/winning value will go first.
    // May use Frame::unescape() and/or Op unesc flag.
    Status GC(Builder &output, const Frame &input) const {
        std::unordered_map<slice_t, Uuid> last;
        auto scan = input.cursor();
        do {
            if (scan.op().size() < 3) continue;
            slice_t key{input.data(), scan.op().atom(2).origin().range()};
            last[key] = scan.op().id();
        } while (scan.Next());

        auto filter = input.cursor();
        if (filter.op().size() == 2) {  // header
            output.AppendOp(filter);
        }
        do {  // TODO maybe check op pattern here
            if (filter.op().size() < 3) continue;
            slice_t key{input.data(), filter.op().atom(2).origin().range()};
            if (last[key] == filter.op().id()) {
                output.AppendOp(filter);
            }
        } while (filter.Next());

        return Status::OK;
    }

    Status MergeGC(Builder &output, Cursors &inputs) const {
        Builder unclean;
        Status ok = Merge(unclean, inputs);
        if (!ok) return ok;
        Frame uc = unclean.frame();
        ok = GC(output, uc);
        return ok;
    }
};

}  // namespace ron

#endif
