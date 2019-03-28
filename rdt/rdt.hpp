#ifndef RON_RDT_HPP
#define RON_RDT_HPP

#include "../ron/op.hpp"
#include "../ron/status.hpp"
#include "log.hpp"
#include "lww.hpp"
#include "meta.hpp"
#include "mx.hpp"
#include "rga.hpp"

namespace ron {

template <class Frame>
class MasterRDT {
    LastWriteWinsRDT<Frame> lww_;
    OpLog<Frame> log_;
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
    Status Merge(Builder &output, FORM form, Cursors &inputs) const {
        switch (form) {
            case LOG_RAW_FORM:
                return log_.Merge(output, inputs);
            case ZERO_RAW_FORM:
            case META_META_FORM:
                return meta_.Merge(output, inputs);
            case YARN_RAW_FORM:
            case LWW_RDT_FORM:
                return lww_.Merge(output, inputs);
            case MX_RDT_FORM:
                return mx_.Merge(output, inputs);
            case RGA_RDT_FORM:
                return rga_.Merge(output, inputs);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }

    Status GC(Builder &output, FORM form, const Frame &input) const {
        switch (form) {
            case LOG_RAW_FORM:
                return log_.GC(output, input);
            case ZERO_RAW_FORM:
            case META_META_FORM:
                return meta_.GC(output, input);
            case YARN_RAW_FORM:
            case LWW_RDT_FORM:
                return lww_.GC(output, input);
            case MX_RDT_FORM:
                return mx_.GC(output, input);
            case RGA_RDT_FORM:
                return rga_.GC(output, input);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }

    Status MergeGC(Builder &output, FORM form, Cursors &inputs) const {
        switch (form) {
            case LOG_RAW_FORM:
                return log_.MergeGC(output, inputs);
            case ZERO_RAW_FORM:
            case META_META_FORM:
                return meta_.MergeGC(output, inputs);
            case YARN_RAW_FORM:
            case LWW_RDT_FORM:
                return lww_.MergeGC(output, inputs);
            case MX_RDT_FORM:
                return mx_.MergeGC(output, inputs);
            case RGA_RDT_FORM:
                return rga_.MergeGC(output, inputs);
            default:
                return Status::NOT_IMPLEMENTED;
        }
    }
};

template <typename Frame>
Status MergeCursorsInto(typename Frame::Builder &into, FORM rdt,
                        typename Frame::Cursors &inputs) {
    using Reducer = MasterRDT<Frame>;
    Reducer reducer;
    return reducer.Merge(into, rdt, inputs);
}

template <class Frame>
Status GetForm(FORM &ret, const typename Frame::Cursors &inputs) {
    Uuid form{};
    for (auto &i : inputs) {
        if (!i.valid() || i.ref().version() != NAME) continue;
        if (form.zero()) {
            form = i.ref();
        } else if (form != i.ref()) {
            return Status::BADARGS.comment("merge fails: form id mismatch");
        }
    }
    ret = uuid2form(form);  // if nothing found, 0-form, merge==concatenation
    return Status::OK;
}

template <typename Frame>
Status MergeCursors(Frame &ret, FORM rdt, typename Frame::Cursors &inputs) {
    using Builder = typename Frame::Builder;
    Builder builder;
    Status ok = MergeCursorsInto<Frame>(builder, rdt, inputs);
    if (ok) {
        builder.Release(ret);
    }
    return ok;
}

template <typename Frame>
Status MergeCursors(Frame &ret, typename Frame::Cursors &inputs) {
    FORM form;
    Status ok = GetForm(form, inputs);
    if (!ok) {
        return ok;
    }
    return MergeCursors(ret, form, inputs);
}

template <typename Frame>
Status MergeStrings(String &ret, FORM rdt, Strings inputs) {
    using Cursors = typename Frame::Cursors;
    Frame frame;
    Cursors cursors;
    for (int i = 0; i < inputs.size(); ++i) cursors.emplace_back(inputs[i]);
    Status ok = MergeCursors(frame, rdt, cursors);
    if (!ok) {
        return ok;
    }
    std::swap(frame, ret);
    return Status::OK;
}

template <typename Frame>
Status MergeFrames(Frame &ret, std::vector<Frame> inputs) {
    using Cursors = typename Frame::Cursors;
    Cursors cursors;
    for (int i = 0; i < inputs.size(); ++i) cursors.emplace_back(inputs[i]);
    FORM rdt;
    Status ok = GetForm<Frame>(rdt, cursors);
    if (!ok) {
        return ok;
    }
    ok = MergeCursors(ret, rdt, cursors);
    if (!ok) {
        return ok;
    }
    return Status::OK;
}

template <typename Frame>
Status SplitFrame(const Frame input, typename Frame::Cursors &chains) {
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
Status ObjectLogIntoState(typename Frame::Builder &into, const Frame input) {
    using Cursors = typename Frame::Cursors;
    Cursors chains;
    Status ok = SplitFrame<Frame>(input, chains);
    if (!ok) return ok;
    if (chains.empty()) return Status::OK;
    Uuid rdt_id = chains[0].ref();
    FORM rdt = uuid2form(rdt_id);
    return MergeCursorsInto<Frame>(into, rdt, chains);
}

template <typename Frame>
Status ObjectLogToState(Frame &ret, const Frame input) {
    using Builder = typename Frame::Builder;
    Builder b;
    Status ok = ObjectLogIntoState<Frame>(b, input);
    if (!ok) {
        return ok;
    }
    ret = b.Release();
    return Status::OK;
}

}  // namespace ron

#endif
