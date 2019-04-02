#include "../rocks_store.hpp"
#define DEframe_bUG 1

using namespace ron;
using namespace std;

using Frame = TextFrame;
using Store = RocksDBStore<TextFrame>;

Status Compare(const Frame& a, const Frame& b) {
    Status ok = CompareFrames(a, b);
    if (!ok) {
        cerr<<"FRAME A:\n"<<a.data()<<".\n";
        cerr<<"FRAME B:\n"<<b.data()<<".\n";
    }
    return ok;
}

void test_range_merge () {
    String frame_a{"@1+A :lww 'int' 1;"};
    String frame_b{"@2+frame_a :1+A 'string' 'str';"};
    String frame_merged{"@1+frame_a :lww 'int' 1, @2+A 'string' 'str';"};
    Store store;
    auto now = Uuid::Now();
    assert(store.Create("test23_range_"+now.str()));
    Frame a{frame_a}, b{frame_b}, m{frame_merged};
    Key key{Uuid{"1+frame_a"}, LWW_FORM_UUID};
    assert(store.Write(key, a));
    assert(store.Write(key, b));
    Frame rocks_merged;
    assert(store.Read(key, rocks_merged));
    assert(Compare(m, rocks_merged));
    assert(store.Read(key, rocks_merged));
    assert(Compare(m, rocks_merged));
    store.Close();
}

void test_iter () {
    String frame_a{"@1+A :lww 'int' 1;"};
    String frame_b{"@2+frame_a :1+A 'string' 'str';"};
    String frame_merged{"@1+frame_a :lww 'int' 1, @2+A 'string' 'str';"};
    Store store;
    auto now = Uuid::Now();
    assert(store.Create("test23_iter_"+now.str()));
    Frame a{frame_a}, b{frame_b}, m{frame_merged};
    Key key{Uuid{"1+frame_a"}, LWW_FORM_UUID};
    assert(store.Write(key, a));
    assert(store.Write(key, b));
    Store::Iterator i{store};
    assert(i.SeekTo(key));
    assert(CompareWithCursors(m.cursor(), i.value()));
    assert(i.Next()==Status::ENDOFINPUT);
    store.Close();
}

/*
void test_db_chain_merge () {
    TextReplica db{};
    string path = Uuid::Now().str();
    assert(db.Create(path));
    rocksdb::Writeframe_batch batch;
    Key key{Uuid{"1gHHUW+test"}, CHframe_aIN_RDT};
    string op1 = "@1gHHUW+test :lww ;";
    string op2 = "@1gHHUW0001+test :1gHHUW+test 'key' 'value' ;";
    Cursor c1{op1};
    Cursor c2{op2};
    assert(db.ReceiveChain(batch, Uuid::NIL, c1));
    assert(db.ReceiveChain(batch, Uuid::NIL, c2));
    db.db().Write(db.wo(), &batch);
    //db.db().Write(db.wo(), db.trunk_, key, op1);
    //db.db().Write(db.wo(), db.trunk_, key, op2);
    string merged;
    db.db().Read(db.ro(), db.trunk(), key, &merged); // FIXframe_mergedE hash?
    assert(merged=="@1gHHUW+test :lww;\n 'key' 'value';\n");
    Frame got;
    assert(db.Read(got, Uuid{"1gHHUW+test"}, LWW_RDT_ID));
    assert(got.data()==merged);
    Frame chain;
    assert(db.ReadChain(chain, Uuid{"1gHHUW+test"}));
    assert(chain.data()==merged);
    assert(db.Close());
    rmdir(path.c_str());
}
 */

int main (int argc, const char** args) {
    test_range_merge();
    test_iter();
}
