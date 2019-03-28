#include <fcntl.h>
#include <gflags/gflags.h>
#include <unistd.h>
#include <ctime>
#include <unordered_map>
#include "../rdt/rdt.hpp"
#include "../ron/ron.hpp"
#include "replica.hpp"
#include "rocksdb/db.h"

using namespace ron;
using namespace std;

typedef TextFrame Frame;
typedef Replica<Frame> RonReplica;
typedef Frame::Builder Builder;
typedef Frame::Cursor Cursor;
typedef vector<Frame> Frames;
using StoreIterator = typename RonReplica::Store::Iterator;

// const Uuid COMMENT_UUID{1134907106097364992UL, 0};
const Uuid OUT_UUID{935024688260710400UL, 0};
const Uuid IN_UUID{824721681762222080UL, 0};
const Uuid TEST_UUID{526483344616062976UL, 0};

DEFINE_bool(create, false, "create a new replica");
DEFINE_string(feed, "", "feed a RON frame");
DEFINE_string(write, "", "write a RON frame (new ops)");
DEFINE_string(query, "", "submit a RON query (e.g. @chain:1gPH5o+gritzko?)");
DEFINE_string(get, "",
              "get a RON frame, e.g. --get @1gPH5o+gritzko:lww or "
              "1gPH5o+gritzko.lww or 1gPH5o+gritzko)");
DEFINE_bool(now, false, "print the current time(stamp)");
DEFINE_string(hash, "", "Merkle-hash a causally ordered frame");
DEFINE_string(dump, "", "dump the db (e.g. --dump meta or --dump -)");
DEFINE_string(store, "", "db store dir");
DEFINE_string(test, "", "run a test case (the test data stays in the db!)");
DEFINE_bool(
    clean, false,
    "clean the result of metadata (e.g. --get @csv:1h8GbW+gYpLcnUnF6 --clean)");
DEFINE_bool(h, false, "Show help");
DECLARE_bool(help);
DECLARE_string(helpmatch);

/**

    swarmdb test 'test/rdt/rga.ron'
    swarmdb mount @todo$gritzko:txt as todo.md
    swarmdb mount list
    swarmdb peer ws://1.2.3.4/lj3Fhtc43
    swarmdb see todo.md, push;

 */

Status CommandHashFrame(const string& filename);
Status CommandWriteNewFrame(RonReplica& replica, const string& filename);
Status CommandGetFrame(RonReplica& replica, const string& name);
Status CommandQuery(RonReplica& replica, const string& name);
Status CommandDump(RonReplica& replica, const string& prefix);
Status CommandTest(RonReplica& replica, const string& file);

string basename(string const& pathname);
Status rm_dir(string path);
bool file_exists(const std::string& name);
Status CompareFrames(const Frame& a, const Frame& b);

Status RunCommands() {
    RonReplica replica{};
    Status ok;
    string store = FLAGS_store.empty() ? ".swarmdb" : FLAGS_store;
    bool rm_on_exit{0};
    if (!FLAGS_test.empty() && FLAGS_store.empty()) {
        store = "test_" + basename(FLAGS_test);
        if (file_exists(store)) {
            ok = rm_dir(store);
            if (!ok) return ok;
        }
        rm_on_exit = true;
    }

    if (FLAGS_create || !FLAGS_test.empty()) {
        ok = replica.Create(store);
    } else {
        ok = replica.Open(store);
    }
    if (!ok) return ok;

    if (FLAGS_now) {
        Uuid now = replica.Now();
        cout << now.str() << endl;
    } else if (!FLAGS_hash.empty()) {
        ok = CommandHashFrame(FLAGS_hash);
    } else if (!FLAGS_query.empty()) {
        ok = CommandQuery(replica, FLAGS_query);
    } else if (!FLAGS_get.empty()) {
        ok = CommandGetFrame(replica, FLAGS_get);
    } else if (!FLAGS_write.empty()) {
        ok = CommandWriteNewFrame(replica, FLAGS_write);
    } else if (!FLAGS_dump.empty()) {
        ok = CommandDump(replica, FLAGS_dump);
    } else if (!FLAGS_test.empty()) {
        ok = CommandTest(replica, FLAGS_test);
    } else {
    }

    if (replica.open()) replica.Close();
    if (rm_on_exit && ok) ok = rm_dir(store);

    return ok;
}

int main(int argn, char** args) {
    gflags::SetUsageMessage("swarmdb -- a syncable embedded RON database");
    gflags::ParseCommandLineNonHelpFlags(&argn, &args, true);
    if (FLAGS_help || FLAGS_h) {
        FLAGS_help = false;
        FLAGS_helpmatch = "db";
    }
    gflags::HandleCommandLineHelpFlags();

    Status ok = RunCommands();

    if (!ok) cerr << "error\t" << ok.str() << '\n';

    return ok ? 0 : -1;
}

Status LoadFrame(Frame& target, int fd) {
    string tmp;
    constexpr size_t BLOCK = 1 << 12;
    char buf[BLOCK];
    ssize_t s;
    while ((s = read(fd, buf, BLOCK)) > 0) tmp.append(buf, size_t(s));
    if (s < 0) return Status::IOFAIL;
    target = Frame{tmp};
    return Status::OK;
}

Status LoadFrame(Frame& target, const string& filename) {
    int fd = filename.empty() ? STDIN_FILENO : open(filename.c_str(), O_RDONLY);
    if (fd < 0) return Status::IOFAIL;
    return LoadFrame(target, fd);
}

Status CommandTest(RonReplica& replica, const string& file) {
    Frame tests;
    Status ok = LoadFrame(tests, file);
    if (!ok) return ok;
    Frames io;
    ok = tests.Split(io);
    if (!ok) return ok;
    Builder b;
    static const string OK{"\033[0;32mOK\033[0m\t"};
    static const string FAIL{"\033[1;31mFAIL\033[0m\t"};
    for (int i = 0; ok && i < io.size(); i++) {
        Cursor c = io[i].cursor();
        if (c.id() != COMMENT_UUID)
            return Status::BADFRAME.comment("no in/out header");
        TERM term = c.term();
        string comment;
        if (c.size() > 2 && c.has(2, STRING)) comment = c.string(2);
        c.Next();
        if (term == QUERY) {
            Frame re = b.Release();
            b = Builder{};
            ok = CompareFrames<Frame>(re, io[i]);
            if (!ok) {
                cerr << io[i].data() << '\n';
                cerr << "@~ 'the actual response' !\n";
                cerr << re.data() << '\n';
            }
            cerr << "?\t" << comment << '\t' << (ok ? OK : FAIL) << endl;
        } else if (term == HEADER) {
            ok = replica.FrameRecv(b, io[i]);
            cerr << "!\t" << comment << '\t' << (ok ? OK : FAIL + ok.str())
                 << endl;
        } else {
            return Status::BADFRAME.comment("bad in/out header");
        }
    }
    return ok;
}

Status CommandDump(RonReplica& replica, const string& what) {
    FORM rdt = ZERO_RAW_FORM;
    if (what.size() > 1) {
        rdt = uuid2form(Uuid{what});
        if (rdt == ZERO_RAW_FORM) return Status::BADARGS.comment("unknown RDT");
    }
    if (!replica.open()) return Status::BAD_STATE.comment("db is not open?");
    StoreIterator i{replica.db()};
    i.SeekTo(Key{});
    while (i.status()) {
        Key key{i.key()};
        // if (rdt != RDT_COUNT && key.rdt() != rdt) continue;
        String k;
        k.reserve(64);
        k.push_back('\n');
        k.push_back('*');
        form2uuid(key.form()).write_base64(k);
        k.push_back('#');
        key.id().write_base64(k);
        k.push_back('\n');
        int wok = write(STDOUT_FILENO, k.data(), k.size());
        if (wok != k.size()) return Status::IOFAIL;
        Slice val = i.value();
        wok = write(STDOUT_FILENO, val.data(), val.size());
        if (wok != val.size()) return Status::IOFAIL;
        i.Next();
    }
    i.Close();
    return Status::OK;
}

Status CommandQuery(RonReplica& replica, const string& name) {
    if (name.empty()) return Status::BADARGS;
    Uuid id{}, rdt{};
    size_t dot;
    string termd{name};
    if (termd.find('?') == -1) termd.push_back('?');
    TextFrame::Cursor cur{termd};
    if (!cur.valid()) return Status::BADARGS.comment("need a query op");
    id = cur.id();
    rdt = cur.ref();
    if (id == Uuid::FATAL || rdt == Uuid::FATAL)
        return Status::BADARGS.comment("not an UUID");
    Builder result;
    Status ok = replica.Recv(result, cur);
    if (ok) cout << result.data() << '\n';
    return ok;
}

Status CommandGetFrame(RonReplica& replica, const string& name) {
    if (name.empty()) return Status::BADARGS;
    Uuid id{}, rdt{};
    size_t dot;
    if (name[0] == '@') {
        string termd{name};
        termd.push_back(';');
        TextFrame::Cursor cur{termd};
        if (!cur.valid()) return Status::BADARGS;
        id = cur.id();
        rdt = cur.ref();
    } else if ((dot = name.find('.')) != -1) {
        id = Uuid{name.substr(0, dot)};
        rdt = Uuid{name.substr(dot + 1, name.size() - dot - 1)};
    } else {
        id = Uuid{name};
    }
    if (id == Uuid::FATAL || rdt == Uuid::FATAL) return Status::BADARGS;
    Frame result;
    Status ok = id.version() == TIME ? replica.GetFrame(result, id, rdt)
                                     : replica.GetMap(result, rdt, id);
    if (!ok) return ok;
    if (!FLAGS_clean) {
        cout << result.data() << '\n';
    } else {
        Cursor c = result.cursor();  // TODO
        if (c.valid() && c.has(2, STRING)) {
            cout << c.string(2);
        }
    }
    return ok;
}

Status CommandWriteNewFrame(RonReplica& replica, const string& filename) {
    Uuid now = replica.Now();
    Frame unstamped;
    LoadFrame(unstamped, filename);
    Builder stamp;
    Cursor uc = unstamped.cursor();
    constexpr uint64_t MAXSEQ = 1 << 30;
    while (uc.valid()) {
        if (uc.id().origin() != 0)
            return Status::BAD_STATE.comment("stamped already");
        Uuid id{now.value()._64 + uc.id().value()._64, now.origin()};
        Uuid ref{uc.ref()};
        if (ref.origin() == 0 && ref.value() < MAXSEQ) {
            ref = Uuid{now.value()._64 + uc.ref().value()._64, now.origin()};
        }
        stamp.AppendAmendedOp(uc, RAW, id, ref);
        uc.Next();
    }
    Frame stamped = stamp.Release();

    cout << stamped.data() << '\n';

    Cursor cur = stamped.cursor();
    Status ok = Status::OK;
    RonReplica::Records batch;

    while (cur.valid() && ok) {
        ok = replica.WriteNewChain(batch, cur);
    }

    if (ok) {
        replica.db().Write(batch);
    }

    return ok;
}

Status CommandHashFrame(const string& filename) {
    Frame frame;
    LoadFrame(frame, filename);
    Frame::Builder report;

    unordered_map<Uuid, SHA2> hashes;
    unordered_map<Word, Word> tips;

    Frame::Cursor cur = frame.cursor();
    while (cur.valid()) {
        const Uuid& id = cur.id();
        const Uuid& ref = cur.ref();
        if (id.version() == TIME) {
            SHA2 sha2prev, sha2ref;
            auto ti = tips.find(id.origin());
            if (ti == tips.end()) {
                // hash_root(id.origin(), sha2prev);
                sha2prev = SHA2{Uuid{0, id.origin()}};
            } else {
                sha2prev = hashes[Uuid{ti->second, id.origin()}];
            }
            if (ref.version() == TIME) {
                auto ri = hashes.find(ref);
                if (ri == hashes.end()) return Status::TREEGAP;
                sha2ref = ri->second;
            } else if (ref.version() == NAME) {
                sha2ref = SHA2{ref};
            } else {
                return Status::BAD_STATE.comment("unrecognized op pattern");
            }
            SHA2 sha2;
            hash_op<Frame>(cur, sha2, sha2prev, sha2ref);
            tips[id.origin()] = id.value();  // TODO causality checks
            hashes[id] = sha2;
            report.AppendNewOp(RAW, OpMeta::SHA2_UUID, id, sha2.base64());
        } else if (id == OpMeta::SHA2_UUID && cur.size() > 2 &&
                   cur.type(2) == STRING) {
            string base64 = cur.string(2);
            SHA2 hash = SHA2::ParseBase64(base64);
            auto hi = hashes.find(id);
            if (hi == hashes.end()) {
                hashes[id] = hash;
                if (id.value() > tips[id.origin()])
                    tips[id.origin()] = id.value();
            } else if (!hash.matches(hi->second)) {
                return Status::HASHBREAK;
            }
        }
        cur.Next();
    }
    cout << report.data() << endl;
    return Status::OK;
}
