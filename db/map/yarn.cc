#include "map.hpp"

namespace ron {

    template<typename Frame>
    Status YarnMapper<Frame>::Map(Builder& response, Cursor& query, const VV& hili) const {
        const Uuid& ref = query.ref();
        if (ref.version()!=TIME) return Status::BADARGS.comment("need an op id");
        std::unique_ptr<rocksdb::Iterator> i{host_->FindYarn(ref)};
        if (i==nullptr) return Status::NOT_FOUND.comment("no such yarn");
        while (i->Valid()) {
            slice_t chain{slice(i->value())};
            Cursor cc{chain};
            while (cc.valid()) { // TODO range
                response.AppendOp(cc); // TODO TERM
                cc.Next();
            }
            do {
                i->Next();
            } while (i->Valid() && Key{i->key()}.rdt()!=CHAIN_RDT);
        }
        return Status::OK;
    }

    template class YarnMapper<TextFrame>;

}
