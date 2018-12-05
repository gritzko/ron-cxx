#include "ron/ron.hpp"
#include "db/replica.hpp"

using namespace ron;
using namespace std;
typedef TextFrame Frame;
typedef Replica<Frame> TFR;

void test_defaults () {
    Frame::Builder b;
    Frame raw{"@12345+test :lww ;@123456+test :12345+test 'key' 'value' ;"};
    b.AppendFrame(raw);
    Frame nice = b.frame();
    assert(nice.data()=="@12345+test :lww;\n'key' 'value';\n");
}

void test_db_chain_merge () {
    TFR db{};
    assert(db.Create("tmp_path"));
    rocksdb::WriteBatch batch;
    string op1 = "@12345+test :lww ;";
    string op2 = "@123456+test :12345+test 'key' 'value' ;";
    db.ReceiveChain(batch, Uuid::ZERO, Frame{op1});
    db.ReceiveChain(batch, Uuid::ZERO, Frame{op2});
    string merged;
    db.db().Get(db.ro(), db.chains_, TFR::Key{Uuid{"12345+test"}}, &merged);
    assert(merged=="@12345+test :lww ;\n'key' 'value' ;\n");
    string chain;
    db.FindChain(Uuid{"12345+test"}, chain);
    assert(chain==merged);
    assert(db.Close());
}

int main (int argc, const char** args) {
    test_defaults();
    test_db_chain_merge();
}
