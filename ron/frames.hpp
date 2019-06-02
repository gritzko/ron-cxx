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
    if (a.op().size() != b.op().size()) return Status::BADVALUE;
    for (int i = 2; i < a.op().size(); i++) {
        Atom ai = a.atom(i), bi = b.atom(i);
        if (ai.type() != bi.type())
            return Status::BADVALUE.comment("value type mismatch");
        switch (ai.type()) {
            case INT:
                if (ai.value.as_integer != bi.value.as_integer)
                    return Status::BADVALUE.comment("different int");
                break;
            case FLOAT:
                if (ai.value.as_float != bi.value.as_float)
                    return Status::BADVALUE.comment("different float");
                break;
            case UUID:
                if (ai != bi) return Status::BADVALUE.comment("different UUID");
                break;
            case STRING:
                String as, bs;
                ReadString(as, a, i);
                ReadString(bs, b, i);
                if (as != bs)
                    return Status::BADVALUE.comment("different string");
                break;
        }
    }
    return Status::OK;
}

template <typename Cursor>
Status CompareWithCursors(Cursor a, Cursor b) {
    while (a.valid() && b.valid()) {
        while (a.valid() && a.id() == Uuid::COMMENT) a.Next();
        while (b.valid() && b.id() == Uuid::COMMENT) b.Next();
        if (!a.valid() || !b.valid()) break;
        Status ok = CompareOps<Cursor>(a, b);
        if (!ok) {
            return ok;
        }
        a.Next();
        b.Next();
    }
    while (a.valid() && a.id() == Uuid::COMMENT) {
        a.Next();
    }
    while (b.valid() && b.id() == Uuid::COMMENT) {
        b.Next();
    }
    if (a.valid() || b.valid())
        return Status::BADFRAME.comment("one frame is longer");
    return Status::OK;
}

template <typename Frame>
Status CompareFrames(const Frame& frame_a, const Frame& frame_b) {
    using Cursor = typename Frame::Cursor;
    Cursor a = frame_a.cursor();
    Cursor b = frame_b.cursor();
    if (a.Next() != b.Next()) {
        return Status::BADVALUE;
    }
    return CompareWithCursors(a, b);
}

template <typename Frame, class... Ts>
Frame OneOp(Uuid id, Uuid ref, Ts... args) {
    using Builder = typename Frame::Builder;
    Builder b;
    b.AppendOp(Op{id, ref, args...});
    return b.Release();
}

template <typename Frame, class... Ts>
Frame Query(Uuid id, Uuid ref, Ts... args) {
    using Builder = typename Frame::Builder;
    Builder b;
    b.AppendOp(Op{id, ref, args...});
    b.EndChunk(QUERY);
    return b.Release();
}

template <class Frame>
Status Reserialize(std::vector<Frame>& into,
                   std::vector<typename Frame::Cursor>& what) {
    into.clear();
    for (auto& c : what) {
        typename Frame::Builder b;
        Status ok;
        do {
            b.AppendOp(c);
        } while ((ok = c.Next()));
        if (ok != Status::ENDOFFRAME) {
            return ok;
        }
        into.emplace_back(b.Release());
    }
    return Status::OK;
}

/** Reads a String from an op using the Atom as a cursor.
 * By a convention, the atom cursor for a String must be in the BTB state.
 * For a char (a single codepoint), the atom cursor must point at the char. */
template <class Cursor>
Result ReadString(String& to, const Cursor& c, Atom a) {
    assert(a.type() == STRING);
    if (a.value.cp != 0) {
        utf8append(to, a.value.cp);
        return OK;
    }
    Result ret{OK};
    while (OK == (ret = c.NextCodepoint(a))) {
        utf8append(to, a.value.cp);
    }
    return ret == ENDOFINPUT ? OK : ret;
}

template <class Cursor>
Result ReadString16(String16& to, const Cursor& c, Atom a) {
    assert(a.type() == STRING);
    Result ret{OK};
    while (OK == (ret = c.NextCodepoint(a))) {
        utf16append(to, a.value.cp);
    }
    return ret == ENDOFINPUT ? OK : ret;
}

template <class Cursor>
inline Result ReadString(String& to, const Cursor& c, fsize_t idx) {
    return ReadString(to, c, c.atom(idx));
}

/** A happy-path shortcut for ReadString() */
template <class Cursor>
inline String GetString(const Cursor& c, fsize_t idx = 2) {
    String ret;
    ReadString(ret, c, c.atom(idx));
    return ret;
}

template <class Cursor>
inline Codepoint GetChar(const Cursor& c, fsize_t idx = 2) {
    return c.atom(idx).value.cp;
}

template <class Cursor>
inline bool HasValue(const Cursor& cur, ATOM atype, fsize_t idx = 2) {
    const Atoms& op = cur.op();
    return op.size() > idx && op[idx].type() == atype;
}

template <class Cursor>
Status SkipChain(Cursor& cur) {
    Uuid i;
    Status ok;
    do {
        i = cur.id();
        ok = cur.Next();
    } while (ok && cur.ref() == i);
    return ok;
}

/** A convenience method to add all ops from the cursor.
    When passed as a parameter, a cursor must be positioned on the first
    relevant entry (not BTB). */
template <class Frame>
void AppendAll(typename Frame::Builder& to, typename Frame::Cursor& cur) {
    while (cur.valid()) {
        to.AppendOp(cur);
        if (cur.term() != REDUCED) {
            to.EndChunk(cur.term());
        }
        cur.Next();
    }
}

/** A convenience method to add all ops from the frame. */
template <typename Frame>
void AppendFrame(typename Frame::Builder& to, const Frame& frame) {
    auto cur = frame.cursor();
    if (cur.Next()) {
        AppendAll<Frame>(to, cur);
    }
}

}  // namespace ron
#endif
