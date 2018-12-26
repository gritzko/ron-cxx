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

// ## HUMANE
// create database test with spaces sha3,vv, compression gz, format binary
// show object 1ABCk+gritzko with version 1BCDj, format text
// show vv 1BCDj+gritzko

// ## INHUMANE
// db "*vv @1BCDj+gritzko ?"
// db ":db ! 'zip' 'gz', 'spaces' >sha3>vv>json"
// 1AbC2+gritzko
// db ":tag >test >1AbC2+gritzko"

DEFINE_bool(create, false, "create a new replica");
DEFINE_string(feed, "-", "feed a RON frame");
DEFINE_bool(now, false, "print the current time(stamp)");
DEFINE_string(hash, "-", "Merkle-hash a causally ordered frame");
DEFINE_bool(h, false, "Show help");
DECLARE_bool(help);
DECLARE_string(helpmatch);

Status CommandHashFrame(const string& filename);

Status RunCommands () {
    RonReplica replica{};
    Status ok;

    if (FLAGS_create) {
        ok = replica.Create(".swarmdb");
    } else {
        ok = replica.Open(".swarmdb");
    }
    if (!ok) return ok;

    if (FLAGS_now) {
        // TODO load replica clocks
        cout << Uuid{Uuid::HybridTime(time(nullptr)), Word::NEVER}.str()
             << endl;
    } else if (!FLAGS_hash.empty()) {
        ok = CommandHashFrame(FLAGS_hash);
    } else {
    }

    if (!ok) {
        cerr << ok.str() << endl;
    }

    if (replica.open()) replica.Close();

    return Status::OK;
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

    if (!ok) cerr<<"error"<<ok.str()<<'\n';

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
    int fd = open(filename.c_str(), O_RDONLY);
    if (fd < 0) return Status::IOFAIL;
    return LoadFrame(target, fd);
}


Status CommandHashFrame(const std::string& filename) {
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