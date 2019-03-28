#include "../mem_store.hpp"
#include "../joined_store.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

using Frame = TextFrame;
using MemStore = InMemoryStore<TextFrame>;
using TwoMemStore = JoinedStore<MemStore, MemStore>;

void test_read_merge () {
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

void test_iter_merge () {
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

int main (int argc, const char** args) {
    test_read_merge();
    test_iter_merge();
}
