#include "../map.hpp"

namespace ron {

    template<typename Frame>
    Status ChainMapper<Frame>::Map(Builder& response, Cursor& query, const VV& hili) const {
        const Uuid& ref = query.ref();
        if (ref.version()!=TIME) return Status::BADARGS.comment("need an op id");
        std::unique_ptr<rocksdb::Iterator> i{host_->FindChain(ref)};
        if (i==nullptr) return Status::NOT_FOUND;
        Slice chain{slice(i->value())};
        Cursor cc{chain};
        bool gotit{false};
        while (cc.valid()) {
            response.AppendOp(cc); // TODO TERM
            if (cc.id()==ref) gotit = true;
            cc.Next();
        }
        if (!gotit) return Status::NOT_FOUND.comment("no such op, apparently");
        return Status::OK;
    }

    template class ChainMapper<TextFrame>;

}
