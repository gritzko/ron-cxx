#include <fcntl.h>
#include <gflags/gflags.h>
#include <unistd.h>
#include <ctime>
#include <unordered_map>
#include "db/replica.hpp"
#include "rdt/rdt.hpp"
#include "rocksdb/db.h"
#include "ron/ron.hpp"

using namespace ron;
using namespace std;

typedef TextFrame Frame;
typedef Replica<Frame> RonReplica;
typedef Frame::Builder Builder;
typedef Frame::Cursor Cursor;

DEFINE_bool(create, false, "create a new replica");
DEFINE_string(feed, "", "feed a RON frame");
DEFINE_string(write, "", "write a RON frame (new ops)");
DEFINE_string(get, "",
              "get a RON frame, e.g. --get @1gPH5o+gritzko:lww or "
              "1gPH5o+gritzko.lww or 1gPH5o+gritzko)");
DEFINE_bool(now, false, "print the current time(stamp)");
DEFINE_string(hash, "", "Merkle-hash a causally ordered frame");
DEFINE_bool(h, false, "Show help");
DECLARE_bool(help);
DECLARE_string(helpmatch);

Status CommandHashFrame(const string& filename);
Status CommandWriteNewFrame(RonReplica& replica, const string& filename);
Status CommandGetFrame(RonReplica& replica, const string& name);

Status RunCommands() {
    RonReplica replica{};
    Status ok;

    if (FLAGS_create) {
        ok = replica.Create(".swarmdb");
    } else {
        ok = replica.Open(".swarmdb");
    }
    if (!ok) return ok;

    if (FLAGS_now) {
        Uuid now = replica.now();
        cout << now.str() << endl;
    } else if (!FLAGS_hash.empty()) {
        ok = CommandHashFrame(FLAGS_hash);
    } else if (!FLAGS_get.empty()) {
        ok = CommandGetFrame(replica, FLAGS_get);
    } else if (!FLAGS_write.empty()) {
        ok = CommandWriteNewFrame(replica, FLAGS_write);
    } else {
    }

    if (!ok) {
        cerr << ok.str() << endl;
    }

    if (replica.open()) replica.Close();

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

    if (!ok) cerr << "error\t" << ok.str() << '\t' << ok.comment() << '\n';

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
    Status ok = replica.Get(result, id, rdt);
    if (ok) cout << result.data() << '\n';
    return ok;
}

Status CommandWriteNewFrame(RonReplica& replica, const string& filename) {
    Uuid now = replica.now();
    Frame unstamped;
    LoadFrame(unstamped, filename);
    Builder stamp;
    Cursor uc = unstamped.cursor();
    constexpr uint64_t MAXSEQ = 1 << 30;
    while (uc.valid()) {
        if (uc.id().origin() != 0) return Status::BAD_STATE;
        Uuid id{now.value()._64 + uc.id().value()._64, now.origin()};
        Uuid ref{uc.ref()};
        if (ref.origin() == 0 && ref.value() < MAXSEQ) {
            ref = Uuid{now.value()._64 + uc.ref().value()._64, now.origin()};
        }
        stamp.AppendAmendedOp(uc, RAW, id, ref);
        uc.Next();
    }
    Frame stamped = stamp.frame();

    cout << stamped.data() << '\n';

    Cursor cur = stamped.cursor();
    Status ok = Status::OK;
    rocksdb::WriteBatch batch;

    while (cur.valid() && ok) {
        ok = replica.ReceiveChain(batch, Uuid::NIL, cur);
    }

    if (ok) {
        replica.db().Write(replica.wo(), &batch);
    }

    return ok;
}

Status CommandHashFrame(const string& filename) {
    Frame frame;
    LoadFrame(frame, filename);
    Frame::Builder report;

    std::unordered_map<Uuid, SHA2> hashes;
    std::unordered_map<Word, Word> tips;

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
                return Status::BAD_STATE;
            }
            SHA2 sha2;
            hash_op<Frame>(cur, sha2, sha2prev, sha2ref);
            tips[id.origin()] = id.value();  // TODO causality checks
            hashes[id] = sha2;
            report.AppendNewOp(RAW, OpMeta::SHA2_UUID, id, sha2.base64());
        } else if (id == OpMeta::SHA2_UUID && cur.size() > 2 &&
                   cur.type(2) == STRING) {
            string base64 = cur.parse_string(2);
            SHA2 hash;
            SHA2::ParseBase64(hash, base64);
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