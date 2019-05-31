#include "../rocks_store.hpp"
#include "testutil.hpp"

using namespace ron;
using namespace std;

using Frame = TextFrame;
using Store = RocksDBStore<TextFrame>;
using Iterator = typename Store::Iterator;

TEST(Store, Ends) {
    TmpDir tmp;
    tmp.cd("Ends");
    Store store;
    auto now = Uuid::Now();
    Uuid id{now, Word::random()};
    ASSERT_TRUE(IsOK(store.Create(Uuid::NIL)));
    Frame frame;
    ASSERT_TRUE(IsOK(store.Read(Key{}, frame)));
    ASSERT_FALSE(frame.empty());
    ASSERT_TRUE(IsOK(store.Read(Key::END, frame)));
    ASSERT_TRUE(frame.empty());
    ASSERT_TRUE(IsOK(store.Read(Key{id,LWW_RDT_FORM}, frame)));
    ASSERT_TRUE(frame.empty());

    // empty store, 2 records: zero and something
    Frame op1 = OneOp<Frame>(id, LOG_FORM_UUID);
    ASSERT_TRUE(IsOK(store.Write(Key{id,LOG_FORM_UUID}, op1)));

    Iterator i{store};
    ASSERT_EQ(i.key(), Key::END);
    ASSERT_FALSE(i.value().Next());

    ASSERT_TRUE(IsOK(i.SeekTo(Key::END, false)));
    ASSERT_EQ(i.key(), Key::END);
    ASSERT_FALSE(i.value().Next());
    ASSERT_TRUE(IsOK(i.SeekTo(Key::END, true)));
    //ASSERT_EQ(i.key(), Key::END);
    ASSERT_TRUE(i.value().Next()); // there must be something

    ASSERT_TRUE(IsOK(i.SeekTo(Key{}, true)));
    ASSERT_EQ(i.key(), Key{});
    ASSERT_TRUE(i.value().Next());

    ASSERT_TRUE(IsOK(i.SeekTo(Key{}, false)));
    ASSERT_EQ(i.key(), Key{});
    ASSERT_TRUE(i.value().Next());

    ASSERT_TRUE(IsOK(i.Next()));
    ASSERT_TRUE(IsOK(i.SeekTo(Key::END, true)));
    ASSERT_NE(i.key(), Key::END);
    ASSERT_TRUE(i.value().Next());

    // Next() at the end => end, returns ENDOFINPUT
    ASSERT_TRUE(i.Next()==Status::ENDOFINPUT); // FIXME  other stores
    ASSERT_TRUE(IsOK(i.SeekTo(Key::END, false)));
    ASSERT_EQ(i.key(), Key::END);
    ASSERT_FALSE(i.value().valid());
}

TEST (Store, Merge) {
    TmpDir tmp;
    tmp.cd("Merge");
    String frame_a{"@1+A :lww 'int' 1;"};
    String frame_b{"@2+A :1+A 'string' 'str';"};
    String frame_merged{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    Store store;
    auto now = Uuid::Now();
    Uuid id{now, Word::random()};
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


TEST (Store, Iterator) {
    TmpDir tmp;
    tmp.cd("Iterator");
    String frame_a{"@1+A :lww 'int' 1;"};
    String frame_b{"@2+A :1+A 'string' 'str';"};
    String frame_merged{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    Store store;
    auto now = Uuid::Now();
    Uuid id{now, Word::random()};
    ASSERT_TRUE(store.Create(Uuid::NIL));
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
    TmpDir tmp;
    tmp.cd("Branches");
    String frame_a{"@1+A :lww 'int' 1;"};
    String frame_b{"@2+A :1+A 'string' 'str';"};
    String frame_merged{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    Frame a{frame_a}, b{frame_b}, correct{frame_merged};
    Store store;
    auto now = Uuid::Now();
    //Uuid id{now, Word::random()};
    ASSERT_TRUE(IsOK(store.Create(Uuid::NIL)));
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
    
    branch.Close();
    store.Close();
    ASSERT_FALSE(store.open());
    ASSERT_FALSE(branch.open());

    typename Store::Branches branches;
    ASSERT_TRUE(IsOK(Store::OpenAll(branches)));
    ASSERT_EQ(branches.size(), 2);
    Store store2 = branches[Uuid::NIL];
    Store branch2 = branches[branch_id];
    ASSERT_TRUE(store2.open());
    ASSERT_TRUE(branch2.open());

    ASSERT_TRUE(store2.Read(key, merged));
    ASSERT_TRUE(CompareFrames(correct, merged));
    ASSERT_TRUE(branch2.Read(key, unmerged));
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
    if (getenv("TRACE")) {
        Key::trace_by_key = true;
    }
    ::testing::InitGoogleTest(&argc, args);
    return RUN_ALL_TESTS();
}
