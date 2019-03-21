#ifndef CPP_MAPPERS_HPP
#define CPP_MAPPERS_HPP

#include "../rdt/rdt.hpp"
#include "../ron/ron.hpp"
#include "const.hpp"
#include "rocks_store.hpp"

namespace ron {

template <class Frame>
class Replica;

template <typename Frame>
struct CSVMapper {
    using Cursor = typename Frame::Cursor;
    using Builder = typename Frame::Builder;
    using HostReplica = Replica<Frame>;
    using mx_t = typename MatrixRDT<Frame>::mx_t;
    using Store = RocksDBStore<Frame>;
    using Records = typename Store::Records;

    Store store_;

    explicit CSVMapper(RocksDBStore<Frame> store) : store_{store} {}

    Status Read(Builder& response, Cursor& query, Uuid branch = Uuid::NIL);

    Status Write(Records& save, Cursor& query, Uuid branch = Uuid::NIL);
};

template <typename Frame>
struct TxtMapper {
    using Cursor = typename Frame::Cursor;
    using Builder = typename Frame::Builder;
    using HostReplica = Replica<Frame>;
    using Store = RocksDBStore<Frame>;
    using Records = typename Store::Records;

    Store store_;

    explicit TxtMapper(RocksDBStore<Frame> store) : store_{store} {}

    Status Read(Builder& response, Cursor& query, Uuid branch = Uuid::NIL);

    Status Write(Records& save, Cursor& query, Uuid branch = Uuid::NIL);
};

}  // namespace ron

#endif  // CPP_MAPPERS_HPP
