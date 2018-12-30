#include "map.hpp"

namespace ron {

    template<typename Frame>
    Status MetaMapper<Frame>::Map(Builder& response, Cursor& query) const {
        const Uuid& ref = query.ref();
        if (ref.version()!=TIME) return Status::BADARGS.comment("need an op id");
        OpMeta meta;
        Status ok = host_->FindOpMeta(meta, query.ref());
        if (query.id()==META_MAPPER_ID) {
            meta.AppendAnnos(response);
        } else if (query.id()==SHA2_MAPPER_ID) {
            response.AppendNewOp(RAW, SHA2_MAPPER_ID, ref, meta.hash.base64());
        } else if (query.id()==PREV_MAPPER_ID) {
            response.AppendNewOp(RAW, PREV_MAPPER_ID, ref, Uuid{meta.prev, ref.origin()});
        } else if (query.id()==HEAD_MAPPER_ID) {
            response.AppendNewOp(RAW, HEAD_MAPPER_ID, ref, Uuid{meta.chain, ref.origin()});
        } else if (query.id()==OBJ_MAPPER_ID) {
            response.AppendNewOp(RAW, OBJ_MAPPER_ID, ref, meta.object);
        }
        return Status::OK;
    }

    template class MetaMapper<TextFrame>;

}
