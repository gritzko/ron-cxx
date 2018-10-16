#ifndef rdt_lww_hpp
#define rdt_lww_hpp

#include <unordered_map>
#include "rdt/merge.hpp"
#include "rdt/rdt.hpp"
#include "ron/status.hpp"

namespace ron {

template<class Frame>
class LastWriteWins : public ReplicatedDataType<Frame> {
    static bool less_than (const Op& a, const Op& b) {
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
        std::unordered_map<slice_t, Uuid> last;
        auto scan = input.cursor();
        do {
            if (scan.op().size()<3) continue;
            slice_t key{input.data(), scan.op().value_range(2)};
            last[key] = scan.op().id();
        } while (scan.Next());

        auto filter = input.cursor();
        if (filter.op().size()==2) { // header
            output.AddOp(filter.op(), input.data());
        }
        do { // TODO maybe check op pattern here
            if (filter.op().size()<3) continue;
            slice_t key{input.data(), filter.op().value_range(2)};
            if (last[key]==filter.op().id()) {
                output.AddOp(filter.op(), input.data());
            }
        } while (filter.Next());

        return Status::OK;
    }
};

}

#endif