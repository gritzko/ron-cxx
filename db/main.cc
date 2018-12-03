#include <time.h>
#include <gflags/gflags.h>
#include "ron/ron.hpp"
#include "rdt/rdt.hpp"
#include "rocksdb/db.h"
#include "db/replica.hpp"

using namespace ron;
using namespace std;

typedef Replica<TextFrame> TextReplica;

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
DEFINE_bool(h, false, "Show help");
DECLARE_bool(help);
DECLARE_string(helpmatch);

int main (int argn, char** args) {

    gflags::SetUsageMessage("swarmdb -- a syncable embedded RON database");
    gflags::ParseCommandLineNonHelpFlags(&argn, &args, true);
    if (FLAGS_help || FLAGS_h) {
        FLAGS_help = false;
        FLAGS_helpmatch = "db";
    }
    gflags::HandleCommandLineHelpFlags();

    TextReplica replica{};
    Status ok;

    if (FLAGS_create) {
        ok = replica.Create(".swarmdb");
    } else if (FLAGS_now) {
        // TODO load replica clocks
        cout << Uuid{Uuid::HybridTime(time(nullptr)), Word::NEVER}.str() << endl;
    } else {
        ok = replica.Open(".swarmdb");
    }

    if (ok) {
    }
//    TextFrame script{args[1]};
//    TextFrame::Builder response;
//
//    Status ok = replica.Run(response, {script});

    if (!ok.code_.zero()) {
        cerr << ok.str() << endl;
    }

    replica.Close();

    return ok ? 0 : -1;
}
