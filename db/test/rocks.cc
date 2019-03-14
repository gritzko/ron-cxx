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
    Key key{Uuid{"1+A"}, LWW_RDT};
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
    Key key{Uuid{"1+A"}, LWW_RDT};
    check(store.Merge(key, a));
    check(store.Merge(key, b));
    Store::Iterator i{store};
    i.SeekTo(key);
    Frame _m{i.value()};
    check(CompareFrames(m, _m));
    assert(i.Next()==Status::ENDOFINPUT);
    store.Close();
}

int main (int argc, const char** args) {
    test_range_merge();
    test_iter();
}
