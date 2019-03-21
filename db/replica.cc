#include "replica.hpp"
#include "map.hpp"

namespace ron {

using namespace std;

template <typename Frame>
const Uuid Replica<Frame>::NOW_UUID{915334634030497792UL, 0};

//  L I F E C Y C L E

template <typename Frame>
void Replica<Frame>::cerr_batch(Records& save) {
    for (int i = 0; i < save.size(); ++i) {
        cerr << form2uuid(save[i].first.form()).str() << '\t';
        cerr << save[i].first.id().str() << '\t';
        cerr << save[i].second.data();
    }
}

template <typename Frame>
Status Replica<Frame>::Create(std::string home, Word origin) {
    if (db_.open()) {
        return Status::BAD_STATE.comment("db open already");
    }
    Status ok = db_.Create(home);
    if (!ok) {
        return ok;
    }
    Records initials;

    // 1. the yarn object
    Builder yarn_obj_builder;
    now_ = Uuid{0, origin};
    Uuid now = Now();
    yarn_obj_builder.AppendNewOp(TERM::RAW, now, YARN_FORM_UUID);
    Frame yarn_object{yarn_obj_builder.Release()};
    initials.emplace_back(Key{now, YARN_RAW_FORM}, yarn_object);
    // TODO(gritzko) public yarn metadata stored as key-value pairs (e.g. pub
    // key, author name)

    // 2. the "now" record
    See(now, initials);

    // 3. chain meta record
    Cursor mc{yarn_object};
    OpMeta meta{mc};
    Builder meta_record;
    meta.Save(meta_record);
    initials.emplace_back(Key{now, META_META_FORM}, meta_record.Release());

    cerr_batch(initials);

    return db_.Write(initials);
}

template <typename Frame>
Status Replica<Frame>::Open(std::string home) {
    if (db_.open()) {
        return Status::BAD_STATE.comment("already open");
    }

    Status ok = db_.Open(home);
    if (!ok) {
        return ok;
    }

    Frame meta;
    ok = db_.Get(Key{}, meta);
    Cursor mc{meta};
    if (!mc.has(2, UUID)) {
        return Status::DB_FAIL.comment("no timestamp found");
    }
    now_ = mc.uuid(2);

    return Status::OK;
}

template <typename Frame>
Uuid Replica<Frame>::Now() {
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
    if (db_.open()) {
        return db_.Close();
    }
    return Status::OK;
}

template <typename Frame>
Replica<Frame>::~Replica() {
    Close();
}

//  O B J E C T  L O G S

template <class Frame>
Status Replica<Frame>::FindOpMeta(OpMeta& meta, Uuid op_id, Uuid branch) {
    // find head rec
    OpMeta head;
    Status ok = FindHeadMeta(head, op_id);
    if (!ok) {
        return ok;
    }
    // load object log
    Frame object_log;
    ok = GetObjectLog(object_log, head.object, branch);
    if (!ok) {
        return ok;
    }
    // seek to the head
    Cursor cur{object_log};
    while (cur.valid() && cur.id() != head.id) {
        cur.Next();
    }
    if (!cur.valid()) {
        return Status::NOT_FOUND.comment("the head is not in the log?!");
    }
    // seek to the op
    while (cur.id() != op_id && cur.Next()) {
        head.Next(cur);
    }
    if (!cur.valid()) {
        return Status::NOT_FOUND.comment("no such op");
    }
    return Status::OK;
}

template <class Frame>
Status Replica<Frame>::FindHeadMeta(OpMeta& meta, Uuid op_id, Uuid branch) {
    StoreIterator i{db_};
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
        return Status::CAUSEBREAK.comment("no such yarn?");
    }
    ok = meta.Load(metac);
    return ok;
}

template <class Frame>
OpMeta& Replica<Frame>::GetYarnTipMeta(Word yarn, Uuid branch) {
    auto i = tips_.find(yarn);
    if (i != tips_.end()) {
        return i->second;
    }
    // StoreIterator s{db_};
    Uuid yarn_end{NEVER, yarn};
    OpMeta meta{};
    Status ok = FindOpMeta(meta, yarn_end, branch);
    auto ib = tips_.emplace(std::make_pair(yarn, meta));
    if (ok == Status::NOT_FOUND) {
    }
    return ib.first->second;
}

//  E V E N T  Q U E R I E S

template <class Frame>
Status Replica<Frame>::FindYarn(StoreIterator& i, Uuid start, Word branch) {
    i = StoreIterator{db_};
    Status ok = i.SeekTo(Key{});  // ...
    return ok;
}

//  R E C E I V E S

template <class Frame>
Status Replica<Frame>::See(Uuid timestamp, Records& save) {
    if (timestamp < now_) return Status::OK;
    now_ = Uuid{timestamp.value(), now_.origin()};  // TODO concurrent access
    Builder now_record;
    now_record.AppendNewOp(RAW, Uuid::NIL, Uuid::NIL, now_);
    save.emplace_back(Key{}, now_record.Release());
    // TODO sanity/plausibility check
    return Status::OK;
}

template <class Frame>
Status Replica<Frame>::WriteNewYarn(Records& save, Cursor& yroot, Uuid branch) {
    Uuid id = yroot.id();
    Word yarn_id = id.origin();
    if (id.version() != TIME) {
        return Status::BAD_STATE.comment("not an event?!");
    }
    OpMeta checkmeta = GetYarnTipMeta(yarn_id, branch);
    if (!checkmeta.id.zero()) {
        return Status::CAUSEBREAK.comment("the yarn already exists: " +
                                          yarn_id.str());
    }
    Builder crb;
    OpMeta meta{yroot};
    meta.Save(crb);
    Frame meta_record = crb.Release();
    save.push_back(Record{Key{id, META_FORM_UUID}, meta_record});
    tips_[yarn_id] = meta;
    // potentially, may use ReadChainlet if yarn becomes an RDT
    yroot.Next();
    return Status::OK;
}

template <class Frame>
Status Replica<Frame>::ReadChainlet(Builder& to, OpMeta& meta, Cursor& from) {
    meta.Next(from);
    to.AppendOp(from);
    Status ok = from.Next();
    // TODO(gritzko) sanity checks
    while (ok) {
        if (meta.is_next(from)) {
            meta.Next(from);
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

template <class Frame>
Status Replica<Frame>::WriteNewObject(Records& save, Cursor& root,
                                      Uuid branch) {
    assert(root.ref().version() == NAME);
    Uuid id = root.id();
    Uuid ref = root.ref();
    if (id.version() != TIME) {
        return Status::BAD_STATE.comment("not an event?!");
    }
    if (ref.version() != NAME) {
        return Status::BAD_STATE.comment("expect an RDT id");
    }
    OpMeta tip_meta = GetYarnTipMeta(id.origin());
    if (tip_meta.id.zero()) {
        return Status::CAUSEBREAK.comment("yarn unknown: " + ref.str());
    }
    Uuid& prev = tip_meta.id;
    Builder chainlet;
    Status ok = ReadChainlet(chainlet, tip_meta, root);
    if (!ok) {
        return ok;
    }
    ok = See(tip_meta.id, save);
    if (!ok) {
        return ok;
    }

    Frame data = chainlet.Release();
    save.push_back(Record{Key{id, LOG_FORM_UUID}, data});
    save.push_back(Record{Key{id, tip_meta.rdt}, data});

    return ok;
}

/** The key lifecycle method: accepts a new chain of ops, checks it
 * against existing ops, checks integrity/consistency/causality,
 * saves the chain, updates object state.
 * @param batch a WriteBatch for all the db writes
 * @param branch
 * @param chain a cursor positioned on the head of the chain;
 *              will be moved to the first non-chain op (or EOF) */
template <class Frame>
Status Replica<Frame>::WriteNewChain(Records& save, Cursor& chain,
                                     Uuid branch) {
    // TODO(gritzko) sanity checks
    Status ok;
    Uuid id = chain.id();
    if (id.version() != TIME) {
        return Status::BAD_STATE.comment("not an event?!");
    }
    Uuid ref = chain.ref();
    if (ref.version() == TIME) {  // edit object
        if (ref >= id) {
            return Status::CAUSEBREAK;
        }
    } else {  // create object
        if (ref.version() != NAME) {
            return Status::BAD_STATE.comment(
                "an event's ref is either TIME or NAME");
        }
        if (uuid2form(ref) == ZERO_RAW_FORM) {
            return Status::NOT_IMPLEMENTED;
        }
    }
    // may need to read tail meta
    // PREV: fetch the meta on the prev op
    OpMeta tip_meta = GetYarnTipMeta(ref.origin());
    if (tip_meta.id.zero()) {
        return Status::CAUSEBREAK.comment("yarn unknown: " + ref.str());
    }
    Uuid& prev = tip_meta.id;
    // REF: fetch the meta on the referenced op (get the object)
    OpMeta ref_meta;
    if (ref == prev) {
        ref_meta = tip_meta;
    } else if (ref.version() == NAME) {
        ref_meta = OpMeta{chain, tip_meta};
    } else {
        // we enforce referential integrity
        // but can't run datatype-specific checks, so reducers are on their own
        // here
        ok = FindOpMeta(ref_meta, id);
        if (!ok) {
            return ok;
        }
    }
    Uuid& obj = ref_meta.object;
    if (ref != prev) {
        Frame chain_record;
        // @head-id :yarn   :obj+id 'HASH?' (the hash is very optional here)
        // @head-id :yarn   :ref-id 'HASH' (cross-yarn hashes are difficult to
        // recalculate)
        save.push_back(Record{Key{id, META_FORM_UUID}, chain_record});
    }
    // walk/check the chainlet
    Builder chainlet;
    ok = ReadChainlet(chainlet, tip_meta, chain);
    if (!ok) {
        return ok;
    }
    ok = See(tip_meta.id, save);  // implausible timestamps etc
    if (!ok) {
        return ok;
    }

    Frame data = chainlet.Release();
    save.push_back(Record{Key{obj, LOG_FORM_UUID}, data});
    save.push_back(Record{Key{obj, tip_meta.rdt}, data});

    return ok;
}

template <typename Frame>
Status Replica<Frame>::GetFrame(Frame& object, Uuid id, Uuid rdt, Uuid branch) {
    if (!db_.open()) {
        return Status::NOTOPEN;
    }
    FORM t = uuid2form(rdt);
    if (t == ZERO_RAW_FORM) return Status::NOTYPE;
    Key key{id, rdt};
    return db_.Get(key, object, branch);
}

template <typename Frame>
Status Replica<Frame>::GetMap(Frame& result, Uuid id, Uuid map, Uuid branch) {
    if (!db_.open()) {
        return Status::NOTOPEN;
    }
    Builder response;
    Builder query;
    query.AppendNewOp(QUERY, id, map);
    Cursor qc{query.data()};
    // Status ok = MapperQuery(response, qc, branch);
    Records devnull;
    Status ok = DispatchRecv(response, devnull, qc, branch);
    if (ok) {
        result = response.Release();
    }
    return ok;
}

template <typename Frame>
Status Replica<Frame>::ObjectQuery(Builder& response, Cursor& query,
                                   Uuid branch) {
    if (!db_.open()) {
        return Status::NOTOPEN;
    }
    Key key{query.id(), query.ref()};
    if (mode_ & KEEP_STATES) {
        return db_.Read(key, response, branch);
    } else if (mode_ & KEEP_OBJECT_LOGS) {
        Key logkey{query.id(), LOG_FORM_UUID};
        Builder olog;
        db_.Read(logkey, olog, branch);
        Frame log = olog.Release();
        Status ok = ObjectLogIntoState(response, log);
        return ok;
    } else {
        return Status::NOT_FOUND.comment("unsupported form");
    }
}

template <typename Frame>
Status Replica<Frame>::YarnQuery(Builder& response, Cursor& query,
                                 Uuid branch) {
    return Status::NOT_IMPLEMENTED;
}

template <typename Frame>
Status Replica<Frame>::ObjectLogQuery(Builder& response, Cursor& query,
                                      Uuid branch) {
    Uuid id = query.id();
    OpMeta meta;
    Status ok = FindOpMeta(meta, id, branch);
    if (!ok) {
        return ok;
    }
    Key logkey{meta.object, meta.rdt};
    if (id == meta.object) {
        return db_.Read(logkey, response, branch);
    }
    Frame log;
    db_.Get(logkey, log, branch);
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
    while (c.Next() && c.term() == REDUCED)
        ;
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

template <typename Frame>
Status Replica<Frame>::DispatchRecv(Builder& resp, Records& save, Cursor& c,
                                    Uuid b) {
    // NOTE all methods that take a Cursor MUST consume their ops
    // NOTE all incoming chunks MUST specify a form unless they are events
    FORM form = uuid2form(c.ref());
    TERM term = peek_term(c);
    switch (form) {
        case LWW_RDT_FORM:
        case RGA_RDT_FORM:
        case MX_RDT_FORM:
            switch (term) {
                case QUERY:
                    return ObjectQuery(resp, c, b);
                case RAW:
                    return WriteNewObject(save, c, b);
                default:
                    return Status::NOT_IMPLEMENTED;
            }
            break;
        case YARN_RAW_FORM:
            switch (term) {
                case QUERY:
                    return YarnQuery(resp, c, b);
                case RAW:
                    return WriteNewYarn(save, c, b);
                default:
                    return Status::NOT_IMPLEMENTED;
            }
            break;
        case TXT_MAP_FORM:
            switch (term) {
                case QUERY:
                    return txt_.Read(resp, c, b);
                case HEADER:
                    return txt_.Write(save, c, b);
                default:
                    return Status::NOT_IMPLEMENTED;
            }
        default:
            return Status::NOT_IMPLEMENTED.comment("unknown form: " +
                                                   c.ref().str());
    }
}

template <typename Frame>
Status Replica<Frame>::Recv(Builder& resp, Cursor& c, Uuid branch) {
    Records save;
    Status ok = Status::OK;

    while (c.valid() && ok) {
        const Uuid& id = c.id();
        if (c.id() == COMMENT_UUID) {
            ok = c.Next();
            continue;
        }
        if (c.id().version() == HASH) {
            return Status::NOT_IMPLEMENTED.comment(
                "blobs are not supported yet");
        }
        if (c.ref().version() == TIME) {
            ok = WriteNewChain(save, c, branch);
            continue;
        }
        if (c.ref().version() != NAME) {
            ok = Status::NOT_IMPLEMENTED.comment(
                "expecting events,queries or writes");
        }

        ok = DispatchRecv(resp, save, c, branch);
    }

    if (ok && !save.empty()) {
        cerr_batch(save);
        ok = db_.Write(save, branch);
    }
    // the response is rendered

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
            to_process.push_back(next.Release());
            next = Builder{};
        }
        next.AddOp(cur.op(), input.data());
    } while (cur.Next());

    return Recv(response, to_process);
}*/

template class CSVMapper<TextFrame>;
template class Replica<TextFrame>;

}  // namespace ron
