#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include "../rdt/rdt.hpp"
#include "../ron/ron.hpp"
#include "fs.hpp"
#include "replica.hpp"
#include "rocksdb/db.h"

using namespace ron;
using namespace std;

using Frame = TextFrame;
using RonReplica = Replica<Frame>;
using Builder = Frame::Builder;
using Cursor = Frame::Cursor;
using Frames = vector<Frame>;
using Store = RonReplica::Store;
using StoreIterator = typename RonReplica::RocksStore::Iterator;

#define CHECKARG(a, x)                     \
    if (a) {                               \
        return Status::BADARGS.comment(x); \
    }

// const Uuid COMMENT_UUID{1134907106097364992UL, 0};
const Uuid OUT_UUID{935024688260710400UL, 0};
const Uuid IN_UUID{824721681762222080UL, 0};
const Uuid TEST_UUID{526483344616062976UL, 0};

using Args = Strings;

constexpr const char* HELP_USAGE{
    "help\n"
    "   print a memo on commands\n"};

constexpr const char* HOP_USAGE{
    "hop <BranchName|SNAPSHOT>\n"
    "   switch to another branch or snapshot\n"};

Status LoadFrame(Frame& target, int fd) {
    string tmp;
    constexpr size_t BLOCK = 1 << 12;
    char buf[BLOCK];
    ssize_t s;
    while ((s = read(fd, buf, BLOCK)) > 0) tmp.append(buf, size_t(s));
    if (s < 0) {
        return Status::IOFAIL.comment(strerror(errno));
    }
    target = Frame{tmp};
    return Status::OK;
}

Status LoadFrame(Frame& target, const string& filename) {
    int fd = filename.empty() ? STDIN_FILENO : open(filename.c_str(), O_RDONLY);
    if (fd < 0) {
        return Status::IOFAIL.comment(strerror(errno));
    }
    return LoadFrame(target, fd);
}

Status ScanAsNameArg(Uuid& name, case_t lcase, RonReplica& replica,
                     Args& args) {
    if (args.empty()) {
        name = Uuid::NIL;
        return Status::OK;
    }
    if (args.back() != "as") {
        return Status::OK;
    }
    args.pop_back();
    CHECKARG(args.empty(), "please provide a name");
    name = Uuid{args.back()};
    CHECKARG(name.is_error(), "must be a UUID");
    CHECKARG(name.version() != NAME, "must be a NAME UUID");
    CHECKARG(lcase != NUMERIC && name.value().base64_case() != lcase,
             "use " + CASE_NAMES[lcase]);
    args.pop_back();
    return Status::OK;
}

Status ScanOnBranchArg(Uuid& branch, RonReplica& replica, Args& args) {
    if (args.empty()) {
        branch = replica.current_branch();
        return Status::OK;
    }
    if (args.back() != "on") {
        return Status::OK;
    }
    args.pop_back();
    CHECKARG(args.empty(), "on what branch?");
    branch = Uuid{args.back()};
    args.pop_back();
    CHECKARG(branch.is_error(), "bad branch id syntax; must be name/id");
    CHECKARG(!replica.HasBranch(branch), "no such branch");
    return Status::OK;
}

Status ScanOfObjectArg(Uuid& object, RonReplica& replica, Args& args) {
    if (args.empty()) {
        object = Uuid::NIL;
        return Status::OK;
    }
    if (args.back() != "of") {
        return Status::OK;
    }
    args.pop_back();
    CHECKARG(args.empty(), "of what object?");
    object = Uuid{args.back()};
    args.pop_back();
    CHECKARG(object.is_error(), "bad id syntax");
    return Status::OK;
}

constexpr const char* INIT_USAGE{
    "init\n"
    "   create a replica\n"};

Status CommandInit(RonReplica& replica, Args& args) {
    if (replica.open()) {
        return Status::BADARGS.comment("replica already exists");
    }
    return replica.CreateReplica();
}

constexpr const char* CREATE_USAGE{
    "create [as BranchName]\n"
    "   create an empty branch\n"
    "   BranchName a tag for the branch (name UUID, up to 10 Base64 chars)\n"
    "       e.g. swarmdb create as Experiment\n"};

Status CommandCreate(RonReplica& replica, Args& args) {
    Word yarn_id = Word::random();  // TODO keys
    Uuid branch_id = Uuid::Time(NEVER, yarn_id);
    IFOK(replica.CreateBranch(branch_id));
    if (!args.empty() && args.back() == "as") {
        args.pop_back();
        CHECKARG(args.empty(), "need a name for the branch");
        Uuid tag{args.back()};
        CHECKARG(tag.is_error(),
                 "the name must be a name UUID (up to ten Base64 chars)");
        IFOK(replica.WriteName(tag, branch_id));
    }

    return Status{branch_id, "You started a yarn"};
}

constexpr const char* TEST_USAGE{
    "test test_file.ron\n"
    "   runs a RON test script\n"};

Status CommandTest(RonReplica& replica, Args& args) {
    CHECKARG(args.empty(), TEST_USAGE);
    String file = args.back();
    args.pop_back();
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
        if (c.id() != Uuid::COMMENT)
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
            ok = replica.ReceiveFrame(b, io[i]);
            cerr << "!\t" << comment << '\t' << (ok ? OK : FAIL + ok.str())
                 << endl;
        } else {
            return Status::BADFRAME.comment("bad in/out header");
        }
    }
    return ok;
}

Status CommandDump(RonReplica& replica, Args& args) {
    FORM rdt = ZERO_RAW_FORM;
    /*if (what.size() > 1) {
        rdt = uuid2form(Uuid{what});
        if (rdt == ZERO_RAW_FORM) return Status::BADARGS.comment("unknown RDT");
    }*/
    if (!replica.open()) return Status::BAD_STATE.comment("db is not open?");
    StoreIterator i{replica.GetBranch(Uuid::NIL)};
    IFOK(i.SeekTo(Key{}));
    do {
        cout << i.key().str() << '\t' << i.value().data().str();
    } while (i.Next());
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
    Status ok = replica.Receive(result, cur);
    if (ok) cout << result.data() << '\n';
    return ok;
}

constexpr const char* GET_USAGE{
    "get lww of 12345+someid [clean]\n"
    "   prints a stored frame\n"
    "   lww - form id\n"
    "   12345+someid - object UUID or object version UUID\n"
    "       e.g. swarmdb get log of 1kFHN0VmHh+0046cGqZgm\n"};

Status CommandGetFrame(RonReplica& replica, Args& args) {
    CHECKARG(args.empty(), GET_USAGE);
    Uuid rdt{args.back()};
    args.pop_back();
    CHECKARG(rdt == Uuid::FATAL || rdt.version() != NAME,
             "the form must be a NAME UUID");
    Uuid id;
    IFOK(ScanOfObjectArg(id, replica, args));
    if (id.version() == NAME) {
        IFOK(replica.ReadName(id, id));
    }
    CHECKARG(id.version() != UUID::TIME,
             "frame id must be an EVENT UUID, not " + args.back());
    bool clean = !args.empty() && args.back() == "clean";
    size_t dot;
    /*if (name[0] == '@') {
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
    }*/
    Builder result;
    Frame query = Query<Frame>(id, rdt);
    Cursor qc{query};
    IFOK(replica.Receive(result, qc));
    if (!clean) {
        cout << result.data() << '\n';
    } else {
        Cursor c{result.data()};
        if (c.valid() && c.has(2, STRING)) {
            cout << c.string(2);
        } else {
            return Status::BADVALUE.comment("no string in the first value pos");
        }
    }
    return Status::OK;
}

constexpr const char* WRITE_USAGE{
    "write file.ron [on BranchName]\n"
    "   write an unstamped frame to the db\n"};

Status CommandWrite(RonReplica& replica, Args& args) {
    String filename = "-";
    if (!args.empty()) {
        filename = args.back();
        args.pop_back();
    }
    Uuid now = replica.Now();
    Frame unstamped;
    IFOK(LoadFrame(unstamped, filename));
    Builder res;
    return replica.ReceiveFrame(res, unstamped);
}

constexpr const char* NEW_USAGE{
    "new <rdt> [as objectname] [on BranchName]\n"
    "   create an empty object of the given type\n"
    "   <objectname> a tag for the object (name UUID)\n"
    "       e.g. swarmdb new ct as readme\n"};

Status CommandNew(RonReplica& replica, Args& args) {
    CHECKARG(args.empty(), NEW_USAGE);
    Uuid rdt{args.back()};
    CHECKARG(rdt.is_error(), "can't parse " + args.back());
    args.pop_back();
    FORM form = uuid2form(rdt);
    CHECKARG(form == ERROR_NO_FORM, "form unknown");

    Uuid name;
    IFOK(ScanAsNameArg(name, SNAKE, replica, args));

    CHECKARG(!args.empty(), NEW_USAGE);

    Frame new_obj = OneOp<Frame>(replica.Now(), rdt);
    Builder re;
    Cursor cu{new_obj};
    Status ok = replica.Receive(re, cu);
    if (ok) {
        if (!name.zero()) {
            replica.WriteName(name, ok.code());
        }
        ok = ok.comment(rdt.str() + " object created");
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
            report.AppendNewOp(OpMeta::SHA2_UUID, id, sha2.base64());
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

constexpr const char* LIST_USAGE{
    "list [branches|snapshots|stores] [in BranchName]\n"
    "   list branches (or snapshots, or all the stores)\n"};

Status CommandList(RonReplica& replica, Args& args) {
    CHECKARG(args.empty(), LIST_USAGE);

    Uuids stores;
    IFOK(replica.ListStores(stores));

    typename RonReplica::Names names;
    IFOK(replica.ReadNames(names));

    for (auto& u : stores) {
        cout << u.str() << '\t' << names[u] << '\n';
    }
    return Status::OK;
}

constexpr const char* NAME_USAGE{
    "name 1234+someid as new_name [on BranchName]\n"
    "   assign a name to an object (a yarn, a version)\n"};

Status CommandName(RonReplica& replica, Args& args) {
    CHECKARG(args.empty(), NAME_USAGE);
    Uuid id{args.back()};
    args.pop_back();
    CHECKARG(id.is_error(), NAME_USAGE);
    CHECKARG(id.version() != TIME, "can only name event ids");

    Uuid name;
    IFOK(ScanAsNameArg(name, NUMERIC, replica, args));
    CHECKARG(name.zero(), NAME_USAGE);

    Status ok = replica.WriteName(name, id);
    if (ok) {
        ok = ok.comment("assigned name " + name.str() + " to " + id.str());
    }
    return ok;
}

constexpr const char* NAMED_USAGE{
    "named [yarns|objects|versions|things] [on BranchName]\n"
    "   list names for branches (or snapshots, or objects or all the names)\n"};

Status CommandNamed(RonReplica& replica, Args& args) {
    map<String, case_t> str2code{
        {"yarns", CAMEL},
        {"objects", SNAKE},
        {"versions", CAPS},
        {"things", NUMERIC},
    };
    case_t what = NUMERIC;
    if (!args.empty()) {
        auto i = str2code.find(args.back());
        CHECKARG(i == str2code.end(), NAMED_USAGE);
        what = i->second;
        args.pop_back();
    }
    Uuid branch;
    IFOK(ScanOnBranchArg(branch, replica, args));

    typename RonReplica::Names names;
    IFOK(replica.ReadNames(names, branch));

    for (auto& p : names) {
        Uuid id = p.second;
        Uuid name = p.first;
        if (what == NUMERIC || what == name.value().base64_case()) {
            cout << p.second.str() << '\t' << p.first.str() << '\n';
        }
    }

    return Status::OK;
}

Status CommandHop(RonReplica& replica, Args& args) {
    // 1. exact tag list mech
    // 2. current branch? LoadFrame?
    // 3. now/version?
    return Status::NOT_IMPLEMENTED;
}

Status CommandHelp(RonReplica& replica, Args& args) {
    cout << "swarmdb -- a versioned syncable RON database\n"
            "   \n"
         << HELP_USAGE << INIT_USAGE << CREATE_USAGE << NEW_USAGE << GET_USAGE
         << LIST_USAGE << NAME_USAGE << NAMED_USAGE << HOP_USAGE << TEST_USAGE
         << WRITE_USAGE;
    return Status::OK;
}

Status OpenReplica(RonReplica& replica) {
    // TODO  go ../../.. for .swarmdb
    if (!file_exists(ROCKSDB_STORE_DIR)) {
        return Status::NOT_FOUND.comment("no replica found (.swarmdb)");
    }
    IFOK(replica.Open());
    return Status::OK;
}

Status RunCommands(Args& args) {
    TmpDir tmp;
    RonReplica replica{};
    Status ok;

    if (args.empty()) return Status::OK;
    String verb{args.back()};
    args.pop_back();

    if (verb == "test") {
        IFOK(tmp.cd("swarmdb_test"));
        IFOK(replica.CreateReplica());
        IFOK(OpenReplica(replica));
        tmp.back();
    } else {
        std::srand(std::time(nullptr));
        ok = OpenReplica(replica);
    }

    if (verb == "init") {
        return CommandInit(replica, args);
    } else if (verb == "create") {
        return CommandCreate(replica, args);
    } else if (verb == "now") {
        return replica.Now();
    } else if (verb == "dump") {
        return CommandDump(replica, args);
    } else if (verb == "get") {
        return CommandGetFrame(replica, args);
    } else if (verb == "help" || verb == "--help" || verb == "-help" || verb == "-h") {
        return CommandHelp(replica, args);
    } else if (verb == "test") {
        return CommandTest(replica, args);
    } else if (verb == "create") {
        // TODO create db there^  create a CF here
    } else if (verb == "new") {
        return CommandNew(replica, args);
    } else if (verb == "list") {
        return CommandList(replica, args);
    } else if (verb == "named") {
        return CommandNamed(replica, args);
    } else if (verb == "name") {
        return CommandName(replica, args);
    } else if (verb == "write") {
        return CommandWrite(replica, args);
    } else if (verb == "hop") {
        return CommandHop(replica, args);
    } else {
        return Status::BADARGS.comment(
            "format: swarmdb verb [object] [prepositionals...]");
    }
    /*
     } else if (!FLAGS_hash.empty()) {
     ok = CommandHashFrame(FLAGS_hash);
     } else if (!FLAGS_query.empty()) {
     ok = CommandQuery(replica, FLAGS_query);
     } else if (!FLAGS_get.empty()) {
     ok = CommandGetFrame(replica, FLAGS_get);
     } else if (!FLAGS_write.empty()) {
     ok = CommandWriteNewFrame(replica, FLAGS_write);
     } else if (!FLAGS_dump.empty()) {
     } else if (!FLAGS_test.empty()) {
     ok = CommandTest(replica, FLAGS_test);
     } else {
     }*/

    replica.Close();

    return ok;
}

int main(int argn, char** args) {
    Args arguments{};
    for (int i = 1; i < argn; ++i) {
        arguments.emplace_back(args[i]);
    }
    std::reverse(arguments.begin(), arguments.end());
    if (getenv("TRACE")) {
        Key::trace_by_key = true;
    }

    Status ok = RunCommands(arguments);

    if (ok != Status::OK) {
        cout << ok.code().str() << '\n';
        if (!ok.comment().empty()) {
            cerr << ok.comment() << '\n';
        }
    }

    return ok.code().is_error() ? -1 : 0;
}
