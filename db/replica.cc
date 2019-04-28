#include "replica.hpp"
#include "map.hpp"

namespace ron {

using namespace std;

template <typename Store>
const Uuid Replica<Store>::NOW_UUID{915334634030497792UL, 0};

//  L I F E C Y C L E

template <typename Store>
Status Replica<Store>::CreateReplica() {
    Store db;
    // Uuid branch_id{0, origin};
    // Uuid now = Uuid::Time(Uuid::Now(), 0);
    IFOK(db.Create(Uuid::NIL));

    Frame local_names = OneOp<Frame>(Uuid::NIL, LWW_FORM_UUID);
    IFOK(db.Write(Key{Uuid::NIL, LWW_FORM_UUID}, local_names));
    Uuid now0 = Uuid::Time(Uuid::Now(), ZERO);
    Frame local_yarn = OneOp<Frame>(now0, YARN_FORM_UUID);
    IFOK(db.Write(Key{now0, LOG_FORM_UUID}, local_yarn));
    OpMeta tip_meta{local_yarn.cursor(), OpMeta{}};
    Builder b;
    tip_meta.Save(b);
    IFOK(db.Write(Key{now0, META_FORM_UUID}, b.Release()));

    return Status::OK;
}

template <typename Store>
Status Replica<Store>::Open() {
    if (open()) {
        return Status::BAD_STATE.comment("already open");
    }

    Status ok = Store::OpenAll(branches_);
    if (!ok) {
        return ok;
    }

    Store& meta = GetBranch(Uuid::NIL);
    Frame meta_rec;
    ok = meta.Read(Key{}, meta_rec);
    if (!ok) {
        return ok;
    }
    if (meta_rec.empty()) {
        return Status::BAD_STATE.comment("no metadata?!");
    }
    Cursor mc{meta_rec};
    // if (!mc.has(2, UUID)) {
    //    return Status::DB_FAIL.comment("no timestamp found");
    //}
    // now_ = mc.uuid(2);

    return Status::OK;
}

template <typename Store>
Status Replica<Store>::CreateBranch(Uuid branch_id) {
    if (HasBranch(branch_id)) {
        return Status::BADARGS.comment("branch already exists");
    }
    const Store& meta = GetBranch(Uuid::NIL);
    Store new_branch{meta.db()};
    IFOK(new_branch.Create(branch_id));
    branches_.emplace(branch_id, new_branch);

    Records initials;

    Frame yarn_meta = OneOp<Frame>(branch_id, YARN_FORM_UUID);
    IFOK(new_branch.Write(Key{branch_id, LOG_RAW_FORM}, yarn_meta));

    Frame names = OneOp<Frame>(Uuid::NIL, LWW_FORM_UUID);
    IFOK(new_branch.Write(Key{Uuid::NIL, LWW_FORM_UUID}, names));

    return Status::OK;
}

template <typename Store>
Status Replica<Store>::WriteName(Uuid key, Uuid value, Uuid branch) {
    if (!HasBranch(branch)) return Status::NOT_FOUND.comment("branch unknown");
    Records w;
    Uuid id = Now(branch.origin());
    Key names_key{Uuid::NIL, LWW_RDT_FORM};
    Frame valop = OneOp<Frame>(id, Uuid::NIL, key, value);
    GetBranch(branch).Write(names_key, valop);
    //???!!!last_name_id_ = id;
    return id;
}

template <typename Store>
Status Replica<Store>::ReadNames(Names& names, Uuid branch) {
    if (!HasBranch(branch)) {
        return Status::NOT_FOUND.comment("no such branch");
    }
    Frame zeroobj;
    IFOK(GetObject(zeroobj, Uuid::NIL, LWW_FORM_UUID));
    Cursor c{zeroobj, false};
    while (c.Next()) {
        if (c.size() != 4) {
            continue;
        }
        if (!c.has(2, UUID) || !c.has(3, UUID)) {
            continue;
        }
        names[c.uuid(2)] = c.uuid(3);
    }
    return Status::OK;
}

template <typename Store>
Status Replica<Store>::ReadName(Uuid& id, Uuid name, Uuid branch) {
    return Status::NOT_IMPLEMENTED.comment("ReadName");
}

template <typename Store>
Uuid Replica<Store>::Now(Word origin) {
    if (origin == ZERO) {
        origin = home_.origin();
    }
    Word next = Uuid::HybridTime(time(nullptr));
    now_ = next > now_ ? next : now_.inc();
    return Uuid::Time(now_, origin);
}

template <typename Store>
Status Replica<Store>::GC() {
    return Status::OK;
}

template <typename Store>
Status Replica<Store>::Close() {
    if (!branches_.empty()) {
        GetBranch(Uuid::NIL).Close();
        branches_.clear();
    }
    return Status::OK;
}

template <typename Store>
Replica<Store>::~Replica() {
    Close();
}

//  O B J E C T  L O G S

template <typename Store>
Status Replica<Store>::Commit::FindOpMeta(OpMeta& meta, Uuid op_id) {
    // find head rec
    IFOK(FindChainHeadMeta(meta, op_id));
    if (meta.id == op_id) {
        return Status::OK;
    }
    // load object log
    Frame object_log;
    IFOK(FindObjectLog(object_log, meta.object));
    // seek to the head
    Cursor cur{object_log};
    while (cur.valid() && cur.id() != meta.id) {
        cur.Next();
    }
    if (!cur.valid()) {
        return Status::NOT_FOUND.comment("the chain head is not in the log?!");
    }
    // seek to the op
    while (cur.Next()) {
        if (cur.id().origin() != op_id.origin()) {
            continue;
        }
        if (cur.ref() != meta.id) {
            return Status::NOT_FOUND.comment("no op in the chain:" +
                                             op_id.str());
        }
        meta.Next(cur, meta);
        if (meta.id == op_id) {
            break;
        }
    }
    if (!cur.valid()) {
        if (op_id.value() == NEVER) {
            return Status::OK;  // a dirty trick to pick the yarn tip
        }
        return Status::NOT_FOUND.comment("no such op: " + op_id.str());
    }
    return Status::OK;
}

template <typename Store>
Status Replica<Store>::Commit::FindChainHeadMeta(OpMeta& meta, Uuid op_id) {
    Iterator i{join_};
    Status ok = i.SeekTo(Key{op_id, META_FORM_UUID}, true);
    if (!ok) {
        return ok;
    }
    Cursor metac{i.value()};
    if (!metac.valid()) {
        return Status::BAD_STATE.comment("unparseable meta record?!");
    }
    if (metac.id().origin() != op_id.origin() ||
        metac.ref() != META_FORM_UUID) {
        return Status::NOT_FOUND.comment("no such yarn?");
    }
    ok = meta.Load(metac);
    return ok;
}

template <typename Store>
Status Replica<Store>::Commit::FindYarnTipMeta(OpMeta& meta, Word yarn) {
    Uuid yarn_end{NEVER, yarn};
    return FindOpMeta(meta, yarn_end);
}

template <typename Store>
Status Replica<Store>::Commit::FindObjectLog(Frame& frame, Uuid id) {
    return join_.Read(Key{id, LOG_RAW_FORM}, frame);
}

//  E V E N T  Q U E R I E S

//  R E C E I V E S

template <typename Store>
Status Replica<Store>::See(Uuid timestamp) {
    if (timestamp.value() < now_) {
        return Status::OK;
    }
    now_ = timestamp.value();  // TODO(gritzko) concurrent access
    /*
    Builder now_record;
    now_record.AppendNewOp(Uuid::NIL, Uuid::NIL, now_);
    save.emplace_back(Key{}, now_record.Release());
    // TODO sanity/plausibility check
    // TODO move to Close()
    */
    return Status::OK;
}

template <typename Store>
Status Replica<Store>::Commit::SaveChainlet(Builder& to, OpMeta& meta,
                                            Cursor& from) {
    to.AppendOp(from);
    Status ok = from.Next();
    while (ok) {
        LOG('?', Key{from.id(), ZERO_RAW_FORM}, "...\n");
        if (from.id() == Uuid::COMMENT) {
        } else if (meta.is_next(from)) {
            IFOK(CheckEventSanity(from));
            meta.Next(from, meta);
            to.AppendOp(from);
        } else if (meta.is_check(from)) {
            ok = meta.Check(from);
            if (!ok) {
                return ok;
            }
        } else {
            break;
        }
        ok = from.Next();
    }
    if (ok == Status::ENDOFFRAME) {
        ok = Status::OK;
    }
    return ok;
}

template <typename Store>
Status Replica<Store>::Commit::CheckEventSanity(const Cursor& chain) {
    Uuid id = chain.id();
    Uuid ref_id = chain.ref();
    if (id.version() != TIME) {
        return Status::BAD_STATE.comment("not an event?!");
    }
    if (ref_id.version() == TIME) {  // edit object
        if (ref_id >= id) {
            return Status::CAUSEBREAK;
        }
    } else {  // create object
        if (ref_id.version() != NAME) {
            return Status::BAD_STATE.comment("ref is either TIME or NAME");
        }
        if (uuid2form(ref_id) == ZERO_RAW_FORM) {
            return Status::NOT_IMPLEMENTED.comment("create zero typed obj?");
        }
    }
    return Status::OK;
}

/** The key lifecycle method: accepts a new chain of ops, checks it
 * against existing ops, checks integrity/consistency/causality,
 * saves the chain, updates object state.
 * @param batch a WriteBatch for all the db writes
 * @param branch
 * @param chain a cursor positioned on the head of the chain;
 *              will be moved to the first non-chain op (or EOF) */
template <typename Store>
Status Replica<Store>::Commit::SaveChain(Builder&, Cursor& chain) {
    Status ok;
    Uuid id = chain.id();
    Uuid ref_id = chain.ref();

    // sanity checks
    IFOK(CheckEventSanity(chain));

    // find the last op on the yarn (the tip) and its metadata
    OpMeta tip_meta;
    Uuid& tip_id = tip_meta.id;
    ok = FindYarnTipMeta(tip_meta, id.origin());
    if (chain.ref() == YARN_FORM_UUID) {
        // actually, it is the first op on the yarn
        if (ok == Status::NOT_FOUND) {
            tip_meta = OpMeta{chain, OpMeta{}};
            ok = Status::OK;
        } else if (!ok) {
            return ok;
        } else if (id == tip_meta.id) {
            // TODO compare
            return Status::REPEAT;
        } else {
            return Status::CONFLICT.comment("can't merge a duplicate-id yarn");
        }
    } else {
        if (!ok) {
            return ok;
        }
        if (tip_id >= id) {
            return Status::REPEAT;
        }
    }

    // find the referenced op and its metadata (hash, object id)
    if (ref_id == tip_id) {  // the op refs the tip, we have the meta already
        tip_meta.Next(chain, tip_meta);
    } else if (ref_id.version() == NAME) {  // tree root, references nothing
        FORM form = uuid2form(ref_id);
        if (form == ERROR_NO_FORM) {
            return Status::NOT_IMPLEMENTED.comment("form unknown: " +
                                                   ref_id.str());
        }
        tip_meta = OpMeta{chain, tip_meta};
        // TODO check it is an RDT or yarn
    } else {  // ok, let's check the db
        // we enforce referential integrity but we
        // can't run datatype-specific checks here
        OpMeta ref_meta;
        IFOK(FindOpMeta(ref_meta, ref_id));
        tip_meta.Next(chain, ref_meta);
    }

    if (ref_id != tip_id) {
        Builder chain_record;
        tip_meta.Save(chain_record);
        IFOK(join_.Write(Key{id, META_FORM_UUID}, chain_record.Release()));
    }

    // walk/check the chainlet
    Uuid& obj_id = tip_meta.object;
    Builder chainlet;
    IFOK(SaveChainlet(chainlet, tip_meta, chain));
    IFOK(host_.See(tip_meta.id));  // implausible timestamps etc
    Frame data = chainlet.Release();
    IFOK(join_.Write(Key{obj_id, LOG_FORM_UUID}, data));
    IFOK(join_.Write(Key{obj_id, tip_meta.rdt}, data));

    return tip_meta.id;
}

template <typename Store>
Status Replica<Store>::GetFrame(Frame& object, Uuid id, Uuid rdt, Uuid branch) {
    if (!open()) {
        return Status::NOTOPEN;
    }
    if (!HasBranch(branch)) {
        return Status::NOT_FOUND.comment("branch unknown");
    }
    Store& store = GetBranch(branch);
    FORM t = uuid2form(rdt);
    if (t == ZERO_RAW_FORM) {
        return Status::NOTYPE;
    }
    Key key{id, rdt};
    return store.Read(key, object);
}

template <typename Store>
Status Replica<Store>::GetMap(Frame& result, Uuid id, Uuid map, Uuid branch) {
    if (!open()) {
        return Status::NOTOPEN;
    }
    Builder response;
    Builder query;
    query.AppendNewOp(id, map);
    query.EndChunk(QUERY);
    Cursor qc{query.data()};
    // Status ok = MapperQuery(response, qc, branch);
    MemStore mem;
    if (!HasBranch(branch)) {
        return Status::NOT_FOUND.comment("no branch " + branch.str());
    }

    // FIXME all wrong, see @cblp

    // Commit readonly{GetBranch(branch), mem};
    // Records devnull;
    // IFOK(ReceiveWrites(response, qc, readonly));
    // result = response.Release();
    return Status::OK;
}

template <typename Store>
Status Replica<Store>::Commit::QueryObject(Builder& response, Cursor& query) {
    Key key{query.id(), query.ref()};
    if (host_.mode_ & KEEP_STATES) {
        Frame f;
        IFOK(join_.Read(key, f));
        Cursor c{f};
        response.AppendAll(c);
        query.Next();
        return Status::OK;
    } else if (host_.mode_ & KEEP_OBJECT_LOGS) {
        Key logkey{query.id(), LOG_FORM_UUID};
        Frame log;
        join_.Read(logkey, log);
        Status ok = ObjectLogIntoState(response, log);
        query.Next();
        return ok;
    } else {
        return Status::NOT_FOUND.comment("unsupported form");
    }
}

template <typename Store>
Status Replica<Store>::Commit::QueryYarnVV(Builder& response, Cursor& query) {
    return Status::NOT_IMPLEMENTED.comment("QueryYarnVV");
}

template <typename Store>
Status Replica<Store>::Commit::QueryYarn(Builder& response, Cursor& query) {
    return Status::NOT_IMPLEMENTED.comment("YarnQuery");
}

template <typename Store>
Status Replica<Store>::Commit::QueryObjectLog(Builder& response,
                                              Cursor& query) {
    Uuid id = query.id();
    query.Next();
    OpMeta meta;
    IFOK(FindOpMeta(meta, id));
    Key logkey{meta.object, meta.rdt};
    if (id == meta.object) {
        Frame obj;
        IFOK(join_.Read(logkey, obj));
        Cursor objc{obj};
        response.AppendAll(objc);
        return Status::OK;
    }
    Frame log;
    IFOK(join_.Read(logkey, log));
    // version | since | segment
    Cursor c{log};
    do {
        response.AppendOp(c);
    } while (c.id() != id && c.Next());
    return c.id() == id ? Status::OK
                        : Status::NOT_FOUND.comment("no such op in the log");
}

template <class Cursor>
TERM look_ahead(Cursor c) {
    while (c.Next() && c.term() == REDUCED) {
    }
    return c.valid() ? c.term() : REDUCED;
}

template <class Cursor>
inline TERM peek_term(const Cursor& c) {
    if (c.term() != REDUCED) {
        return c.term();
    }
    return look_ahead(c);
}

template <class Cursor>
inline bool is_query(const Cursor& c) {
    return peek_term(c) == QUERY;
}

template <typename Store>
Status Replica<Store>::Commit::WriteNewEvents(Builder& resp, Cursor& uc) {
    Builder stamp;
    Uuid now = host_.Now(yarn_id());
    constexpr uint64_t MAXSEQ = 1 << 30;
    while (uc.valid() && uc.id().origin().payload() == 0) {
        now.inc();
        if (uc.id().origin().payload() != 0) {
            return Status::BAD_STATE.comment("stamped already");
        }
        Uuid id{now.value()._64, now.origin()};
        Uuid ref{uc.ref()};
        if (ref.origin() == 0 && ref.value() < MAXSEQ) {
            ref = Uuid{now.value()._64 + uc.ref().value()._64, now.origin()};
        }
        stamp.AppendAmendedOp(uc, RAW, id, ref);
        uc.Next();
    }
    Cursor stamped{stamp.data()};
    Status ok;
    while (stamped.valid() && ok) {
        ok = SaveChain(resp, stamped);
    }
    return ok;
}

template <typename Store>
Status Replica<Store>::Commit::ReceiveWrites(Builder& resp, Cursor& c) {
    // NOTE all methods that take a Cursor MUST consume their ops
    // NOTE all incoming chunks MUST specify a form unless they are events
    if (c.ref().version() == TIME) {
        return WriteNewEvents(resp, c);
    }
    if (c.ref().version() != NAME) {
        return Status::BADARGS.comment("unrecognized write pattern");
    }
    FORM form = uuid2form(c.ref());
    TERM term = peek_term(c);
    switch (form) {  // TODO mark forms as rdt/meta/map
        case LWW_RDT_FORM:
        case RGA_RDT_FORM:
        case MX_RDT_FORM:
        case YARN_RAW_FORM:
            return WriteNewEvents(resp, c);
        case TXT_MAP_FORM:
            return host_.txt_.Write(resp, c, *this);
        default:
            return Status::NOT_IMPLEMENTED.comment("unknown form: " +
                                                   c.ref().str());
    }
}

template <typename Store>
Status Replica<Store>::Commit::ReceiveQuery(Builder& response, Cursor& c) {
    FORM form = uuid2form(c.ref());
    switch (form) {
        case LWW_RDT_FORM:
        case RGA_RDT_FORM:
        case MX_RDT_FORM:
        case YARN_RAW_FORM:
            return QueryObject(response, c);
        case LOG_RAW_FORM:
            return QueryObjectLog(response, c);
        case TXT_MAP_FORM:
            return host_.txt_.Read(response, c, *this);
        default:
            return Status::NOT_IMPLEMENTED.comment("unknown query form: " +
                                                   c.ref().str());
    }
    c.Next();
}

template <typename Store>
Status Replica<Store>::Commit::Save() {
    Records save;
    if (mem_.Release(save)) {
        // Frames are applied transactionally, all or nothing.
        // We saw no errors => we may save the changes.
        return main_.Write(save);
    } else {
        return Status::OK;
    }
}

template <typename Store>
Status Replica<Store>::Receive(Builder& resp, Cursor& c, Uuid branch_id) {
    Status ok = Status::OK;
    if (!open()) {
        return Status::NOTOPEN;
    }
    if (!HasBranch(branch_id)) {
        // TODO 1 such check
        return Status::NOT_FOUND.comment("unknown branch");
    }
    Store branch_store = GetBranch(branch_id);
    Commit commit{*this, branch_store, branch_id.origin()};

    while (c.valid() && ok) {
        if (c.id() == Uuid::COMMENT) {
            ok = c.Next();
        } else if (c.term() == QUERY) {
            ok = commit.ReceiveQuery(resp, c);
        } else if (c.id().version() == TIME && c.id().origin().payload() != 0) {
            ok = commit.SaveChain(resp, c);
        } else if (c.id().version() == TIME) {
            ok = commit.ReceiveWrites(resp, c);
        } else if (c.id().version() == NAME) {
            ok = Status::BADVALUE.comment("a runaway annotation");
        } else if (c.id().version() == HASH) {
            ok = Status::NOT_IMPLEMENTED.comment("no blob support yet");
        } else {
            ok = Status::NOT_IMPLEMENTED.comment("unrecognized op pattern");
        }
    }

    if (ok) {
        ok = commit.Save();
    }
    // the response is rendered already

    return ok;
}

/*template<typename Frame>
template<class FrameB>
Status Replica<Store>::Recv (Builder& response, const FrameB& input) {
    // scan, check, convert, split
    Batch to_process;
    Builder next;
    //typedef typename FrameB::Cursor CursorB;
    auto cur = input.cursor();
    do {
        if (!next.empty() && cur.op().term()!=TERM::REDUCED) {
            to_process.push_back(next.Release());
            next = Builder{};
        }
        next.AddOp(cur.op(), input.data());
    } while (cur.Next());

    return Recv(response, to_process);
}*/

template class Replica<RocksDBStore<TextFrame>>;

}  // namespace ron
