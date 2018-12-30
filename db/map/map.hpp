#ifndef CPP_MAPPERS_HPP
#define CPP_MAPPERS_HPP

#include "db/replica.hpp"

namespace ron {


    template<typename Frame>
    struct ChainMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;

        HostReplica* host_;

        ChainMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query) const;

        Status Write(rocksdb::WriteBatch& batch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    static const Uuid CHAIN_MAPPER_ID{715112314629521408UL,0};

    template<typename Frame>
    struct OpMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;

        HostReplica* host_;

        OpMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query) const;

        Status Write(rocksdb::WriteBatch& batch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    static const Uuid OP_MAPPER_ID{933371022772535296UL,0};

    template<typename Frame>
    struct MasterMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;

        ChainMapper<Frame> chain_;
        OpMapper<Frame> op_;

        MasterMapper(HostReplica* host) : chain_{host}, op_{host} {}

        /** Map RON events into some external representation (e.g. JSON). */
        Status Map(Builder& response, Cursor& query) const {
            const Uuid& id = query.id();
            if (id == CHAIN_MAPPER_ID) {
                return chain_.Map(response, query);
            } else if (id == OP_MAPPER_ID) {
                return op_.Map(response, query);
            } else {
                return Status::NOT_IMPLEMENTED.comment("unknown mapper");
            }
        }

        /** Consume external representation, produce new RON events. */
        Status Write(rocksdb::WriteBatch& batch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

}

#endif //CPP_MAPPERS_HPP
