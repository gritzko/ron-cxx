#ifndef RON_REPLICA_HPP
#define RON_REPLICA_HPP

#include <rdt/lww.hpp>
#include <string>
#include <unordered_map>
#include "db/key.hpp"
#include "rdt/rdt.hpp"
#include "rocksdb/db.h"
#include "ron/hash.hpp"
#include "ron/ron.hpp"

namespace ron {

template <typename Frame>
class Replica {
   public:
    typedef typename Frame::Batch Batch;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;

   private:
    rocksdb::DB* db_;

    rocksdb::WriteOptions wo_;
    rocksdb::ReadOptions ro_;

   public:
    // The defining feature of an op chain is that the next op references
    // the previous (yarn-previous) one; a chain is a chunk of some yarn.
    // The hash of the next op only depends on the previous op.
    struct ChainMeta {
        Uuid at;
        SHA2 hash;
        Uuid object;
        RDT rdt;

        ChainMeta() : at{}, hash{}, object{}, rdt{} {}
        // learns/verifies 3 annotations: @obj, @sha2, @prev
        Status ScanOp(Cursor &op);
        Status ScanAll(Cursor &cur) {
            Status ret;
            do {
                ret = ScanOp(cur);
            } while (ret && cur.Next());
            return ret;
        }
        inline Status ScanFrame(const std::string &data) {
            Frame f{data};
            Cursor c{f};
            return ScanAll(c);
        }
    };
    // chain cache - skip db reads for ongoing op chains
    std::unordered_map<Word, ChainMeta> cache_;

   public:
    typedef rocksdb::ColumnFamilyHandle CFHandle;

    CFHandle* trunk_;
    std::unordered_map<Uuid, CFHandle*> objects_;

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

    //  C H A I N  S T O R E

    Status FindChain(Uuid op_id, std::string& chain);
    Status FindChainMeta(Uuid op_id, ChainMeta& meta) {
        std::string chain;
        Status ok = FindChain(op_id, chain);
        if (ok) meta.ScanFrame(chain);
        return ok;
    }

    Status WalkChain(const Frame& chain, const Frame& meta, const Uuid& to,
                     std::string& hash);

    rocksdb::Iterator* FindYarn(Word replica);

    //  O B J E C T  S T O R E

    Status CreateObjectStore(Uuid store);

    Status FillObjectStore(Uuid store, VV version);

    Status SplitObjectStore(Uuid store, Uuid new_store, VV version);

    Status DropObjectStore(Uuid store);

    Status GetObject(const Uuid& store, const Uuid& key, Frame& frame);

    Status Get (Frame& object, const Uuid& id, const Uuid& rdt=Uuid::ZERO, const Uuid& branch=Uuid::ZERO);

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

extern const Uuid HISTORY_CF_UUID;
extern const Uuid LOG_CF_UUID;
extern const std::string HISTORY_CF_NAME;
extern const std::string LOG_CF_NAME;
extern const Uuid CHAIN_STORE;
extern const Uuid TRUNK_STORE;
extern const Uuid SHA2_UUID;
extern const Uuid OBJ_UUID;
extern const Uuid PREV_UUID;
extern const Uuid RDT_UUID;

}  // namespace ron

#endif
