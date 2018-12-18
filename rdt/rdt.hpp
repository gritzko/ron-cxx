#ifndef rdt_rdt_hpp
#define rdt_rdt_hpp

#include "rdt/chain.hpp"
#include "rdt/lww.hpp"
#include "ron/op.hpp"
#include "ron/status.hpp"

namespace ron {

enum RDT : uint8_t { CHAIN = 0, ACK = 1, LWW = 2, RGA = 3 };

template <class Frame>
class MasterRDT {
    LastWriteWinsRDT<Frame> lww_;
    OpChain<Frame> chain_;

   public:
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef std::vector<Frame> Frames;
    typedef std::vector<Cursor> Cursors;

    MasterRDT() : lww_{} {}

    virtual Status Merge(Builder &output, RDT reducer,
                         const std::vector<Cursor> &inputs) const {
        switch (reducer) {
            case LWW:
                return lww_.Merge(output, inputs);
            case CHAIN:
                return chain_.Merge(output, inputs);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }

    virtual Status GC(Builder &output, RDT reducer, const Frame &input) const {
        switch (reducer) {
            case LWW:
                return lww_.GC(output, input);
            case CHAIN:
                return chain_.GC(output, input);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }

    virtual Status MergeGC(Builder &output, RDT reducer,
                           const std::vector<Cursor> &inputs) const {
        switch (reducer) {
            case LWW:
                return lww_.MergeGC(output, inputs);
            case CHAIN:
                return chain_.MergeGC(output, inputs);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }
};

}  // namespace ron

#endif
