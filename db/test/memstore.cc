#include "../mem_store.hpp"
#include "testutil.hpp"

using namespace ron;
using namespace std;

using Frame = TextFrame;
using MemStore = InMemoryStore<TextFrame>;
using Store = MemStore;
using Iterator = typename Store::Iterator;

TEST(MemStore, Keys) {
    auto now = Uuid::Now();
    Word yarn = Word::random();
    Uuid id1 = Uuid::Time(now, yarn);
    Key key1{id1, LWW_FORM_UUID};
    ASSERT_EQ(key1.id(), id1);
    ASSERT_EQ(key1.form(), LWW_RDT_FORM);
    Uuid id2 = Uuid::Time(now.inc(), yarn);
    Uuid id3 = Uuid::Time(now, yarn.inc());
    ASSERT_GT(id2, id1);
    ASSERT_GT(id3, id1);
    ASSERT_GT(id2, id3);
    Key key2{id2, LWW_FORM_UUID};
    Key key3{id3, LWW_FORM_UUID};
    ASSERT_GT(key2, key1);
    ASSERT_GT(key3, key1);
    ASSERT_GT(key3, key2); // origin first
}

TEST(MemStore, Ends) {
    TmpDir tmp;
    tmp.cd("Ends");
    Store store;
    auto now = Uuid::Now();
    Uuid id{now, Word::random()};
    ASSERT_TRUE(IsOK(store.Open(id)));
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
    ASSERT_TRUE(i.value().Next());
    ASSERT_TRUE(IsOK(i.SeekTo(Key{}, false)));
    ASSERT_EQ(i.key(), Key{});
    ASSERT_TRUE(i.value().Next());
    
    // empty store, 2 records: zero and end
    ASSERT_TRUE(IsOK(i.Next()));
    ASSERT_TRUE(IsOK(i.SeekTo(Key::END, true)));
    ASSERT_EQ(i.key(), Key::END);
    ASSERT_FALSE(i.value().valid());
    
    // Next() at the end => end, returns ENDOFINPUT
    ASSERT_FALSE(i.Next()==Status::ENDOFINPUT);
    ASSERT_TRUE(IsOK(i.SeekTo(Key::END, false)));
    ASSERT_EQ(i.key(), Key::END);
    ASSERT_FALSE(i.value().valid());
}



TEST(MemStore, Range) {
    String A{"@1+A :lww 'int' 1;"};
    String B{"@2+A :1+A 'string' 'str';"};
    String M{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    MemStore store;
    Frame a{A}, b{B}, m{M};
    Key key{Uuid{"1+A"}, LWW_FORM_UUID};
    assert(store.Write(key, a));
    assert(store.Write(key, b));
    Frame _m;
    assert(store.Read(key, _m));
    std::cerr<<m.data();
    std::cerr<<_m.data();
    assert(CompareFrames(m, _m));
    assert(store.Read(key, _m));
    assert(CompareFrames(m, _m));
}

TEST (MemStore, Iterator) {
    String A{"@1+A :lww 'int' 1;"};
    String B{"@2+A :1+A 'string' 'str';"};
    String M{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    MemStore store;
    Frame a{A}, b{B}, m{M};
    Key key{Uuid{"1+A"}, LWW_FORM_UUID};
    assert(store.Write(key, a));
    assert(store.Write(key, b));
    MemStore::Iterator i{store};
    i.SeekTo(Key{});
    auto c = i.value();
    i.Next();
    assert(i.key()==Key::END);
    auto mc = m.cursor();
    assert(CompareWithCursors(mc, c));
    //auto j = store.begin();
    //assert(CompareFrames(m, (*j).second));
}

int main (int argc, char** args) {
    ::testing::InitGoogleTest(&argc, args);
    return RUN_ALL_TESTS();
}
