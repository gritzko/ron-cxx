#ifndef CPP_CHAIN_HPP
#define CPP_CHAIN_HPP

#include "rdt/merge.hpp"
#include "ron/status.hpp"
#include "ron/uuid.hpp"

namespace ron {

template <typename Frame>
class OpChain {
    static bool less_than(const Op &a, const Op &b) { return a.id() < b.id(); }

    typedef MergeCursor<Frame, less_than> merger;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;

   public:
    Status Merge(typename Frame::Builder &output,
                 const std::vector<Cursor> &inputs) const {
        merger m{inputs};
        m.Merge(output);
        return Status::OK;
    }

    Status GC(Builder &output, const Frame &input) const {
        Cursor cur = input.cursor();
        output.AppendAll(cur);
        return Status::OK;
    }

    virtual Status MergeGC(Builder &output,
                           const typename Frame::Cursors &inputs) const {
        return Merge(output, inputs);
    }
};

};  // namespace ron

#endif  // CPP_CHAIN_HPP
