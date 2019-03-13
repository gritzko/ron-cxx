#ifndef CPP_CHAIN_HPP
#define CPP_CHAIN_HPP

#include "merge.hpp"
#include "ron/status.hpp"
#include "ron/uuid.hpp"

namespace ron {

template <typename Frame>
class OpChain {
    static bool less_than(const Op &a, const Op &b) { return a.id() < b.id(); }

    using MCursor = MergeCursor<Frame, less_than>;
    using Builder = typename Frame::Builder;
    using Cursor = typename Frame::Cursor;
    using Cursors = typename Frame::Cursors;

   public:
    Status Merge(typename Frame::Builder &output, Cursors &inputs) const {
        MCursor m{inputs};
        m.Merge(output);
        return Status::OK;
    }

    Status GC(Builder &output, const Frame &input) const {
        Cursor cur = input.cursor();
        output.AppendAll(cur);
        return Status::OK;
    }

    Status MergeGC(Builder &output, Cursors &inputs) const {
        return Merge(output, inputs);
    }
};

};  // namespace ron

#endif  // CPP_CHAIN_HPP
