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
    assert(store.Merge(key, a));
    assert(store.Merge(key, b));
    Frame _m;
    assert(store.Get(key, _m));
    assert(CompareFrames(m, _m));
    assert(store.Get(key, _m));
    assert(CompareFrames(m, _m));
}

void test_iter () {
    String A{"@1+A :lww 'int' 1;"};
    String B{"@2+A :1+A 'string' 'str';"};
    String M{"@1+A :lww 'int' 1, @2+A 'string' 'str';"};
    MemStore store;
    Frame a{A}, b{B}, m{M};
    Key key{Uuid{"1+A"}, LWW_FORM_UUID};
    assert(store.Merge(key, a));
    assert(store.Merge(key, b));
    auto i = store.begin();
    Frame _m = (*i).second;
    ++i;
    assert(i==store.end());
    assert(CompareFrames(m, _m));
    auto j = store.begin();
    assert(CompareFrames(m, (*j).second));
}

int main (int argc, const char** args) {
    test_range_merge();
    test_iter();
}
