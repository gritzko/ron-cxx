#ifndef rdt_lww_hpp
#define rdt_lww_hpp

#include <unordered_set>
#include "rdt/merge.hpp"
#include "rdt/rdt.hpp"
#include "ron/status.hpp"

namespace ron {

template<class Frame>
class LastWriteWins : public ReplicatedDataType<Frame> {
    static int less_than (const Op& a, const Op& b) {
        return a.id() < b.id();
    }
    typedef MergeCursor<Frame, less_than> merger;
    typedef typename Frame::Builder Builder;
    typedef std::vector<Frame> Frames;
public:

    Status Merge(typename Frame::Builder& output, const Frames& inputs) {
        merger m{inputs};
        m.Merge(output);
        return Status::OK;
    }

    // TODO this impl will not match escaped keys, e.g. '\006bey' for 'key'.
    // Either way, the latest/winning value will go first.
    // May use Frame::unescape() and/or Op unesc flag.
    Status GC(Builder& output, const Frame& input) {
        std::unordered_set<slice_t> seen;
        auto cur = input.cursor();
        do {
            slice_t key{input.data(), cur.op().value_range(2)};
            if (seen.find(key)==seen.end()) {
                seen.insert(key);
                output.AddOp(cur.op(), input.data());
            }
        } while (cur.Next());
        return Status::OK;
    }
};

}

#endif