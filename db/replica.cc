#include "db/replica.hpp"
#include "db/map/map.hpp"
#include "db/merge_ops.hpp"

namespace ron {

using namespace std;
using namespace rocksdb;
using Slice = ron::Slice;

#define RSLICE(str) \
    rocksdb::Slice { (const char*)str.data(), str.size() }

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
    db_->Put(wo(), nil_key(), RSLICE(nowrec.data()));

    Close();
    return Open(home);
}

template <typename Frame>
Status Replica<Frame>::Open(std::string home) {
    if (db_) return Status::BAD_STATE.comment("db open already");

    rocksdb::Options options;
    options.create_if_missing = false;
    options.error_if_exists = false;
    options.max_total_wal_size = UINT64_MAX;
    options.WAL_size_limit_MB = 1UL << 30U;
    options.WAL_ttl_seconds = UINT64_MAX;

    typedef rocksdb::ColumnFamilyDescriptor CFD;
    typedef rocksdb::ColumnFamilyHandle CFH;
    vector<CFD> families;
    vector<CFH*> handles;
    families.push_back(CFD{kDefaultColumnFamilyName, CFOptions()});
    // TODO branches

    auto ok = rocksdb::DB::Open(options, home, families, &handles, &db_);
    if (!ok.ok()) {
        return status(ok);
    }

    auto i = handles.begin();
    trunk_ = *i;

    string meta;
    ok = db_->Get(ro_, nil_key(), &meta);
    Cursor mc{Slice{meta}};
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

template <class Frame>
rocksdb::Iterator* Replica<Frame>::FindChain(Uuid target_id) {
    if (!open()) return nullptr;
    Key key{target_id, CHAIN_RDT};
    rocksdb::Iterator* i{db_->NewIterator(ro_, trunk_)};
    i->SeekForPrev(key);
    while (i->Valid() && (key = Key{i->key()}).rdt() != CHAIN_RDT) i->Prev();
    if (!i->Valid() || key.id().origin() != target_id.origin()) {
        delete i;
        i = nullptr;
    }
    return i;
}

template <class Frame>
rocksdb::Iterator* Replica<Frame>::FindYarn(Uuid target_id) {
    if (!open()) return nullptr;
    Key key{target_id, CHAIN_RDT};
    rocksdb::Iterator* i{db_->NewIterator(ro_, trunk_)};
    i->Seek(key);
    while (i->Valid() && (key = Key{i->key()}).rdt() != CHAIN_RDT) i->Next();
    if (!i->Valid() || key.id().origin() != target_id.origin()) {
        delete i;
        i = nullptr;
    }
    return i;
}

template <class Frame>
Status Replica<Frame>::FindOpMeta(OpMeta& meta, Uuid target_id) {
    static_assert(RDT::META_RDT + 1 == RDT::CHAIN_RDT,
                  "ensure records go in this exact order");
    string chain_data, meta_data;

    // 1. find chain
    std::unique_ptr<rocksdb::Iterator> i{FindChain(target_id)};
    if (!i) return Status::NOT_FOUND.comment("no such chain");
    Key chain_key{i->key()};
    chain_data = string{i->value().data(), i->value().size()};

    // 2. find chain meta
    i->Prev();
    if (!i->Valid()) return Status::BAD_STATE.comment("no meta record");
    Key meta_key{i->key()};
    if (meta_key.id() != chain_key.id() || meta_key.rdt() != RDT::META_RDT)
        return Status::BAD_STATE.comment("bad meta record");
    meta_data = string{i->value().data(), i->value().size()};

    // 3. scroll the chain
    Cursor metac{meta_data};
    meta = OpMeta{chain_key.id()};
    Status ok = meta.ScanAnnotations<Cursor>(metac);
    Cursor chainc{chain_data};
    while (meta.id != target_id && chainc.Next()) {
        meta = OpMeta{chainc, meta, meta};
    }
    if (!chainc.valid() && target_id.value() != NEVER)
        return Status::NOT_FOUND.comment("no such op in the chain");

    return ok;
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
    Uuid id = chain.id();
    if (id.version() != TIME)
        return Status::BAD_STATE.comment("not an event?!");

    Uuid ref = chain.ref();
    if (ref.version() == TIME) {
        if (ref >= id) return Status::CAUSEBREAK;
    } else {
        if (ref.version() != NAME)
            return Status::BAD_STATE.comment("ref is either TIME or NAME");
        if (uuid2rdt(ref) == RDT_COUNT) return Status::NOT_IMPLEMENTED;
    }

    // PREV: fetch the meta on the prev op (likely, chain)
    auto ti = tips_.find(id.origin());
    if (ti == tips_.end()) {
        auto ib = tips_.insert(tipmap_t::value_type{id.origin(), OpMeta{}});
        ti = ib.first;
        if (!FindTipMeta(ti->second, id.origin()))
            ti->second = OpMeta(id.origin(), SHA2{});  // TODO
    }
    // the tip should stay in the cache, so we do in-place writes
    OpMeta& tip = ti->second;

    OpMeta refd{tip};
    // REF: fetch the meta on the referenced op (get the object)
    if (ref != tip.id) {
        if (ref.version() == NAME) {  // new object
            refd = OpMeta{id, ref};
        } else {
            Status ok = FindOpMeta(refd, ref);
            if (!ok) return ok;
        }
    }

    // START OUR CHAIN-LET
    Builder chainlet;
    tip = OpMeta{chain, tip, refd};
    if (tip.chain_id().zero())
        return Status::BAD_STATE.comment("cant find chain");
    if (tip.object.zero()) return Status::BAD_STATE.comment("cant find object");

    chainlet.AppendOp(chain);
    if (tip.head()) {
        Builder annos;
        tip.AppendAnnos(annos);
        batch.Merge(trunk_, Key{tip.chain_id(), META_RDT}, annos.data());
    }
    See(id);

    // TODO chain comparator: sort annos just after the op!!!!

    // walk/check the chainlet
    while (chain.Next()) {
        if (chain.id().version() == NAME) {
            Status ok = tip.ReadAnnotation(chain);
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
    batch.Merge(trunk_, Key{tip.chain_id(), CHAIN_RDT}, data);  // TODO branches
    batch.Merge(trunk_, Key{tip.object, rdt}, data);

    return Status::OK;
}

template <typename Frame>
Status Replica<Frame>::Get(Frame& object, Uuid id, Uuid rdt, Uuid branch) {
    if (db_ == nullptr) return Status::NOTOPEN;
    RDT t = uuid2rdt(rdt);
    if (t == RDT::RDT_COUNT) return Status::NOTYPE;
    string data;
    Key k{id, t};
    rocksdb::Slice key{k};
    auto ok = db_->Get(ro_, trunk_, key, &data);
    if (ok.IsNotFound()) return Status::NOT_FOUND;
    if (!ok.ok()) return status(ok);
    object.swap(data);
    return Status::OK;
}

template <typename Frame>
Status Replica<Frame>::GetMap(Frame& result, Uuid id, Uuid map, Uuid branch) {
    if (db_ == nullptr) return Status::NOTOPEN;
    Builder qb;
    qb.AppendNewOp(QUERY, map, id);
    Cursor query{qb.data()};
    MasterMapper<Frame> mapper{this};
    Builder re;
    Status ok = mapper.Map(re, query);
    result = re.frame();
    return ok;
}

template <typename Frame>
Status Replica<Frame>::ReceiveObjectQuery(Builder& response, Uuid object_store,
                                          Cursor& query) {
    if (db_ == nullptr) return Status::NOTOPEN;
    Uuid id = query.id();
    Uuid ref = query.ref();
    RDT t = uuid2rdt(ref);
    if (t == RDT::RDT_COUNT) return Status::NOTYPE;
    string data;
    Key k{id, t};
    rocksdb::Slice key{k};
    auto ok = db_->Get(ro_, trunk_, key, &data);
    if (ok.IsNotFound()) return Status::NOT_FOUND;
    if (!ok.ok()) return Status::DB_FAIL.comment(ok.ToString());
    response.AppendFrame(Frame{data});
    query.Next();
    return Status::OK;
}

template <typename Frame>
Status Replica<Frame>::ReceiveMapQuery(Builder& response, Uuid object_store,
                                       Cursor& query) {
    MasterMapper<Frame> mapper{this};
    Status ok = mapper.Map(response, query);
    query.Next();
    return ok;
}

template <typename Frame>
Status Replica<Frame>::Receive(Builder& response, Uuid branch, Cursor& c) {
    rocksdb::WriteBatch batch;
    Status ok = Status::OK;
    while (c.valid() && ok) {
        const Uuid& id = c.id();
        if (c.id() == COMMENT_UUID) {
            c.Next();
        } else if (c.term() == QUERY) {
            switch (id.version()) {
                case TIME:
                    ok = ReceiveQuery(response, branch, c);
                    break;
                case NAME:
                case DERIVED:
                    ok = ReceiveMapQuery(response, branch, c);
                    break;
                default:
                    ok = Status::NOT_IMPLEMENTED.comment("TODO: blobs");
            }
        } else {
            switch (id.version()) {
                case TIME:
                    ok = ReceiveChain(batch, branch, c);
                    break;
                case NAME:
                    ok = ReceiveCheck(branch, c);
                    break;
                case DERIVED:
                    ok = ReceiveMap(batch, branch, c);
                    break;
                default:
                    ok = Status::NOT_IMPLEMENTED.comment("TODO: blobs");
            }
        }
    }

    if (ok && batch.Count() > 0) {
        auto dbok = db().Write(wo(), &batch);
        if (!dbok.ok()) ok = Status::DB_FAIL.comment(dbok.ToString());
    }

    return ok;
}

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
