#include <cstdio>
#include <iostream>
#include <cassert>
#include "../uuid.hpp"
#include "../spans.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;


void test_uuids () {
    Uuid one{"1i5C0o+gYpLcnUnF6"};
    Uuid two{"1i5C1O+gYpLcnUnF6"};
    Uuids ids;
    assert(ids.size()==0);
    assert(ids.empty());

    ids.push_back(one);
    assert(!ids.empty());
    assert(ids.front()==one);
    assert(ids.back()==one);
    assert(ids.size()==1);

    ids.push_back(one.inc());
    assert(ids.size()==2);
    assert(ids.front()==one);
    assert(ids.back()==one.inc());

    ids.push_back(Span{two,2});
    assert(ids.size()==3);
    assert(ids.front()==one);
    assert(ids.back()==two);

    auto i = ids.begin();
    assert(*i==one);
    ++i;
    assert(*i==one.inc());
    ++i;
    assert(*i==two);
    ++i;
    assert(i==ids.end());
    --i;
    assert(*i==two);
    assert(i.ref_inc()==2);
    --i;
    assert(*i==one.inc());
    assert(i.ref_inc()==1);
    --i;
    assert(*i==one);

    ids.pop_back();
    assert(ids.size()==2);

}


void test_uuid () {
    static_assert(sizeof(Word)==8, "no mishup");
    static_assert(sizeof(Uuid)==16, "...");

    Uuid u1{"0000000001"};
    assert(u1.value()==1);
    assert(u1.str()=="0000000001");
    assert(u1.variant()==0); // FIXME
    assert(u1.version()==UUID::NAME);

    Uuid test{"test"};
    std::string test_str = test.str();
    assert(test_str=="test");

    static const Uuid SUBS_CF_UUID{1007006897032658944L, 0};
    assert(SUBS_CF_UUID.str()=="subs");

    Uuid zeros{"abc000$0"};
    assert(zeros.str()=="abc");
    assert(Uuid{"0/100+200"}.str()=="1+2");

    Uuid one = Uuid::Time(1,Word{"origin"});
    assert(one.version()==UUID::TIME);
    assert(one.derived().version()==UUID::DERIVED);
    assert(one.str()=="0000000001+origin");
    Uuid two = one.inc();
    assert(two.str()=="0000000002+origin");
    assert(two.derived().str()=="0000000002-origin");

    assert(one < one.derived());
    assert(one==one);
    assert(two>one);
    assert(one<two);
    assert(one!=two);

    Uuid led{"A/LED"};
    assert(led.variant()==0);
    assert(led.version()==UUID::NAME);
    assert(led.variety()==10);
    assert(led.str()=="A/LED");
}


int main (int argn, char** args) {
    test_uuid();
    test_uuids();
    // TODO a little benchmark
    // TODO Uuid:Sha2()
    // TODO calendar -> timestamp
    // TODO clock
    // TODO coarsened timestamp
}
