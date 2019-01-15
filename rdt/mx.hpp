#ifndef rdt_mx_hpp
#define rdt_mx_hpp

#include <bits/unordered_map.h>
#include <unordered_map>
#include "rdt/merge.hpp"
#include "ron/status.hpp"

namespace ron {

typedef std::pair<fsize_t, fsize_t> mxidx_t;

const mxidx_t MX_IDX_MAX{FSIZE_MAX, FSIZE_MAX};

}  // namespace ron

namespace std {

template <>
struct hash<ron::mxidx_t> {
    size_t operator()(const ron::mxidx_t &idx) const noexcept {
        return hash<int>()(idx.first) ^ hash<int>()(idx.second);
    }
};
}  // namespace std

namespace ron {

template <typename Cursor>
mxidx_t readmxidx(const Cursor &read) {
    if (read.size() != 2 + 3 || !read.has(2, INT) || !read.has(3, INT))
        return MX_IDX_MAX;
    return mxidx_t{(fsize_t)read.integer(2), (fsize_t)read.integer(3)};
}

template <class Frame>
class MatrixRDT {
    static bool less_than(const Op &a, const Op &b) { return a.id() < b.id(); }

    typedef MergeCursor<Frame, less_than> MCursor;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef typename Frame::Cursors Cursors;

   public:
    typedef std::unordered_map<mxidx_t, Atom> mx_t;

    Status Merge(typename Frame::Builder &output, Cursors &inputs) const {
        MCursor m{inputs};
        m.Merge(output);
        return Status::OK;
    }

    Status GC(Builder &output, const Frame &input) const {
        mx_t max{};
        for (Cursor read = input.cursor(); read.valid(); read.Next()) {
            mxidx_t at{read};
            if (at == MX_IDX_MAX) continue;
            max[at] = read.id();
        }
        for (Cursor write = input.cursor(); write.valid(); write.Next()) {
            mxidx_t at{write};
            if (at != MX_IDX_MAX && max[at] == write.id())
                output.AppendOp(write);
        }
        return Status::OK;
    }

    Status MergeGC(Builder &output, Cursors &inputs) const {
        return Status::NOT_IMPLEMENTED;
    }
};

}  // namespace ron

#endif