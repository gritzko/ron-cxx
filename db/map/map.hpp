#ifndef CPP_MAPPERS_HPP
#define CPP_MAPPERS_HPP

#include "db/replica.hpp"
#include "db/map/const.hpp"

namespace ron {


    template<typename Frame>
    struct ChainMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;

        HostReplica* host_;

        explicit ChainMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const;

        Status Write(Builder& patch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    template<typename Frame>
    struct OpMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;

        HostReplica* host_;

        explicit OpMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const;

        Status Write(Builder& patch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    template<typename Frame>
    struct YarnMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;

        HostReplica* host_;

        explicit YarnMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const;

        Status Write(Builder& patch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    template<typename Frame>
    struct MetaMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;

        HostReplica* host_;

        explicit MetaMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const;

        Status Write(Builder& patch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    template <typename Frame>
    struct CSVMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;
        typedef typename MatrixRDT<Frame>::mx_t mx_t;

        HostReplica* host_;

        explicit CSVMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const;

        Status Write(Builder& patch, Cursor& query) const;

    };


    template<typename Frame>
    struct MasterMapper {
        typedef typename Frame::Cursor Cursor;
        typedef typename Frame::Builder Builder;
        typedef Replica<Frame> HostReplica;

        ChainMapper<Frame> chain_;
        OpMapper<Frame> op_;
        YarnMapper<Frame> yarn_;
        MetaMapper<Frame> meta_;
        CSVMapper<Frame> csv_;

        explicit MasterMapper(HostReplica* host) : chain_{host}, op_{host}, yarn_{host}, meta_{host}, csv_{host} {}

        /** Map RON events into some external representation (e.g. JSON). */
        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const {
            const Uuid& id = query.id();
            if (id.version()!=NAME) return Status::BAD_STATE;
            const MAP map = uuid2map(id);
            switch (map) {
                case CHAIN_MAP:
                    return chain_.Map(response, query);
                case OP_MAP:
                    return op_.Map(response, query);
                case YARN_MAP:
                    return yarn_.Map(response, query);
                case META_MAP:
                case SHA2_MAP:
                case PREV_MAP:
                case HEAD_MAP:
                case OBJ_MAP:
                    return meta_.Map(response, query);
                case CSV_MAP:
                    return csv_.Map(response, query);
                default:
                    return Status::NOT_IMPLEMENTED.comment("unknown mapper "+id.str());
            }
        }

        /** Consume external representation, produce new RON events. */
        Status Write(Builder& patch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

}

#endif //CPP_MAPPERS_HPP
