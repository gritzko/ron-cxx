#ifndef ron_frames_hpp
#define ron_frames_hpp

#include <cstring>
#include <vector>
#include "op.hpp"
#include "status.hpp"
#include "uuid.hpp"

namespace ron {

template <typename Frame>
std::vector<typename Frame::Cursor> cursors(const std::vector<Frame>& frames) {
    std::vector<typename Frame::Cursor> ret;
    for (int i = 0; i < frames.size(); i++) ret.push_back(frames[i].cursor());
    return ret;
}

template <typename Cursor>
Status CompareOps(const Cursor& a, const Cursor& b) {
    if (a.id() != b.id()) return Status::BADID;
    if (a.ref() != b.ref()) return Status::BADREF;
    if (a.size() != b.size()) return Status::BADVALUE;
    for (int i = 2; i < a.size(); i++) {
        if (a.type(i) != b.type(i))
            return Status::BADVALUE.comment("value type mismatch");
        switch (a.type(i)) {
            case INT:
                if (a.integer(i) != b.integer(i))
                    return Status::BADVALUE.comment("different int");
                break;
            case FLOAT:
                if (a.number(i) != b.number(i))
                    return Status::BADVALUE.comment("different float");
                break;
            case UUID:
                if (a.uuid(i) != b.uuid(i))
                    return Status::BADVALUE.comment("different UUID");
                break;
            case STRING:
                if (a.string(i) != b.string(i))
                    return Status::BADVALUE.comment("different string");
                break;
        }
    }
    return Status::OK;
}

template <typename Cursor>
Status CompareWithCursors(Cursor a, Cursor b) {
    while (a.valid() && b.valid()) {
        while (a.valid() && a.id() == COMMENT_UUID) a.Next();
        while (b.valid() && b.id() == COMMENT_UUID) b.Next();
        if (!a.valid() || !b.valid()) break;
        Status ok = CompareOps<Cursor>(a, b);
        if (!ok) {
            return ok;
        }
        a.Next();
        b.Next();
    }
    while (a.valid() && a.id() == COMMENT_UUID) a.Next();
    while (b.valid() && b.id() == COMMENT_UUID) b.Next();
    if (a.valid() || b.valid())
        return Status::BADFRAME.comment("one frame is longer");
    return Status::OK;
}

template <typename Frame>
Status CompareFrames(const Frame& frame_a, const Frame& frame_b) {
    using Cursor = typename Frame::Cursor;
    Cursor a = frame_a.cursor();
    Cursor b = frame_b.cursor();
    return CompareWithCursors(a, b);
}

template <typename Frame>
Frame OneOp(Uuid id, Uuid ref) {
    using Builder = typename Frame::Builder;
    Builder b;
    b.AppendNewOp(id, ref);
    return b.Release();
}

}  // namespace ron
#endif
