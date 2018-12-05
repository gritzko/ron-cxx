#include <unistd.h>
#include "ron/ron.hpp"
#include "db/replica.hpp"

using namespace ron;
using namespace std;
typedef TextFrame Frame;
typedef Replica<Frame> TFR;

void test_db_chain_merge () {
    TFR db{};
    string path = Uuid::HybridTime(time(nullptr)).str();
    assert(db.Create(path));
    rocksdb::WriteBatch batch;
    TFR::Key key{Uuid{"12345+test"}};
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

int main (int argc, const char** args) {
    test_db_chain_merge();
}
