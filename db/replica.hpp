#ifndef RON_REPLICA_HPP
#define RON_REPLICA_HPP

#include <db/map/const.hpp>
#include <rdt/lww.hpp>
#include <string>
#include <unordered_map>
#include "db/key.hpp"
#include "rdt/rdt.hpp"
#include "rocksdb/db.h"
#include "ron/hash.hpp"
#include "ron/opmeta.hpp"
#include "ron/ron.hpp"

namespace ron {

template <typename Frame>
class Replica {
   public:
    typedef typename Frame::Batch Batch;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef rocksdb::ColumnFamilyHandle CFHandle;
    typedef std::unordered_map<Word, OpMeta> tipmap_t;

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
    Replica() : db_{nullptr}, trunk_{nullptr}, objects_{}, wo_{}, ro_{} {}

    //  L I F E C Y C L E

    Status Create(std::string home, Word origin = 0);

    Status Open(std::string home);

    Status GC();

    Status Close();

    ~Replica();

    Uuid now();
    Status See(const Uuid& timestamp) {
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

    rocksdb::Iterator* FindYarn(const Uuid& replica);
    rocksdb::Iterator* FindChain(const Uuid& op_id);

    //  O B J E C T  S T O R E

    Status CreateBranch(Uuid store);

    Status FillBranch(Uuid store, VV version);

    Status SplitBranch(Uuid store, Uuid new_store, VV version);

    Status DropBranch(Uuid store);

    Status GetObject(const Uuid& store, const Uuid& key, Frame& frame);

    Status Get(Frame& object, const Uuid& id, const Uuid& rdt = Uuid::NIL,
               const Uuid& branch = Uuid::NIL);

    Status GetMap(Frame& result, const Uuid& id, const Uuid& map = Uuid::NIL,
                  const Uuid& branch = Uuid::NIL);

    inline Status GetChain(Frame& chain, Uuid chain_id) {
        return Get(chain, chain_id, CHAIN_MAP_ID);
    }

    /** If we don't know the exact chain id, we have to scan the table to
     *  find the chain. Then, we scan the chain to find the op. */
    Status FindOpMeta(OpMeta& meta, const Uuid& target_id);

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
    Status Receive(Uuid conn_id, const FrameB& frame);

    // feed a causally ordered log - checks causality, updates the chain cache
    Status ReceiveChain(rocksdb::WriteBatch& batch, Uuid object_store,
                        Cursor& chain);

    // a hash check MUST follow its op in the frame => the hash must be cached
    Status ReceiveSHA2Check(const Uuid& id, const SHA2& check);

    Status ReceiveQuery(Builder& response, Uuid object_store, Cursor& query);

    Status RecvData(Builder& response, const Frame& query) {
        return Status::OK;
    }

   private:
    //  U T I L

    rocksdb::ColumnFamilyOptions CFOptions() const;
};

inline slice_t slice(rocksdb::Slice s) {
    return slice_t{s.data_, (fsize_t)s.size_};
}
inline slice_t slice(const rocksdb::Slice* s) { return slice(*s); }

}  // namespace ron

#endif
