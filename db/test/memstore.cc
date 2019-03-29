#include "../mem_store.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

using Frame = TextFrame;
using MemStore = InMemoryStore<TextFrame>;

void test_range_merge () {
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
    assert(CompareFrames(m, _m));
    assert(store.Read(key, _m));
    assert(CompareFrames(m, _m));
}

void test_iter () {
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
    assert(i.key()==END_KEY);
    auto mc = m.cursor();
    assert(CompareWithCursors(mc, c));
    //auto j = store.begin();
    //assert(CompareFrames(m, (*j).second));
}

int main (int argc, const char** args) {
    test_range_merge();
    test_iter();
}
