#ifndef ron_frames_hpp
#define ron_frames_hpp

#include <cstring>
#include "uuid.hpp"

namespace ron {

struct stringbuf {
    std::string buf;
    operator const std::string& () const {
        return buf;
    }
    Value Add(const char* str, fsize_t len) {
        auto from = (fsize_t)buf.size();
        buf.append(str);
        return Value::String(frange_t{from, len});
    }
    inline Value Add(const char* str) {
        return Add(str, (fsize_t)strlen(str));
    }
    Value Add(const std::string& str) {
        return Add(str.data(), (fsize_t)str.size());
    }
};

// splits a frame into chunks, possibly recodes, checks for RON invariants
template<typename Frame1, typename Frame2>
Status Split(std::vector <Frame2> &chunks, const Frame1 &source);

template<typename Cursor>
std::string string_atom(const Cursor &cursor, fsize_t pos) {
    return cursor.op().value(pos).string_value(cursor.data());
}

template<typename Builder, typename Cursor>
void AddOp(Builder &builder, const Cursor &cursor) {
    builder.AddOp(cursor.op(), cursor.data());
};

const std::string EMPTY_STRING = "";

template<typename Builder>
void AddOp(Builder &builder, const Uuid& id, const Uuid& ref) {
    Op op{id, ref};
    builder.AddOp(op, EMPTY_STRING);
}

template<typename Builder>
void AddOp(Builder &builder, const char* id, const char* ref) {
    AddOp(builder, Uuid{id}, Uuid{ref});
}

template<typename Builder>
void AddOp(Builder &builder, const Uuid& id, const Uuid& ref, const char *val0, const char *val1) {
    stringbuf buf;  // we only optimize Add(cursor); here we do a lot of allocs
    Op op{id, ref, buf.Add(val0), buf.Add(val1)};
    builder.AddOp(op, buf);
}

template<typename Builder>
inline void AddOp(Builder &builder, const char *id, const char *ref, const char *val0, const char *val1) {
    AddOp(builder, Uuid{id}, Uuid{ref}, val0, val1);
}

template<typename Cursor>
inline std::string GetString (const Cursor& cur, fsize_t idx) {
    const Atom& cell = cur.op().value(idx);
    if (cell.variant()!=VARIANT::RON_ATOM) {
        return "";
    }
    auto& atom = (const Value&) cell;
    if (atom.type()!=ATOM::STRING) {
        return "";
    }
    return atom.string_value(cur.data());
}

}
#endif
