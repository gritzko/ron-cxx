#include "../rocks_store.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

using Frame = TextFrame;
using Store = RocksDBStore<TextFrame>;

Status log (Status result, bool need=true) {
    if (result!=need) {
        cerr<<result.str()<<"\n";
    }
    return result;
}

#define check(X) assert(log(X, true))
#define checknot(X) assert(!log(X, false))

void test_range_merge () {
    String A{"@1+A :lww 'int' 1;"};
    String B{"@2+A :1+A 'string' 'str';"};
    String M{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    Store store;
    auto now = Uuid::Now();
    check(store.Create("test23_range_"+now.str()));
    Frame a{A}, b{B}, m{M};
    Key key{Uuid{"1+A"}, LWW_FORM_UUID};
    check(store.Merge(key, a));
    check(store.Merge(key, b));
    Frame _m;
    check(store.Get(key, _m));
    check(CompareFrames(m, _m));
    check(store.Get(key, _m));
    check(CompareFrames(m, _m));
    store.Close();
}

void test_iter () {
    String A{"@1+A :lww 'int' 1;"};
    String B{"@2+A :1+A 'string' 'str';"};
    String M{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    Store store;
    auto now = Uuid::Now();
    check(store.Create("test23_iter_"+now.str()));
    Frame a{A}, b{B}, m{M};
    Key key{Uuid{"1+A"}, LWW_FORM_UUID};
    check(store.Merge(key, a));
    check(store.Merge(key, b));
    Store::Iterator i{store};
    i.SeekTo(key);
    Frame _m{i.value()};
    check(CompareFrames(m, _m));
    assert(i.Next()==Status::ENDOFINPUT);
    store.Close();
}

/*
void test_db_chain_merge () {
    TextReplica db{};
    string path = Uuid::Now().str();
    assert(db.Create(path));
    rocksdb::WriteBatch batch;
    Key key{Uuid{"1gHHUW+test"}, CHAIN_RDT};
    string op1 = "@1gHHUW+test :lww ;";
    string op2 = "@1gHHUW0001+test :1gHHUW+test 'key' 'value' ;";
    Cursor c1{op1};
    Cursor c2{op2};
    assert(db.ReceiveChain(batch, Uuid::NIL, c1));
    assert(db.ReceiveChain(batch, Uuid::NIL, c2));
    db.db().Write(db.wo(), &batch);
    //db.db().Merge(db.wo(), db.trunk_, key, op1);
    //db.db().Merge(db.wo(), db.trunk_, key, op2);
    string merged;
    db.db().Get(db.ro(), db.trunk(), key, &merged); // FIXME hash?
    assert(merged=="@1gHHUW+test :lww;\n 'key' 'value';\n");
    Frame got;
    assert(db.Get(got, Uuid{"1gHHUW+test"}, LWW_RDT_ID));
    assert(got.data()==merged);
    Frame chain;
    assert(db.GetChain(chain, Uuid{"1gHHUW+test"}));
    assert(chain.data()==merged);
    assert(db.Close());
    rmdir(path.c_str());
}
 */

int main (int argc, const char** args) {
    test_range_merge();
    test_iter();
}
