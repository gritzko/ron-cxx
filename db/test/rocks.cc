#include "../rocks_store.hpp"
#include "gtest/gtest.h"
#define DEBUG 1

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

::testing::AssertionResult IsOK(Status ok) {
    if (ok)
        return ::testing::AssertionSuccess();
    else
        return ::testing::AssertionFailure() << ok.str();
}
#include "../fs.cc"

TEST (Store, range_merge) {
    String frame_a{"@1+A :lww 'int' 1;"};
    String frame_b{"@2+A :1+A 'string' 'str';"};
    String frame_merged{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    Store store;
    auto now = Uuid::Now();
    Uuid id{now, Word::random()};
    ASSERT_TRUE(cdtmp());
    ASSERT_TRUE(IsOK(store.Create(id)));
    ASSERT_TRUE(store.open());
    Frame a{frame_a}, b{frame_b}, correct{frame_merged};
    Key key{Uuid{"1+A"}, LWW_FORM_UUID};
    ASSERT_TRUE(IsOK(store.Write(key, a)));
    ASSERT_TRUE(store.Write(key, b));
    Frame rocks_merged;
    ASSERT_TRUE(store.Read(key, rocks_merged));
    ASSERT_TRUE(CompareFrames(correct, rocks_merged));
    ASSERT_TRUE(store.Read(key, rocks_merged));
    ASSERT_TRUE(CompareFrames(correct, rocks_merged));
    //store.Close();
}


TEST (Store, iterator) {
    String frame_a{"@1+A :lww 'int' 1;"};
    String frame_b{"@2+A :1+A 'string' 'str';"};
    String frame_merged{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    Store store;
    auto now = Uuid::Now();
    Uuid id{now, Word::random()};
    ASSERT_TRUE(cdtmp());
    ASSERT_TRUE(store.Create(id));
    Frame a{frame_a}, b{frame_b}, correct{frame_merged};
    Key key{Uuid{"1+A"}, LWW_FORM_UUID};
    ASSERT_TRUE(store.Write(key, a));
    ASSERT_TRUE(store.Write(key, b));
    Store::Iterator i{store};
    ASSERT_TRUE(i.SeekTo(key));
    ASSERT_TRUE(CompareWithCursors(correct.cursor(), i.value()));
    i.Next();
    i.Close();
    //ASSERT_TRUE(i.Next()==Status::ENDOFINPUT);
    store.Close();
}

TEST (Store, Branches) {
    String frame_a{"@1+A :lww 'int' 1;"};
    String frame_b{"@2+A :1+A 'string' 'str';"};
    String frame_merged{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    Frame a{frame_a}, b{frame_b}, correct{frame_merged};
    Store store;
    auto now = Uuid::Now();
    ASSERT_TRUE(cdtmp());
    Uuid id{now, Word::random()};
    ASSERT_TRUE(IsOK(store.Create(id)));
    Uuid branch_id{"0+branchB"};
    Key key{Uuid{"1+A"}, LWW_FORM_UUID};
    Store branch = store;
    ASSERT_TRUE(IsOK(branch.Create(branch_id)));
    ASSERT_TRUE(store.Write(key, a));
    ASSERT_TRUE(store.Write(key, b));
    ASSERT_TRUE(IsOK(branch.Write(key, a)));

    Frame merged, unmerged;
    ASSERT_TRUE(store.Read(key, merged));
    ASSERT_TRUE(CompareFrames(correct, merged));
    ASSERT_TRUE(branch.Read(key, unmerged));
    ASSERT_TRUE(CompareFrames(a, unmerged));

}

/*
void test_db_chain_merge () {
    TextReplica db{};
    string path = Uuid::Now().str();
    ASSERT_TRUE(db.Create(path));
    rocksdb::Writeframe_batch batch;
    Key key{Uuid{"1gHHUW+test"}, CHframe_aIN_RDT};
    string op1 = "@1gHHUW+test :lww ;";
    string op2 = "@1gHHUW0001+test :1gHHUW+test 'key' 'value' ;";
    Cursor c1{op1};
    Cursor c2{op2};
    ASSERT_TRUE(db.ReceiveChain(batch, Uuid::NIL, c1));
    ASSERT_TRUE(db.ReceiveChain(batch, Uuid::NIL, c2));
    db.db().Write(db.wo(), &batch);
    //db.db().Write(db.wo(), db.trunk_, key, op1);
    //db.db().Write(db.wo(), db.trunk_, key, op2);
    string merged;
    db.db().Read(db.ro(), db.trunk(), key, &merged); // FIXframe_mergedE hash?
    ASSERT_TRUE(merged=="@1gHHUW+test :lww;\n 'key' 'value';\n");
    Frame got;
    ASSERT_TRUE(db.Read(got, Uuid{"1gHHUW+test"}, LWW_RDT_ID));
    ASSERT_TRUE(got.data()==merged);
    Frame chain;
    ASSERT_TRUE(db.ReadChain(chain, Uuid{"1gHHUW+test"}));
    ASSERT_TRUE(chain.data()==merged);
    ASSERT_TRUE(db.Close());
    rmdir(path.c_str());
}
 */

int main (int argc, char** args) {
    ::testing::InitGoogleTest(&argc, args);
    return RUN_ALL_TESTS();
}
