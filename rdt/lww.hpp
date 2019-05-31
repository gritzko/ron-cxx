#ifndef rdt_lww_hpp
#define rdt_lww_hpp

#include <unordered_map>
#include "merge.hpp"

namespace ron {

template <class Frame>
class LastWriteWinsRDT {
    using Builder = typename Frame::Builder;
    using Cursor = typename Frame::Cursor;
    using Cursors = typename Frame::Cursors;

    static bool less_than(const Cursor &a, const Cursor &b) {
        return a.id() < b.id();
    }
    typedef MergeCursor<Frame, less_than> MCursor;

   public:
    Status Merge(typename Frame::Builder &output, Cursors &inputs) const {
        MCursor m{inputs};
        m.Merge(output);
        return Status::OK;
    }

    // TODO this impl will not match escaped keys, e.g. '\006bey' for 'key'.
    // Either way, the latest/winning value will go first.
    // May use Frame::unescape() and/or Op unesc flag.
    Status GC(Builder &output, const Frame &input) const {
        std::unordered_map<Slice, Uuid> last;
        auto scan = input.cursor();
        do {
            if (scan.op().size() < 3) continue;
            Slice key{input.data(), scan.atom(2).origin.as_range};
            last[key] = scan.id();
        } while (scan.Next());

        auto filter = input.cursor();
        if (filter.op().size() == 2) {  // header
            output.AppendOp(filter);
        }
        do {  // TODO maybe check op pattern here
            if (filter.op().size() < 3) continue;
            Slice key{input.data(), filter.atom(2).origin.as_range};
            if (last[key] == filter.id()) {
                output.AppendOp(filter);
            }
        } while (filter.Next());

        return Status::OK;
    }

    Status MergeGC(Builder &output, Cursors &inputs) const {
        Builder unclean;
        Status ok = Merge(unclean, inputs);
        if (!ok) return ok;
        Frame uc = unclean.Release();
        ok = GC(output, uc);
        return ok;
    }
};

}  // namespace ron

#endif
