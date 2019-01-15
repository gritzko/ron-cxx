#include "map.hpp"

namespace ron {

    template<typename Frame>
    Status MetaMapper<Frame>::Map(Builder& response, Cursor& query, const VV& hili) const {
        const Uuid& ref = query.ref();
        if (ref.version()!=TIME) return Status::BADARGS.comment("need an op id");
        OpMeta meta;
        Status ok = host_->FindOpMeta(meta, query.ref());
        if (!ok) return ok;
        const Uuid& id = query.id();
        const MAP map = uuid2map(id);
        switch (map) {
            case META_MAP:
                meta.AppendAnnos(response);
                break;
            case SHA2_MAP:
                response.AppendNewOp(RAW, SHA2_MAP_ID, ref, meta.hash.base64());
                break;
            case PREV_MAP:
                response.AppendNewOp(RAW, PREV_MAP_ID, ref, Uuid{meta.prev, ref.origin()});
                break;
            case HEAD_MAP:
                response.AppendNewOp(RAW, HEAD_MAP_ID, ref, Uuid{meta.chain, ref.origin()});
                break;
            case OBJ_MAP:
                response.AppendNewOp(RAW, OBJ_MAP_ID, ref, meta.object);
                break;
            default:
                return Status::NOT_IMPLEMENTED;
        }
        return Status::OK;
    }

    template class MetaMapper<TextFrame>;

}
