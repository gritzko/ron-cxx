#include "db/replica.hpp"

using namespace rocksdb;
using namespace std;

namespace ron {

const Uuid CHAIN_STORE{};
const Uuid TRUNK_STORE{1024260140364201984, Word::PAYLOAD_BITS};

//  U T I L

template <typename Frame>
ColumnFamilyOptions Replica<Frame>::DataCFOptions() const {
    ColumnFamilyOptions cfo{};
    // cfo.merge_operator;
    return cfo;
}
template <typename Frame>
ColumnFamilyOptions Replica<Frame>::HistoryCFOptions() const {
    ColumnFamilyOptions cfo{};
    // cfo.merge_operator;
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

    /*
    status = db_->CreateColumnFamily(HistoryCFOptions(), HISTORY_CF_NAME,
    &history_cf_); if (!status.ok()) return Status::DB_FAIL;

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
    families.push_back(CFD{kDefaultColumnFamilyName, DataCFOptions()});
    families.push_back(CFD{HISTORY_CF_NAME, HistoryCFOptions()});
    families.push_back(CFD{LOG_CF_NAME, LogCFOptions()});

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
    /*if (data_cf_) {
        delete data_cf_;
        data_cf_ = nullptr;
    }
    if (history_cf_) {
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
    Key key{op_id};
    auto i = db_->NewIterator(ro, chains_);
    i->SeekForPrev(key);
    if (!i->Valid()) {
        delete i;
        return Status::NOT_FOUND;
    }
    Uuid at{Key{i->value()}};
    if (at.version() != DERIVED) {
        delete i;
        return Status::NOT_FOUND;
    }
    /*meta.clear();
    prepend_id(op_id.derived(), meta);
    meta.append(i->value().data_, i->value().size_);
    i->Prev();
    if (!i->Valid() || Uuid{Key{i->value()}}!=op_id) {
        delete i;
        return Status::NOT_FOUND;
    }
    chain.clear();
    prepend_id(op_id.derived(), chain);
    chain.append(i->value().data_, i->value().size_);
    */
    delete i;
    return Status::OK;
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
                                    Uuid object_store, const Frame& frame) {
    Cursor cur = frame.cursor();
    const Uuid& id = cur.op().id();
    const Uuid& ref = cur.op().ref();

    // FIXME actually, we consume a chain here!
    // (put it into our separate frame)
    Frame new_chain;

    // each op: find the chain
    ChainMeta meta;
    // chain like?
    if (ref.origin() == id.origin()) {
        //      ok; chain not cached?
        //           find the chain, walk, set up the cache
        //      ok; check the hash, update the cache
    }
    if (true) {
        // no chain? create (load the ref, walk, check the hash), write meta
        string chain, meta;
        // Status prevok = FindChain(id, chain, meta);
        /*meta.walk(chain);
        Status refok = FindChain(ref, refchain, refmeta);
        meta = Chain{cur, prev, ref}; // need a fully parsed op here
        batch.Write(id.derived(), meta.frame());*/
    }
    // merge to the chain
    // batch.Merge(CHAIN_STORE, id, new_chain);
    // merge to the object
    // batch.Merge(store, id, new_chain);
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
