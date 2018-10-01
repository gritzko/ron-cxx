#include <cstdio>
#include <iostream>
#include <cassert>
#include "uuid.hpp"

using namespace ron;
using namespace std;


int main (int argn, char** args) {
    Word value, origin;
    value.set8(7, 1);
    origin.set16(3, 2);

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
