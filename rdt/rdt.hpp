#ifndef rdt_rdt_hpp
#define rdt_rdt_hpp

#include "ron/op.hpp"
#include "ron/status.hpp"

namespace ron {

template<class Frame>
class ReplicatedDataType {

public:

    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef std::vector<Frame> Frames;
    typedef std::vector<Cursor> Cursors;

    virtual Status Merge(Builder& output, const std::vector<Cursor>& inputs) = 0;

    virtual Status GC(Builder& output, const Frame& input) = 0;

    virtual Status MergeGC(Builder& output, const Cursors& inputs) {
        Builder unclean;
        Status ok = Merge(unclean, inputs);
        if (!ok) return ok;
        Frame uc = unclean.frame();
        ok = GC(output, uc);
        return ok;
    }

};

}

#endif
