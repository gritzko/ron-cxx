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
    struct YarnMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;

        HostReplica* host_;

        YarnMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query) const;

        Status Write(rocksdb::WriteBatch& batch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    static const Uuid YARN_MAPPER_ID{1109533813702131712UL,0};

    template<typename Frame>
    struct MetaMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;

        HostReplica* host_;

        MetaMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query) const;

        Status Write(rocksdb::WriteBatch& batch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    static const Uuid META_MAPPER_ID{894494834235015168UL,0};
    static const Uuid SHA2_MAPPER_ID{1003339682156642304UL,0};
    static const Uuid PREV_MAPPER_ID{952132676872044544UL,0};
    static const Uuid HEAD_MAPPER_ID{804339484962324480UL,0};
    static const Uuid OBJ_MAPPER_ID{929632683238096896UL,0};

    template<typename Frame>
    struct MasterMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;

        ChainMapper<Frame> chain_;
        OpMapper<Frame> op_;
        YarnMapper<Frame> yarn_;
        MetaMapper<Frame> meta_;

        MasterMapper(HostReplica* host) : chain_{host}, op_{host}, yarn_{host}, meta_{host} {}

        /** Map RON events into some external representation (e.g. JSON). */
        Status Map(Builder& response, Cursor& query) const {
            const Uuid& id = query.id();
            if (id == CHAIN_MAPPER_ID) {
                return chain_.Map(response, query);
            } else if (id == OP_MAPPER_ID) {
                return op_.Map(response, query);
            } else if (id == YARN_MAPPER_ID) {
                return yarn_.Map(response, query);
            } else if (id == META_MAPPER_ID) {
                return meta_.Map(response, query);
            } else if (id == SHA2_MAPPER_ID) {
                return meta_.Map(response, query);
            } else if (id == PREV_MAPPER_ID) {
                return meta_.Map(response, query);
            } else if (id == HEAD_MAPPER_ID) {
                return meta_.Map(response, query);
            } else if (id == OBJ_MAPPER_ID) {
                return meta_.Map(response, query);
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
