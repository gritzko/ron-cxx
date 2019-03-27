#ifndef RON_RDT_LOG_HPP
#define RON_RDT_LOG_HPP

#include "../ron/status.hpp"
#include "../ron/uuid.hpp"
#include "merge.hpp"

namespace ron {

template <typename Frame>
class OpLog {
    using Builder = typename Frame::Builder;
    using Cursor = typename Frame::Cursor;
    using Cursors = typename Frame::Cursors;

   public:
    Status Merge(typename Frame::Builder &output, Cursors &inputs) const {
        for (int i = 0; i < inputs.size(); ++i) output.AppendAll(inputs[i]);
        return Status::OK;
    }

    Status GC(Builder &output, const Frame &input) const { return Status::OK; }

    Status MergeGC(Builder &output, Cursors &inputs) const {
        return Merge(output, inputs);
    }
};

};  // namespace ron

#endif  // CPP_CHAIN_HPP
