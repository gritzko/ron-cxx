#include "db/replica.hpp"
#include "db/merge_ops.hpp"

using namespace rocksdb;
using namespace std;

namespace ron {

const Uuid CHAIN_STORE{};
const Uuid TRUNK_STORE{1024260140364201984, Word::PAYLOAD_BITS};
const Uuid SHA2_UUID{1003339682156642304UL, 0};
const Uuid OBJ_UUID{929632683238096896UL, 0};
const Uuid PREV_UUID{952132676872044544UL, 0};
const Uuid RDT_UUID{984282809185075200UL, 0};

//  U T I L

template <typename Frame>
ColumnFamilyOptions Replica<Frame>::DataCFOptions() const {
    ColumnFamilyOptions cfo{};
    // cfo.merge_operator;
    return cfo;
}
template <typename Frame>
ColumnFamilyOptions Replica<Frame>::ChainCFOptions() const {
    ColumnFamilyOptions cfo{};
    cfo.merge_operator =
        shared_ptr<MergeOperator>{new ChainMergeOperator<Frame>()};
    return cfo;
}
template <typename Frame>
ColumnFamilyOptions Replica<Frame>::LogCFOptions() const {
    ColumnFamilyOptions cfo{};
    return cfo;
}

const Uuid HISTORY_CF_UUID = Uuid{805545704900788224UL, 0};
const string HISTORY_CF_NAME = HISTORY_CF_UUID.str();
const Uuid LOG_CF_UUID = Uuid{879235468267356160UL, 0};
const string LOG_CF_NAME = LOG_CF_UUID.str();

//  C H A I N S

template <typename Frame>
Status Replica<Frame>::ChainMeta::NextOp(Cursor& cur) {
    SHA2 need_hash{};
    // ABSORB OR COMPARE
    while (cur.valid() && cur.id().version() == NAME) {  // eat annos
        if (cur.id() == SHA2_UUID && cur.type(2) == ATOM::STRING) {
            need_hash = SHA2{cur.parse_string(2)};  // FIXME check
        } else if (cur.id() == OBJ_UUID && cur.type(2) == ATOM::UUID) {
            const Uuid& need_object = cur.parse_uuid(2);
            if (object.zero()) {
                object = need_object;
            } else if (cur.parse_uuid(2) != object) {
                return Status::TREEBREAK;
            }
        } else if (cur.id() == PREV_UUID && cur.type(2) == ATOM::UUID) {
            const Uuid& need_prev = cur.parse_uuid(2);
            if (!at.zero() && need_prev != at) return Status::CHAINBREAK;
        }
        cur.Next();
    }
    if (!cur.valid()) return Status::OK;

    const Uuid& ref = cur.ref();
    const Uuid& id = cur.id();

    if (ref.version() == NAME) {
        if (!at.zero()) return Status::CHAINBREAK;
        object = id;
    } else if (ref != at) {
        return Status::BAD_STATE;
    }

    if (id.origin() != at.origin()) return Status::CHAINBREAK;

    SHA2 next_hash;  // = SHA2{hash_, hash_, cur};
    hash_op<Frame>(cur, next_hash, hash, hash);
    if (need_hash != next_hash)  // length-0 hash equals anything
        return Status::HASHBREAK;

    at = id;
    hash = next_hash;
    cur.Next();

    return Status::OK;
}

//  L I F E C Y C L E

template <typename Frame>
Status Replica<Frame>::Create(std::string home) {
    DBOptions db_options;
    db_options.create_if_missing = true;
    db_options.error_if_exists = true;
    db_options.max_total_wal_size = UINT64_MAX;
    db_options.WAL_size_limit_MB = 1UL << 30U;
    db_options.WAL_ttl_seconds = UINT64_MAX;

    ColumnFamilyOptions cf_options = DataCFOptions();
    Options options{db_options, cf_options};

    auto status = DB::Open(options, home, &db_);
    if (!status.ok()) return Status::DB_FAIL;

    status =
        db_->CreateColumnFamily(ChainCFOptions(), CHAIN_STORE.str(), &chains_);
    if (!status.ok()) return Status::DB_FAIL;
    /*

    status = db_->CreateColumnFamily(LogCFOptions(), LOG_CF_NAME, &log_cf_);
    if (!status.ok()) return Status::DB_FAIL;
    */

    return Status::OK;
}

template <typename Frame>
Status Replica<Frame>::Open(std::string home) {
    if (db_) return Status::BAD_STATE;

    Options options;
    options.create_if_missing = false;
    options.error_if_exists = false;
    options.max_total_wal_size = UINT64_MAX;
    options.WAL_size_limit_MB = 1UL << 30U;
    options.WAL_ttl_seconds = UINT64_MAX;

    typedef ColumnFamilyDescriptor CFD;
    vector<ColumnFamilyDescriptor> families;
    vector<ColumnFamilyHandle*> handles;
    families.push_back(CFD{kDefaultColumnFamilyName, ChainCFOptions()});
    //    families.push_back(CFD{HISTORY_CF_NAME, HistoryCFOptions()});
    //    families.push_back(CFD{LOG_CF_NAME, LogCFOptions()});

    auto status = DB::Open(options, home, families, &handles, &db_);
    if (!status.ok()) {
        return Status::DB_FAIL;
    }

    auto i = handles.begin();
    /*data_cf_ = *i++;
    history_cf_ = *i++;
    log_cf_ = *i++;
    */
    return Status::OK;
}

template <typename Frame>
Status Replica<Frame>::GC() {
    return Status::OK;
}

template <typename Frame>
Status Replica<Frame>::Close() {
    if (chains_) {
        delete chains_;
        chains_ = nullptr;
    }
    /*if (history_cf_) {
        delete history_cf_;
        history_cf_ = nullptr;
    }
    if (log_cf_) {
        delete log_cf_;
        log_cf_ = nullptr;
    }*/
    if (db_) {
        delete db_;
        db_ = nullptr;
    }
    return Status::OK;
}

template <typename Frame>
Replica<Frame>::~Replica() {
    if (!db_) Close();
}

//  C H A I N  S T O R E

// chain store records for a chain starting at 12345+origin
// meta: @12345-origin :1234-object 5 'sha3hash' 'dsasig'
// chain: @12345-origin :root+ref 'value1', 'value2'...
// FindChain() finds the (only possible) chain for the id,
// but checks no range validity, no hash validity.
template <class Frame>
Status Replica<Frame>::FindChain(Uuid op_id, std::string& chain) {
    rocksdb::ReadOptions ro;
    Key key{op_id, RDT::CHAIN};
    auto i = db_->NewIterator(ro, chains_);
    i->SeekForPrev(key);
    if (!i->Valid()) {
        delete i;
        return Status::NOT_FOUND;
    }
    Uuid at = Key{i->key()}.id();
    if (at.version() != TIME) {
        delete i;
        return Status::NOT_FOUND;
    }
    chain.clear();
    // prepend_id(op_id.derived(), chain);
    chain.append(i->value().data_, i->value().size_);
    delete i;
    return Status::OK;
    // can parallelize this if a yarn is pinned to a thread
}

template <class Frame>
Status Replica<Frame>::WalkChain(const Frame& chain, const Frame& meta,
                                 const Uuid& to, std::string& hash) {
    Cursor chc = chain.cursor();
    return Status::OK;
}

//  R E C E I V E S

template <class Frame>
Status Replica<Frame>::ReceiveChain(rocksdb::WriteBatch& batch,
                                    Uuid object_store, Cursor& cur) {
    // Cursor cur = chain.cursor();
    ChainMeta meta, prev_meta, ref_meta;

    const Uuid& id = cur.op().id();
    const Uuid& ref = cur.op().ref();

    // fetch the meta on the prev op (likely, chain)
    Status ok = FindChainMeta(Uuid{NEVER, id.origin()}, prev_meta);
    if (!ok) return ok;
    bool new_chain = ref.version() != TIME || ref.origin() != id.origin();

    // fetch the meta on the referenced op (get the object)
    if (ref.version() == NAME) {  // new object
        ref_meta.object = id;
        hash_uuid(ref, ref_meta.hash);
    } else if (ref != prev_meta.at) {  // a new chain
        ok = FindChainMeta(ref, ref_meta);
        if (!ok) return ok;
    } else {
        ref_meta = prev_meta;
    }

    meta.object = prev_meta.object;
    // first op hash, according to our records
    hash_op<Frame>(cur, meta.hash, prev_meta.hash, ref_meta.hash);

    // walk/check the chain
    do {
        ok = meta.NextOp(cur);  // checks hash-mentions
        // if (meta.has_object() && ref_meta.object()!=meta.object())
        //    return WRONG_OBJECT;
        // also hash
        if (!ok) return ok;

        // WRITE THE CHAIN
        // WRITE OBJ-ADD

    } while (cur.Next());

    Key key{meta.object, meta.rdt};
    if (new_chain) {
        Builder anno;
        anno.AppendNewOp(HEADER, SHA2_UUID, id, meta.hash.base64());
        anno.AppendNewOp(HEADER, OBJ_UUID, id, meta.object);
        batch.Merge(chains_, key, anno.frame().data());
    }
    // batch.Merge(chains_, key, chain);
    // batch.Merge(cf(object_store), Key{object}, chain);

    return Status::OK;
}

/*   template <class Frame>
Status Replica<Frame>::ReceiveLog(ron::Uuid store, Frame frame){
   WriteBatch batch;

   Cursor cur = frame.cursor();
   while (cur.valid()) { // TODO big switch for 16 op patterns
       if (cur.op().ref().version()==UUID::NAME) {
           if (cur.op().id().version()==UUID::TIME) {
               ReceiveChain(store, cur, batch); // new object
           } else if (cur.op().id().version()==UUID::DERIVED) {
               // hash check?
           } else {
               return Status::BAD_STATE;
           }
       } else {
           ReceiveChain(store, cur, batch);
       }
   }

   db_->Write(wo, &batch);

   return Status::OK;
}*/

/*template<typename Frame>
template<class FrameB>
Status Replica<Frame>::Recv (Builder& response, const FrameB& input) {
    // scan, check, convert, split
    Batch to_process;
    Builder next;
    //typedef typename FrameB::Cursor CursorB;
    auto cur = input.cursor();
    do {
        if (!next.empty() && cur.op().term()!=TERM::REDUCED) {
            to_process.push_back(next.frame());
            next = Builder{};
        }
        next.AddOp(cur.op(), input.data());
    } while (cur.Next());

    return Recv(response, to_process);
}*/

template class Replica<TextFrame>;

}  // namespace ron
