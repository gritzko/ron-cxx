#ifndef CPP_MAPPERS_HPP
#define CPP_MAPPERS_HPP

#include "../rdt/rdt.hpp"
#include "../ron/ron.hpp"
#include "const.hpp"
#include "dmp_diff.hpp"
#include "rocks_store.hpp"

namespace ron {

template <typename Commit>
class CSVMapper {
   public:
    using Frame = typename Commit::Frame;
    using Cursor = typename Frame::Cursor;
    using Builder = typename Frame::Builder;
    using mx_t = typename MatrixRDT<Frame>::mx_t;
    using Records = typename Commit::Records;

    CSVMapper() = default;

    Status Read(Builder& response, Cursor& query, Commit& branch);

    Status Write(Builder& response, Cursor& query, Commit& branch);
};

template <typename Commit>
class TxtMapper {
   public:
    using Frame = typename Commit::Frame;
    using Cursor = typename Frame::Cursor;
    using Builder = typename Frame::Builder;
    using Records = typename Commit::Records;
    using StringIterator = typename Frame::StringIterator;
    using DMP = MyersDiff<Codepoints>;
    using Ids = inc_stack<Uuid>;
    using Diff = typename DMP::Diff;
    using Diffs = typename DMP::Diffs;

    TxtMapper() = default;

    Status Read(Builder& response, Cursor& query, Commit& branch);
    Status ReadVersion(Builder& response, Cursor& query, Commit& branch);
    Status ReadPatch(Builder& response, Cursor& query, Commit& branch);

    Status WriteDiffs(const Ids& ids, const Diffs& diffs, Commit& branch);
    Status WriteState(Builder& response, Cursor& query, Commit& branch);
    Status WritePatch(Builder& response, Cursor& query, Commit& branch);
    Status Write(Builder& response, Cursor& query, Commit& branch);

    static Status RGA2Codepoints(Codepoints& text, Ids& ids, const Frame& frame,
                                 const Booleans& tombs);
};

}  // namespace ron

#endif  // CPP_MAPPERS_HPP
