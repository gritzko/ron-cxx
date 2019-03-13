#ifndef RON_REPLICA_HPP
#define RON_REPLICA_HPP

#include <string>
#include <unordered_map>
#include "const.hpp"
#include "key.hpp"
#include "rdt/lww.hpp"
#include "rdt/rdt.hpp"
#include "rocksdb/db.h"
#include "ron/hash.hpp"
#include "ron/opmeta.hpp"
#include "ron/ron.hpp"

namespace ron {

template <typename Frame>
class Replica {
   public:
    using Batch = typename Frame::Batch;
    using Builder = typename Frame::Builder;
    using Cursor = typename Frame::Cursor;
    using CFHandle = rocksdb::ColumnFamilyHandle;
    using tipmap_t = std::unordered_map<Word, OpMeta>;

    static const Uuid NOW_UUID;

   private:
    rocksdb::DB* db_;
    rocksdb::WriteOptions wo_;
    rocksdb::ReadOptions ro_;
    CFHandle* trunk_;

    Uuid now_;

    std::unordered_map<Uuid, CFHandle*> objects_;

    // chain cache - skip db reads for ongoing op chains
    tipmap_t tips_;

    Key nil_key() const { return Key{Uuid::NIL, META_RDT}; }

   public:
    Replica() = default;

    //  L I F E C Y C L E

    Status Create(std::string home, Word origin = 0);

    Status Open(std::string home);

    Status GC();

    Status Close();

    ~Replica();

    Uuid now();
    Status See(Uuid timestamp) {
        if (timestamp < now_) return Status::OK;
        // TODO sanity/plausibility check
        now_ = Uuid{timestamp.value(), now_.origin()};
        return Status::OK;
    }

    rocksdb::DB& db() { return *db_; }
    const rocksdb::ReadOptions& ro() const { return ro_; }
    const rocksdb::WriteOptions& wo() const { return wo_; }
    inline bool open() const { return db_ != nullptr; }
    inline CFHandle* trunk() const { return trunk_; }

    //  C H A I N  S T O R E

    rocksdb::Iterator* FindYarn(Uuid replica);
    rocksdb::Iterator* FindChain(Uuid op_id);

    //  O B J E C T  S T O R E

    Status CreateBranch(Uuid store);

    Status FillBranch(Uuid store, VV version);

    Status SplitBranch(Uuid store, Uuid new_store, VV version);

    Status DropBranch(Uuid store);

    Status GetObject(Frame& frame, Uuid key, Uuid store = Uuid::NIL) {
        return Status::NOT_IMPLEMENTED;
    }

    Status Get(Frame& object, Uuid id, Uuid rdt = Uuid::NIL,
               Uuid branch = Uuid::NIL);

    Status GetMap(Frame& result, Uuid id, Uuid map = Uuid::NIL,
                  Uuid branch = Uuid::NIL);

    inline Status GetChain(Frame& chain, Uuid chain_id) {
        return Get(chain, chain_id, CHAIN_MAP_ID);
    }

    /** If we don't know the exact chain id, we have to scan the table to
     *  find the chain. Then, we scan the chain to find the op. */
    Status FindOpMeta(OpMeta& meta, Uuid target_id);

    /** Fetches the op metadata for the chain head op.
     * @param meta - the op meta object with op id set to the chain id */
    Status GetHeadMeta(OpMeta& meta) {
        Frame raw;
        Uuid chain_id = meta.id;
        Status ok = Get(raw, chain_id, META_MAP_ID);
        if (!ok) return ok;
        for (Cursor c = raw.cursor(); ok && c.valid(); c.Next()) {
            ok = meta.ReadAnnotation(c);
        }
        return ok;
    }

    inline Status FindTipMeta(OpMeta& meta, Word origin) {
        return FindOpMeta(meta, Uuid{NEVER, origin});
    }

    // Q U E R I E S

    Status DataQuery(Builder& response, Uuid id);

    //  @object+id :type ?
    //  @last+version ?
    //  @from+version :till+version ?
    //
    //  @object+id :type !
    //  @diffop1 'value'
    //  @diffop2 'value'
    //  ...
    Status DiffQuery(Builder& response, Uuid id);

    //  @queried-id :vv ?
    //
    //  @queried-id :vv !
    //  @time1-origin1 ,
    //  @time2-origin2 ,
    //  ...
    Status VVQuery(Builder& response, Uuid id);

    //  @queried-id :sha3 ?
    //
    //  @queried-id :sha3 'sha3hash' !
    Status SHA3Query(Builder& response, Uuid id);

    //  R E C E I V E S

    // the entry point: recoder, normalizer, access control
    // converts any-coded incoming frame into internal-coded chains, queries,
    // hash checks
    template <class FrameB>
    Status Receive(Uuid conn_id, const FrameB& frame) {
        return Status::NOT_IMPLEMENTED;
    }

    // feed a causally ordered log - checks causality, updates the chain cache
    Status ReceiveChain(rocksdb::WriteBatch& batch, Uuid object_store,
                        Cursor& chain);

    Status ReceiveMap(rocksdb::WriteBatch& batch, Uuid object_store,
                      Cursor& chain) {
        return Status::NOT_IMPLEMENTED;
    }

    Status ReceiveCheck(Uuid branch, Cursor& check) {
        return Status::NOT_IMPLEMENTED;
    }

    // a hash check MUST follow its op in the frame => the hash must be cached
    Status ReceiveSHA2Check(Uuid id, const SHA2& check) {
        return Status::NOT_IMPLEMENTED;
    }

    Status ReceiveQuery(Builder& response, Uuid object_store, Cursor& query) {
        Uuid ref = query.ref();
        RDT rdt = uuid2rdt(ref);
        if (rdt != RDT_COUNT) {
            return ReceiveObjectQuery(response, object_store, query);
        }
        MAP mapper = uuid2map(ref);
        if (mapper != MAP_COUNT) {
            return ReceiveMapQuery(response, object_store, query);
        }
        return Status::NOT_IMPLEMENTED.comment("unknown RDT/mapper " +
                                               ref.str());
    }

    Status ReceiveObjectQuery(Builder& response, Uuid object_store,
                              Cursor& query);

    Status ReceiveMapQuery(Builder& response, Uuid object_store, Cursor& query);

    Status RecvData(Builder& response, Cursor& query) {
        return Status::NOT_IMPLEMENTED;
    }

    Status Receive(Builder& response, Uuid branch, Cursor& c);

   private:
    //  U T I L

    rocksdb::ColumnFamilyOptions CFOptions() const;

    inline static Status status(const rocksdb::Status& orig) {
        return orig.ok() ? Status::OK
                         : Status::DB_FAIL.comment(orig.ToString());
    }
};

inline Slice slice(rocksdb::Slice s) {
    return Slice{s.data_, (fsize_t)s.size_};
}
inline Slice slice(const rocksdb::Slice* s) { return slice(*s); }

}  // namespace ron

#endif
