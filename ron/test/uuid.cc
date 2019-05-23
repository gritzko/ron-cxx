#include <cstdio>
#include <iostream>
#include <gtest/gtest.h>
#include "../uuid.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;


TEST(Word, Case) {
    ASSERT_TRUE(Uuid::NIL.value.base64_case()==NUMERIC);
    ASSERT_TRUE(Uuid{"2134"}.value.base64_case()==NUMERIC);
    ASSERT_TRUE(Uuid{"ABC~DEFZ"}.value.base64_case()==CAPS);
    ASSERT_TRUE(Uuid{"Abc_Def"}.value.base64_case()==CAMEL);
    ASSERT_TRUE(Uuid{"abc_xyz"}.value.base64_case()==SNAKE);
}


TEST (Uuid, All) {
    ASSERT_TRUE(sizeof(Word)==8);
    ASSERT_TRUE(sizeof(Uuid)==16);

    Uuid u1{"0000000001"};
    ASSERT_TRUE(u1.value==1);
    ASSERT_TRUE(u1.str()=="0000000001");
    //ASSERT_TRUE(u1.variant()==0); // FIXME
    ASSERT_TRUE(u1.version()==UUID::NAME);

    Uuid test{"test"};
    std::string test_str = test.str();
    ASSERT_TRUE(test_str=="test");

    static const Uuid SUBS_CF_UUID{1007006897032658944L, 0};
    ASSERT_TRUE(SUBS_CF_UUID.str()=="subs");

    Uuid zeros{"abc000$0"};
    ASSERT_TRUE(zeros.str()=="abc");
    ASSERT_TRUE(Uuid{"0/100+200"}.str()=="1+2");

    Uuid one = Uuid::Time(1,Word{"origin"});
    ASSERT_TRUE(one.version()==UUID::TIME);
    ASSERT_TRUE(one.derived().version()==UUID::DERIVED);
    ASSERT_TRUE(one.str()=="0000000001+origin");
    Uuid two = one.inc();
    ASSERT_TRUE(two.str()=="0000000002+origin");
    ASSERT_TRUE(two.derived().str()=="0000000002-origin");

    ASSERT_TRUE(one < one.derived());
    ASSERT_TRUE(one==one);
    ASSERT_TRUE(two>one);
    ASSERT_TRUE(one<two);
    ASSERT_TRUE(one!=two);

    Uuid led{"A/LED"};
    //ASSERT_TRUE(led.variant()==0);
    ASSERT_TRUE(led.version()==UUID::NAME);
    ASSERT_TRUE(led.variety()==10);
    ASSERT_TRUE(led.str()=="A/LED");
}


int main (int argn, char** args) {
    ::testing::InitGoogleTest(&argn, args);
    return RUN_ALL_TESTS();
}
