#ifndef CPP_MAPPERS_HPP
#define CPP_MAPPERS_HPP

#include "db/replica.hpp"
#include "db/map/const.hpp"

namespace ron {


    template<typename Frame>
    struct ChainMapper {
        using Cursor = typename Frame::Cursor;
        using Builder = typename Frame::Builder;
        using HostReplica = Replica<Frame>;

        HostReplica* host_;

        explicit ChainMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const;

        Status Write(Builder& patch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    template<typename Frame>
    struct OpMapper {
        using Cursor = typename Frame::Cursor;
        using Builder = typename Frame::Builder;
        using HostReplica = Replica<Frame>;

        HostReplica* host_;

        explicit OpMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const;

        Status Write(Builder& patch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    template<typename Frame>
    struct YarnMapper {
        using Cursor = typename Frame::Cursor;
        using Builder = typename Frame::Builder;
        using HostReplica = Replica<Frame>;

        HostReplica* host_;

        explicit YarnMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const;

        Status Write(Builder& patch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    template<typename Frame>
    struct MetaMapper {
        using Cursor = typename Frame::Cursor;
        using Builder = typename Frame::Builder;
        using HostReplica = Replica<Frame>;

        HostReplica* host_;

        explicit MetaMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const;

        Status Write(Builder& patch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

    template <typename Frame>
    struct CSVMapper {
        using Cursor = typename Frame::Cursor;
        using Builder = typename Frame::Builder;
        using HostReplica = Replica<Frame>;
        typedef typename MatrixRDT<Frame>::mx_t mx_t;

        HostReplica* host_;

        explicit CSVMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const;

        Status Write(Builder& patch, Cursor& query) const;

    };

    template <typename Frame>
    struct TxtMapper {
        using Cursor = typename Frame::Cursor;
        using Builder = typename Frame::Builder;
        using HostReplica = Replica<Frame>;
        typedef typename MatrixRDT<Frame>::mx_t mx_t;

        HostReplica* host_;

        explicit TxtMapper(HostReplica* host) : host_{host} {}

        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const;

        Status Write(Builder& patch, Cursor& query) const;

    };

    template<typename Frame>
    struct MasterMapper {
        using Cursor = typename Frame::Cursor;
        using Builder = typename Frame::Builder;
        using HostReplica = Replica<Frame>;

        ChainMapper<Frame> chain_;
        OpMapper<Frame> op_;
        YarnMapper<Frame> yarn_;
        MetaMapper<Frame> meta_;
        CSVMapper<Frame> csv_;
        TxtMapper<Frame> txt_;

        explicit MasterMapper(HostReplica* host) : chain_{host}, op_{host}, yarn_{host}, meta_{host}, csv_{host}, txt_{host} {}

        /** Map RON events into some external representation (e.g. JSON). */
        Status Map(Builder& response, Cursor& query, const VV& hili=EMPTY_VV) const {
            const Uuid& id = query.id();
            const Uuid& ref = query.ref();
            if (ref.version()!=NAME) return Status::BAD_STATE.comment("the ref is not a mapper id");
            const MAP map = uuid2map(ref);
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
                case TXT_MAP:
                    return txt_.Map(response, query);
                default:
                    return Status::NOT_IMPLEMENTED.comment("unknown mapper "+ref.str());
            }
        }

        /** Consume external representation, produce new RON events. */
        Status Write(Builder& patch, Cursor& query) const {
            return Status::NOT_IMPLEMENTED;
        }

    };

}

#endif //CPP_MAPPERS_HPP
