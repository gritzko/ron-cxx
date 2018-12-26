#ifndef RON_REPLICA_HPP
#define RON_REPLICA_HPP

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

   private:
    rocksdb::DB* db_;
    rocksdb::WriteOptions wo_;
    rocksdb::ReadOptions ro_;
    CFHandle* trunk_;

    std::unordered_map<Uuid, CFHandle*> objects_;

   public:
    // chain cache - skip db reads for ongoing op chains
    typedef std::unordered_map<Word, OpMeta> tipmap_t;
    tipmap_t tips_;

   public:
    Replica() : db_{nullptr}, trunk_{nullptr}, objects_{}, wo_{}, ro_{} {}

    //  L I F E C Y C L E

    Status Create(std::string home);

    Status Open(std::string home);

    Status GC();

    Status Close();

    ~Replica();

    rocksdb::DB& db() { return *db_; }
    const rocksdb::ReadOptions& ro() const { return ro_; }
    const rocksdb::WriteOptions& wo() const { return wo_; }
    inline bool open() const { return db_ != nullptr; }
    inline CFHandle* trunk() const { return trunk_; }

    //  C H A I N  S T O R E

    Status FindChain(Uuid op_id, std::string& chain);
    Status FindChainMeta(Uuid op_id, OpMeta& meta) {
        std::string chain;
        Status ok = FindChain(op_id, chain);
        if (!ok) return ok;
        Cursor c{chain};
        while (c.valid()) {
            if (c.id().version() == NAME) meta.ScanAnno(c);
            c.Next();
        }
        return Status::OK;
    }

    rocksdb::Iterator* FindYarn(Word replica);

    //  O B J E C T  S T O R E

    Status CreateObjectStore(Uuid store);

    Status FillObjectStore(Uuid store, VV version);

    Status SplitObjectStore(Uuid store, Uuid new_store, VV version);

    Status DropObjectStore(Uuid store);

    Status GetObject(const Uuid& store, const Uuid& key, Frame& frame);

    Status Get(Frame& object, const Uuid& id, const Uuid& rdt = Uuid::NIL,
               const Uuid& branch = Uuid::NIL);

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

    Status ReceiveQuery(Builder& response, Uuid object_store,
                        const Frame& query) {
        Cursor cur = query.cursor();
        //        switch (cur.op().type().value().get30(1)) { // assuming these
        //        names <= 5 chars
        //            case LWW_INT:
        //            //case RGA_DATA_ID[VALUE]:
        //                return data_.RecvQuery(response, cur);
        //            case VV_FUNC_INT:
        //                return vv_.RecvQuery(response, cur);
        //            case SHA3_FUNC_INT:
        //                return sha3_.RecvQuery(response, cur);
        //        }
        return Status::OK;
    }

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
