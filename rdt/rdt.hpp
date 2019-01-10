#ifndef rdt_rdt_hpp
#define rdt_rdt_hpp

#include "rdt/chain.hpp"
#include "rdt/const.hpp"
#include "rdt/lww.hpp"
#include "rdt/meta.hpp"
#include "rdt/mx.hpp"
#include "ron/op.hpp"
#include "ron/status.hpp"

namespace ron {

template <class Frame>
class MasterRDT {
    LastWriteWinsRDT<Frame> lww_;
    OpChain<Frame> chain_;
    MetaRDT<Frame> meta_;
    MatrixRDT<Frame> mx_;

   public:
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef std::vector<Frame> Frames;
    typedef std::vector<Cursor> Cursors;

    MasterRDT() : lww_{}, chain_{}, meta_{}, mx_{} {}

    Status Merge(Builder &output, RDT reducer,
                 const std::vector<Cursor> &inputs) const {
        switch (reducer) {
            case CHAIN_RDT:
                return chain_.Merge(output, inputs);
            case META_RDT:
                return meta_.Merge(output, inputs);
            case LWW_RDT:
                return lww_.Merge(output, inputs);
            case MX_RDT:
                return mx_.Merge(output, inputs);
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
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }

    Status MergeGC(Builder &output, RDT reducer,
                   const std::vector<Cursor> &inputs) const {
        switch (reducer) {
            case CHAIN_RDT:
                return chain_.MergeGC(output, inputs);
            case META_RDT:
                return meta_.MergeGC(output, inputs);
            case LWW_RDT:
                return lww_.MergeGC(output, inputs);
            case MX_RDT:
                return mx_.MergeGC(output, inputs);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }
};

template <typename Frame>
std::string Merge(RDT rdt, const typename Frame::Cursors &inputs) {
    typedef MasterRDT<Frame> Reducer;
    Reducer reducer;
    typedef typename Reducer::Cursor Cursor;
    typename Reducer::Builder builder;
    reducer.Merge(builder, rdt, inputs);
    return builder.data();
}

}  // namespace ron

#endif
