#include "../map.hpp"

namespace ron {

template <typename Frame>
Status OpMapper<Frame>::Map(Builder& response, Cursor& query,
                            const VV& hili) const {
    const Uuid& ref = query.ref();
    if (ref.version() != TIME) return Status::BADARGS.comment("need an op id");
    std::unique_ptr<rocksdb::Iterator> i{host_->FindChain(ref)};
    if (i == nullptr) return Status::NOT_FOUND.comment("no such chain");
    Slice chain{slice(i->value())};
    Cursor cc{chain};
    while (cc.valid()) {
        if (cc.id() == ref) {
            response.AppendOp(cc);  // TODO TERM
            return Status::OK;
        }
        cc.Next();
    }
    return Status::NOT_FOUND.comment("no such op");
}

template class OpMapper<TextFrame>;

}  // namespace ron
