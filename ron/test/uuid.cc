#include <cstdio>
#include <iostream>
#include <cassert>
#include "ron/uuid.hpp"

using namespace ron;
using namespace std;


int main (int argn, char** args) {

    //cout << Uuid{"sha2"}.value()._64 << '\n';

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

    Uuid SUBS_CF_UUID{1007006897032658944L, 0};
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

    // TODO a little benchmark
    // TODO Uuid:Sha2()
    // TODO calendar -> timestamp
    // TODO clock
    // TODO coarsened timestamp

}
