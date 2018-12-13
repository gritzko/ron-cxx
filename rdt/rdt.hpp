#ifndef rdt_rdt_hpp
#define rdt_rdt_hpp

#include "rdt/lww.hpp"
#include "ron/op.hpp"
#include "ron/status.hpp"

namespace ron {

enum RDT : uint8_t { ACK = 0, CHAIN = 1, LWW = 2, RGA = 3 };

template <class Frame>
class MasterRDT {
    LastWriteWinsRDT<Frame> lww_;

   public:
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef std::vector<Frame> Frames;
    typedef std::vector<Cursor> Cursors;

    MasterRDT() : lww_{} {}

    virtual Status Merge(Builder &output, RDT reducer,
                         const std::vector<Cursor> &inputs) {
        switch (reducer) {
            case LWW:
                return lww_.Merge(output, inputs);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }

    virtual Status GC(Builder &output, RDT reducer, const Frame &input) {
        switch (reducer) {
            case LWW:
                return lww_.GC(output, input);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }

    virtual Status MergeGC(Builder &output, RDT reducer, const Frame &input) {
        switch (reducer) {
            case LWW:
                return lww_.MergeGC(output, input);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }
};

}  // namespace ron

#endif
