#include "db/replica.hpp"
#include "db/merge_ops.hpp"

using namespace rocksdb;
using namespace std;

namespace ron {

template <typename Frame>
const Uuid Replica<Frame>::NOW_UUID{915334634030497792UL, 0};

//  U T I L

template <typename Frame>
ColumnFamilyOptions Replica<Frame>::CFOptions() const {
    ColumnFamilyOptions cfo{};
    cfo.merge_operator =
        shared_ptr<MergeOperator>{new RDTMergeOperator<Frame>()};
    return cfo;
}

//  C H A I N S

/*
template <typename Frame>
Status Replica<Frame>::OpMeta::FirstChainOp(const OpMeta& prev,
                                            const OpMeta& ref, Cursor& cur) {
    object = ref.object;
    rdt = ref.rdt;
    at = cur.id();
    // first op hash, according to our records
    hash_op<Frame>(cur, hash, prev.hash, ref.hash);
    return Status::OK;
}

template <typename Frame>
Status Replica<Frame>::OpMeta::NextChainOp(Cursor& cur) {
    SHA2 need_hash{};
    // HAVE ? COMPARE : SET
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
        // rdt = uuid2rdt(ref); FIXME
        // at = id;
    } else {
        if (ref != at) return Status::BAD_STATE;
        if (id.origin() != at.origin()) return Status::CHAINBREAK;
        if (id <= at) return Status::CAUSEBREAK;
    }

    SHA2 next_hash;  // = SHA2{hash_, hash_, cur};
    hash_op<Frame>(cur, next_hash, hash, hash);
    if (!need_hash.matches(next_hash))  // length-0 hash equals anything
        return Status::HASHBREAK;

    at = id;
    hash = next_hash;

    return Status::OK;
}*/

//  L I F E C Y C L E

template <typename Frame>
Status Replica<Frame>::Create(std::string home, Word origin) {
    DBOptions db_options;
    db_options.create_if_missing = true;
    db_options.error_if_exists = true;
    db_options.max_total_wal_size = UINT64_MAX;
    db_options.WAL_size_limit_MB = 1UL << 30U;
    db_options.WAL_ttl_seconds = UINT64_MAX;

    ColumnFamilyOptions cf_options = CFOptions();
    Options options{db_options, cf_options};

    auto status = DB::Open(options, home, &db_);
    if (!status.ok()) return Status::DB_FAIL;

    trunk_ = nullptr;  // db_->DefaultColumnFamily();

    // TODO priv/pub key setup

    // set the clock up
    if (origin == 0) origin = Word::random();
    now_ = Uuid::Time(0, origin);
    now();
    Builder nowrec;
    nowrec.AppendNewOp(RAW, NOW_UUID, now_);
    cout << nowrec.data() << '\n';
    db_->Put(wo(), nil_key(), nowrec.data());

    db_->Close();
    return Open(home);
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
    families.push_back(CFD{kDefaultColumnFamilyName, CFOptions()});
    // TODO branches

    auto status = DB::Open(options, home, families, &handles, &db_);
    if (!status.ok()) {
        return Status::DB_FAIL;
    }

    auto i = handles.begin();
    trunk_ = *i;

    string meta;
    auto ok = db_->Get(ro_, nil_key(), &meta);
    Cursor mc{meta};
    do {
        if (mc.id() == NOW_UUID) now_ = mc.ref();
    } while (mc.Next());

    return Status::OK;
}

template <typename Frame>
Uuid Replica<Frame>::now() {
    Uuid next{Uuid::HybridTime(time(nullptr)), now_.origin()};
    now_ = next > now_ ? next : now_.inc();
    return now_;
}

template <typename Frame>
Status Replica<Frame>::GC() {
    return Status::OK;
}

template <typename Frame>
Status Replica<Frame>::Close() {
    if (trunk_) {
        delete trunk_;
        trunk_ = nullptr;
    }
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
    auto i = db_->NewIterator(ro, trunk_);
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

//  R E C E I V E S

/** The key lifecycle method: accepts a new chain of ops, checks it
 * against existing ops, checks integrity/consistency/causality,
 * saves the chain, updates object state.
 * @param batch a WriteBatch for all the db writes
 * @param branch
 * @param chain a cursor positioned on the head of the chain;
 *              will be moved to the first non-chain op (or EOF) */
template <class Frame>
Status Replica<Frame>::ReceiveChain(rocksdb::WriteBatch& batch, Uuid branch,
                                    Cursor& chain) {
    const Uuid& id = chain.id();
    if (id.version() != TIME) return Status::BAD_STATE;

    const Uuid& ref = chain.ref();
    if (ref.version() == TIME) {
        if (ref >= id) return Status::CAUSEBREAK;
    } else {
        if (ref.version() != NAME) return Status::BAD_STATE;
        if (uuid2rdt(ref) == RDT_COUNT) return Status::NOT_IMPLEMENTED;
    }

    // PREV: fetch the meta on the prev op (likely, chain)
    auto ti = tips_.find(id.origin());
    if (ti == tips_.end()) {
        auto ib = tips_.insert(tipmap_t::value_type{id.origin(), OpMeta{}});
        ti = ib.first;
        if (!FindChainMeta(Uuid{NEVER, id.origin()}, ti->second))
            ti->second = OpMeta(id.origin(), SHA2{});
    }
    // the tip should stay in the cache, so we do in-place writes
    OpMeta& tip = ti->second;

    OpMeta refd{tip};
    // REF: fetch the meta on the referenced op (get the object)
    if (ref != tip.id) {
        if (ref.version() == NAME) {  // new object
            refd = OpMeta{id, ref};
        } else {
            Status ok = FindChainMeta(ref, refd);
            if (!ok) return ok;
        }
    }

    // START OUR CHAIN-LET
    Builder chainlet;
    tip = OpMeta{chain, tip, refd};
    if (tip.chain_id().zero()) return Status::BAD_STATE;
    if (tip.object.zero()) return Status::BAD_STATE;

    chainlet.AppendOp(chain);
    if (tip.head()) {
        Builder annos;
        tip.AppendAnnos(annos);
        batch.Merge(trunk_, Key{tip.chain_id(), META}, annos.data());
    }
    See(id);

    // TODO chain comparator: sort annos just after the op!!!!

    // walk/check the chainlet
    while (chain.Next()) {
        if (chain.id().version() == NAME) {
            Status ok = tip.ScanAnno(chain);
            if (!ok) return ok;
        } else if (tip.next(chain)) {
            tip = OpMeta{chain, tip, tip};
            chainlet.AppendOp(chain);
        } else {
            break;  // end of the chain
        }
        See(chain.id());
    }

    // OK, SAVE
    const string& data = chainlet.data();
    RDT rdt = uuid2rdt(tip.rdt);
    if (rdt == RDT_COUNT) return Status::NOT_IMPLEMENTED;  // TODO db meta-types
    batch.Merge(trunk_, Key{tip.chain_id(), CHAIN}, data);  // TODO branches
    batch.Merge(trunk_, Key{tip.object, rdt}, data);

    return Status::OK;
}

template <typename Frame>
Status Replica<Frame>::Get(Frame& object, const Uuid& id, const Uuid& rdt,
                           const Uuid& branch) {
    if (db_ == nullptr) return Status::NOTOPEN;
    RDT t = uuid2rdt(rdt);
    if (t == RDT::RDT_COUNT) return Status::NOTYPE;
    string data;
    Slice key{Key{id, t}};
    auto ok = db_->Get(ro_, trunk_, key, &data);
    if (ok.IsNotFound()) return Status::NOT_FOUND;
    if (!ok.ok()) return Status::DB_FAIL;
    object.swap(data);
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
