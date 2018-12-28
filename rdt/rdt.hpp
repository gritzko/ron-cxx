#ifndef rdt_rdt_hpp
#define rdt_rdt_hpp

#include "rdt/chain.hpp"
#include "rdt/meta.hpp"
#include "rdt/lww.hpp"
#include "ron/op.hpp"
#include "ron/status.hpp"

namespace ron {

const Uuid CHAIN_UUID{715112314629521408UL, 0};
const Uuid ACK_UUID{677716977128570880UL, 0};
const Uuid META_UUID{894494834235015168UL, 0};
const Uuid LWW_UUID{881557636825219072UL, 0};
const Uuid RGA_UUID{985043671231496192UL, 0};

enum RDT : uint8_t { META, CHAIN, ACK, LWW, RGA, RDT_COUNT };  // ??!!
const Uuid RDT_UUIDS[] = {META_UUID, CHAIN_UUID, ACK_UUID, LWW_UUID, RGA_UUID};

inline RDT uuid2rdt(const Uuid &rdt_id) {
    for (int i = 0; i < RDT_COUNT; i++)
        if (rdt_id == RDT_UUIDS[i]) return (RDT)i;
    return RDT_COUNT;
}

inline Uuid rdt2uuid(RDT rdt) {
    assert(rdt < RDT_COUNT);
    return RDT_UUIDS[rdt];
}

template <class Frame>
class MasterRDT {
    LastWriteWinsRDT<Frame> lww_;
    OpChain<Frame> chain_;
    MetaRDT<Frame> meta_;

   public:
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef std::vector<Frame> Frames;
    typedef std::vector<Cursor> Cursors;

    MasterRDT() : lww_{}, chain_{}, meta_{} {}

    virtual Status Merge(Builder &output, RDT reducer,
                         const std::vector<Cursor> &inputs) const {
        switch (reducer) {
            case CHAIN:
                return chain_.Merge(output, inputs);
            case META:
                return meta_.Merge(output, inputs);
            case LWW:
                return lww_.Merge(output, inputs);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }

    virtual Status GC(Builder &output, RDT reducer, const Frame &input) const {
        switch (reducer) {
            case CHAIN:
                return chain_.GC(output, input);
            case META:
                return meta_.GC(output, input);
            case LWW:
                return lww_.GC(output, input);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }

    virtual Status MergeGC(Builder &output, RDT reducer,
                           const std::vector<Cursor> &inputs) const {
        switch (reducer) {
            case CHAIN:
                return chain_.MergeGC(output, inputs);
            case META:
                return meta_.MergeGC(output, inputs);
            case LWW:
                return lww_.MergeGC(output, inputs);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }
};

template<typename Frame>
std::string Merge ( RDT rdt, const typename Frame::Cursors& inputs) {
    typedef MasterRDT<Frame> Reducer;
    Reducer reducer;
    typedef typename Reducer::Cursor Cursor;
    typename Reducer::Builder builder;
    reducer.Merge(builder, rdt, inputs);
    return builder.data();
}

}  // namespace ron

#endif
