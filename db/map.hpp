#ifndef CPP_MAPPERS_HPP
#define CPP_MAPPERS_HPP

#include "../rdt/rdt.hpp"
#include "../ron/ron.hpp"
#include "const.hpp"
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

    TxtMapper() = default;

    Status Read(Builder& response, Cursor& query, Commit& branch);

    Status Write(Builder& response, Cursor& query, Commit& branch);
};

}  // namespace ron

#endif  // CPP_MAPPERS_HPP
