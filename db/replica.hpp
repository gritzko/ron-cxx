#ifndef RON_REPLICA_HPP
#define RON_REPLICA_HPP

#include <rdt/lww.hpp>
#include <string>
#include <unordered_map>
#include "rdt/rdt.hpp"
#include "rocksdb/db.h"
#include "ron/hash.hpp"
#include "ron/ron.hpp"

namespace ron {

template <typename Frame>
class Replica {
  rocksdb::DB* db_;

  rocksdb::WriteOptions wo_;
  rocksdb::ReadOptions ro_;

  struct ChainMeta {
    Word at_;
    SHA2 hash_;
    Uuid object_;
  };
  // chain cache - skip db reads for ongoing op chains
  std::unordered_map<Word, ChainMeta> cache_;

 public:
  typedef typename Frame::Batch Batch;
  typedef typename Frame::Builder Builder;
  typedef typename Frame::Cursor Cursor;

  typedef rocksdb::ColumnFamilyHandle CFHandle;

  CFHandle* chains_;
  std::unordered_map<Uuid, CFHandle*> objects_;

  Replica() : db_{nullptr}, chains_{nullptr}, objects_{} {}

  struct Key {
    uint64_t bits_[2];

    Key(const Uuid& id)
        : bits_{
              htobe64((id.origin()._64 << 4U) | (id.value()._64 >> 60U)),
              htobe64((id.value()._64 << 4U) | (id.origin()._64 >> 60U)),
          } {}

    Key(rocksdb::Slice data)
        : bits_{*(uint64_t*)data.data_,
                *(uint64_t*)(data.data_ + sizeof(uint64_t))} {
      assert(data.size() == sizeof(Key));
    }

    operator Uuid() const {
      uint64_t v = be64toh(bits_[1]);
      uint64_t o = be64toh(bits_[0]);
      return Uuid{(v >> 4U) | (o << 60U), (o >> 4U) | (v << 60U)};
    }

    operator rocksdb::Slice() const {
      return rocksdb::Slice{(char*)this, sizeof(Key)};
    }
  };

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

  Status WalkChain(const Frame& chain, const Frame& meta, const Uuid& to,
                   std::string& hash);

  rocksdb::Iterator* FindYarn(Word replica);

  //  O B J E C T  S T O R E

  Status CreateObjectStore(Uuid store);

  Status FillObjectStore(Uuid store, VV version);

  Status SplitObjectStore(Uuid store, Uuid new_store, VV version);

  Status DropObjectStore(Uuid store);

  Status GetObject(const Uuid& store, const Uuid& key, Frame& frame);

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
  // converts any-coded incoming frame into internal-coded chains, queries, hash
  // checks
  template <class FrameB>
  Status Receive(Uuid conn_id, const FrameB& frame);

  // feed a causally ordered log - checks causality, updates the chain cache
  Status ReceiveChain(rocksdb::WriteBatch& batch, Uuid object_store,
                      const Frame& frame);

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

  Status RecvData(Builder& response, const Frame& query) { return Status::OK; }

 private:
  //  U T I L

  rocksdb::ColumnFamilyOptions DataCFOptions() const;
  rocksdb::ColumnFamilyOptions HistoryCFOptions() const;
  rocksdb::ColumnFamilyOptions LogCFOptions() const;
};

extern const Uuid HISTORY_CF_UUID;
extern const Uuid LOG_CF_UUID;
extern const std::string HISTORY_CF_NAME;
extern const std::string LOG_CF_NAME;
extern const Uuid CHAIN_STORE;
extern const Uuid TRUNK_STORE;

}  // namespace ron

#endif
