#include <unistd.h>
#include "ron/ron.hpp"
#include "db/replica.hpp"

using namespace ron;
using namespace std;
typedef TextFrame Frame;
typedef Replica<Frame> TextReplica;

void test_db_chain_merge () {
    TextReplica db{};
    string path = Uuid::HybridTime(time(nullptr)).str();
    assert(db.Create(path));
    rocksdb::WriteBatch batch;
    TextReplica::Key key{Uuid{"12345+test"}, RDT::CHAIN};
    string op1 = "@12345+test :lww ;";
    string op2 = "@1234500001+test :12345+test 'key' 'value' ;";
    //db.ReceiveChain(batch, Uuid::ZERO, Frame{op1});
    //db.ReceiveChain(batch, Uuid::ZERO, Frame{op2});
    db.db().Merge(db.wo(), db.chains_, key, op1);
    db.db().Merge(db.wo(), db.chains_, key, op2);
    string merged;
    db.db().Get(db.ro(), db.chains_, key, &merged);
    assert(merged=="@12345+test :lww;\n 'key' 'value';\n");
    string chain;
    assert(db.FindChain(Uuid{"12345+test"}, chain));
    assert(chain==merged);
    assert(db.Close());
    rmdir(path.c_str());
}

void test_chain_breaks () {
    typedef TextReplica::ChainMeta Meta;
    Frame headf{"@1gA9cq+gritzko :lww; 'a' 1; 'b' 2.0;"};
    Frame::Cursor headc = headf.cursor();
    Meta headm;
    assert(headm.Scan(headc));
    assert(headm.object==Uuid{"1gA9cq+gritzko"});
    assert(headm.at==Uuid{"1gA9cq0002+gritzko"});
    assert(headm.Scan("@1gA9cz+notgritzko :1gA9cq0002+gritzko;")==Status::CHAINBREAK);
    assert(headm.Scan("@1gA9cq+gritzko :1gA9cq0002+gritzko;")==Status::REPEAT);
    assert(headm.Scan("@1gA9cq0003+gritzko :1gA8k+notgritzko;")==Status::CHAINBREAK);
    assert(headm.Scan("@obj 12345+obj! @1gA9cq0004+gritzko :1gA9cq0003+gritzko ;")==Status::TREEBREAK);
    assert(headm.Scan("@1gA9cq0004+gritzko :1gA8k0003+gritzko 'd' 4;")==Status::TREEGAP);
    assert(headm.Scan("@1gA9cq0003+gritzko :1gA8k0002+gritzko 'c' 'three';"));
    assert(headm.Scan("@prev 1gA9cq0004+gritzko! @1gA9cq0005+gritzko :1gA8k0003+gritzko 'e' 5;")==Status::YARNGAP);
    assert(headm.Scan("@1gA9cq0004+gritzko :1gA8k0003+gritzko 'd' 4;"));
    assert(headm.object==Uuid{"1gA9cq+gritzko"});
    assert(headm.at==Uuid{"1gA9cq0004+gritzko"});
}

void test_keys () {
    Uuid id{"1gA9cz+gritzko"};
    TextReplica::Key chain{id, RDT::CHAIN};
    assert(chain.rdt()==RDT::CHAIN);
    Uuid id2 = chain.id();
    assert(id2==id);
    TextReplica::Key derived{id.derived(), RDT::CHAIN};
    assert(derived.id()==id);
}

void test_db_object_merge () {

}

int main (int argc, const char** args) {
    test_keys();
    test_chain_breaks();
    test_db_chain_merge();
    test_db_object_merge();
}
