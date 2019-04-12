#include "../mem_store.hpp"
#include "../joined_store.hpp"
#include "testutil.hpp"

using namespace ron;
using namespace std;

using Frame = TextFrame;
using MemStore = InMemoryStore<TextFrame>;
using TwoMemStore = JoinedStore<MemStore, MemStore>;
using Store = TwoMemStore;
using Iterator = typename TwoMemStore::Iterator;

TEST(JoinStore, Ends) {
    TmpDir tmp;
    tmp.cd("Ends");
    Uuid id{Uuid::Now(), Word::random()};
    MemStore mem1, mem2;
    mem1.Open(Uuid{Uuid::Now(), Word::random()});
    mem2.Open(Uuid{Uuid::Now(), Word::random()});
    Store store{mem1, mem2};
    Frame frame;
    ASSERT_TRUE(IsOK(store.Read(Key{}, frame)));
    ASSERT_FALSE(frame.empty());
    ASSERT_TRUE(IsOK(store.Read(Key::END, frame)));
    ASSERT_TRUE(frame.empty());
    ASSERT_TRUE(IsOK(store.Read(Key{id,LWW_RDT_FORM}, frame)));
    ASSERT_TRUE(frame.empty());
    Iterator i{store};
    ASSERT_EQ(i.key(), Key::END);
    ASSERT_FALSE(i.value().valid());
    
    ASSERT_TRUE(IsOK(i.SeekTo(Key::END, false)));
    ASSERT_EQ(i.key(), Key::END);
    ASSERT_FALSE(i.value().valid());
    ASSERT_TRUE(IsOK(i.SeekTo(Key::END, true)));
    ASSERT_EQ(i.key(), Key::END);
    ASSERT_FALSE(i.value().valid());
    
    ASSERT_TRUE(IsOK(i.SeekTo(Key{}, true)));
    ASSERT_EQ(i.key(), Key{});
    ASSERT_TRUE(i.value().valid());
    ASSERT_TRUE(IsOK(i.SeekTo(Key{}, false)));
    ASSERT_EQ(i.key(), Key{});
    ASSERT_TRUE(i.value().valid());
    
    // empty store, 2 records: zero and end
    ASSERT_TRUE(IsOK(i.Next()));
    ASSERT_TRUE(IsOK(i.SeekTo(Key::END, true)));
    ASSERT_EQ(i.key(), Key::END);
    ASSERT_FALSE(i.value().valid());
    
    // Next() at the end => end, returns ENDOFINPUT
    ASSERT_FALSE(i.Next()==Status::ENDOFINPUT);
    ASSERT_TRUE(IsOK(i.SeekTo(Key::END, true)));
    ASSERT_EQ(i.key(), Key::END);
    ASSERT_FALSE(i.value().valid());
}


TEST(JoinStore, Merge) {
    String A{"@1+A :lww 'int' 1;"};
    String B{"@2+A :1+A 'string' 'str';"};
    String M{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    MemStore storeA, storeB;
    TwoMemStore storeAB{storeA, storeB};
    Frame a{A}, b{B}, m{M};
    Key key{Uuid{"1+A"}, LWW_FORM_UUID};
    assert(storeA.Write(key, a));
    assert(storeB.Write(key, b));
    Frame _m;
    assert(storeAB.Read(key, _m));
    assert(CompareFrames(m, _m));
}

TEST (JoinStore, Iterator) {
    String A{"@1+A :lww 'int' 1;"};
    String B{"@2+A :1+A 'string' 'str';"};
    String M{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    MemStore store;
    Frame a{A}, b{B}, m{M};
    Key key{Uuid{"1+A"}, LWW_FORM_UUID};
    assert(store.Write(key, a));
    assert(store.Write(key, b));
    /*auto i = store.begin();
    Frame _m = (*i).second;
    ++i;
    assert(i==store.end());
    assert(CompareFrames(m, _m));
    auto j = store.begin();
    assert(CompareFrames(m, (*j).second));*/
}

int main (int argc, char** args) {
    ::testing::InitGoogleTest(&argc, args);
    return RUN_ALL_TESTS();
}
