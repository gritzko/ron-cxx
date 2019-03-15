#ifndef RON_RDT_HPP
#define RON_RDT_HPP

#include "../ron/op.hpp"
#include "../ron/status.hpp"
#include "chain.hpp"
#include "const.hpp"
#include "lww.hpp"
#include "meta.hpp"
#include "mx.hpp"
#include "rga.hpp"

namespace ron {

template <class Frame>
class MasterRDT {
    LastWriteWinsRDT<Frame> lww_;
    OpChain<Frame> chain_;
    MetaRDT<Frame> meta_;
    MatrixRDT<Frame> mx_;
    RGArrayRDT<Frame> rga_;

   public:
    using Builder = typename Frame::Builder;
    using Cursor = typename Frame::Cursor;
    using Frames = std::vector<Frame>;
    using Cursors = std::vector<Cursor>;

    MasterRDT() = default;

    /**
     * @param{inputs} cursors to consume */
    Status Merge(Builder &output, RDT reducer, Cursors &inputs) const {
        switch (reducer) {
            case CHAIN_RDT:
                return chain_.Merge(output, inputs);
            case META_RDT:
                return meta_.Merge(output, inputs);
            case LWW_RDT:
                return lww_.Merge(output, inputs);
            case MX_RDT:
                return mx_.Merge(output, inputs);
            case RGA_RDT:
                return rga_.Merge(output, inputs);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }

    Status GC(Builder &output, RDT reducer, const Frame &input) const {
        switch (reducer) {
            case CHAIN_RDT:
                return chain_.GC(output, input);
            case META_RDT:
                return meta_.GC(output, input);
            case LWW_RDT:
                return lww_.GC(output, input);
            case MX_RDT:
                return mx_.GC(output, input);
            case RGA_RDT:
                return rga_.GC(output, input);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }

    Status MergeGC(Builder &output, RDT reducer, Cursors &inputs) const {
        switch (reducer) {
            case CHAIN_RDT:
                return chain_.MergeGC(output, inputs);
            case META_RDT:
                return meta_.MergeGC(output, inputs);
            case LWW_RDT:
                return lww_.MergeGC(output, inputs);
            case MX_RDT:
                return mx_.MergeGC(output, inputs);
            case RGA_RDT:
                return rga_.MergeGC(output, inputs);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }
};

template <typename Frame>
Status MergeCursors(Frame &ret, RDT rdt, typename Frame::Cursors &inputs) {
    using Reducer = MasterRDT<Frame>;
    Reducer reducer;
    using Cursor = typename Reducer::Cursor;
    typename Reducer::Builder builder;
    Status ok = reducer.Merge(builder, rdt, inputs);
    ret = builder.frame();
    return ok;
}

template <typename Frame>
Status SplitIntoChains(const Frame input, typename Frame::Cursors &chains) {
    using Cursor = typename Frame::Cursor;
    Cursor cur = input.cursor();
    Cursor nxt = cur;
    Status ok;
    while ((ok = cur.SkipChain())) {
        nxt.Trim(cur);
        chains.push_back(nxt);
        nxt = cur;
    }
    if (ok != Status::ENDOFFRAME) return ok;
    chains.push_back(nxt);
    return Status::OK;
}

template <typename Frame>
Status ObjectLog2State(Frame &ret, RDT rdt, const Frame input) {
    using Cursor = typename Frame::Cursor;
    using Cursors = typename Frame::Cursors;
    Cursors chains;
    Status ok = SplitIntoChains<Frame>(input, chains);
    if (!ok) return ok;
    return MergeCursors(ret, rdt, chains);
}

}  // namespace ron

#endif
