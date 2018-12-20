#include <unistd.h>
#include "ron/ron.hpp"
#include "db/replica.hpp"

using namespace ron;
using namespace std;
typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Replica<Frame> TextReplica;

void test_db_yarn_root () {
}

void test_db_chain_merge () {
    TextReplica db{};
    string path = Uuid::HybridTime(time(nullptr)).str();
    assert(db.Create(path));
    rocksdb::WriteBatch batch;
    Key key{Uuid{"1gHHUW+test"}, RDT::CHAIN};
    string op1 = "@1gHHUW+test :lww ;";
    string op2 = "@1gHHUW0001+test :1gHHUW+test 'key' 'value' ;";
    Cursor c1{op1};
    Cursor c2{op2};
    assert(db.ReceiveChain(batch, Uuid::ZERO, c1));
    assert(db.ReceiveChain(batch, Uuid::ZERO, c2));
    //db.db().Merge(db.wo(), db.trunk_, key, op1);
    //db.db().Merge(db.wo(), db.trunk_, key, op2);
    string merged;
    db.db().Get(db.ro(), db.trunk_, key, &merged); // FIXME hash?
    assert(merged=="@1gHHUW+test :lww;\n 'key' 'value';\n");
    Frame got;
    db.Get(got, Uuid{"1gHHUW+test"}, LWW_TYPE_ID);
    assert(got.data()==merged);
    string chain;
    assert(db.FindChain(Uuid{"1gHHUW+test"}, chain));
    assert(chain==merged);
    assert(db.Close());
    rmdir(path.c_str());
}

void test_chain_breaks () {
    typedef TextReplica::ChainMeta Meta;
    Frame headf{"@1gA9cq+gritzko :lww; 'a' 1; 'b' 2.0;"};
    Frame::Cursor headc = headf.cursor();
    Meta headm;
    assert(headm.ScanAll(headc));
    assert(headm.object==Uuid{"1gA9cq+gritzko"});
    assert(headm.at==Uuid{"1gA9cq0002+gritzko"});
    assert(headm.ScanFrame("@1gA9cz+notgritzko :1gA9cq0002+gritzko;")==Status::CHAINBREAK);
    assert(headm.ScanFrame("@1gA9cq+gritzko :1gA9cq0002+gritzko;")==Status::REPEAT);
    assert(headm.ScanFrame("@1gA9cq0003+gritzko :1gA8k+notgritzko;")==Status::CHAINBREAK);
    assert(headm.ScanFrame("@obj 12345+obj! @1gA9cq0004+gritzko :1gA9cq0003+gritzko ;")==Status::TREEBREAK);
    assert(headm.ScanFrame("@1gA9cq0004+gritzko :1gA8k0003+gritzko 'd' 4;")==Status::TREEGAP);
    assert(headm.ScanFrame("@1gA9cq0003+gritzko :1gA9cq0002+gritzko 'c' 'three';"));
    assert(headm.ScanFrame("@prev 1gA9cq0004+gritzko! @1gA9cq0005+gritzko :1gA8k0003+gritzko 'e' 5;")==Status::YARNGAP);
    assert(headm.ScanFrame("@1gA9cq0004+gritzko :1gA9cq0003+gritzko 'd' 4;"));
    assert(headm.object==Uuid{"1gA9cq+gritzko"});
    assert(headm.at==Uuid{"1gA9cq0004+gritzko"});
}

void test_keys () {
    Uuid id{"1gA9cz+gritzko"};
    Key chain{id, RDT::CHAIN};
    assert(chain.rdt()==RDT::CHAIN);
    Uuid id2 = chain.id();
    assert(id2==id);
    Key derived{id.derived(), RDT::CHAIN};
    assert(derived.id()==id);
}

int main (int argc, const char** args) {
    test_keys();
    test_chain_breaks();
    test_db_yarn_root();
    test_db_chain_merge();
}
