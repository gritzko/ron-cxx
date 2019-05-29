#include <iostream>
#include <gtest/gtest.h>
#include "../uuid.hpp"
#include "../text.hpp"
#include "../encdec.hpp"
#include "../op.hpp"
#include "../form.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

using Frame = TextFrame;
using Cursor = typename Frame::Cursor;
using Builder = typename Frame::Builder;

TEST(Codepoint, UTF8) {
    String latin{"ABC"};
    Codepoints latin_cp;
    ASSERT_EQ(ParseUtf8(latin_cp, latin), OK);
    ASSERT_EQ(latin[0], latin_cp[0]);
    ASSERT_EQ(latin[1], latin_cp[1]);
    ASSERT_EQ(latin[2], latin_cp[2]);

    String raw{"Юникод\t萬國碼"};
    Codepoints rawcp;
    ASSERT_EQ(ParseUtf8(rawcp, raw), OK);
    ASSERT_EQ(rawcp.size(), 10);
    ASSERT_EQ(rawcp[5], 0x0434);

    /*
     
     FIXME UPGRADE!!!
     
     using CodepointReader = TextFrame::StringIterator;
    String escaped{"\\u042Eникод\\u0009萬\\u570B碼"};
    Codepoints cp_raw, cp_esc;
    for(CodepointReader iraw{raw}; iraw; ++iraw) {
        cp_raw.push_back(*iraw);
    }
    for(CodepointReader iesc{escaped}; iesc; ++iesc) {
        cp_esc.push_back(*iesc);
    }
    ASSERT_EQ(cp_raw.size(), 10);
    ASSERT_EQ(cp_raw.size(), cp_esc.size());
    ASSERT_EQ(cp_raw[0], 0x042E);
    ASSERT_EQ(cp_raw[8], 0x570B);
    for(int i=0; i<cp_raw.size(); ++i) {
        ASSERT_EQ(cp_raw[i], cp_esc[i]);
    }*/
}

int main (int argn, char** args) {
    ::testing::InitGoogleTest(&argn, args);
    return RUN_ALL_TESTS();
}
