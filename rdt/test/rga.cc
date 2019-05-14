#include <cassert>
#include "../../ron/ron.hpp"
#include "../rdt.hpp"
#include "testutil.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

using Frame = TextFrame;
using Cursor = typename Frame::Cursor;
using RGA = RGArrayRDT<typename ron::TextFrame>;

string despace (string& orig) {
    string ret;
    bool ows{false};
    for (char &i : orig) {
        if (isspace(i)) continue;
        ret.push_back(i);
    }
    return ret;
}

TEST(Merge, Chain) {
    string abc = "@1+A :rga! 'a', 'b', 'c', ";
    string def = "@1000000004+B :1000000003+A 'D', 'E', 'F', ";
    string abcdef;
    ASSERT_TRUE(IsOK(MergeStrings<TextFrame>(abcdef, RGA_RDT_FORM, Strings{abc, def})));
    string correct = "@1+A :rga! 'a', 'b', 'c', @1000000004+B 'D', 'E', 'F', ";
    ASSERT_TRUE(despace(abcdef)==despace(correct));
}

TEST(Merge, Sibling) {
    string parent = "@1+A :rga!";
    string childA = "@1a+B :1+A 'b';";
    string childB = "@1b+C :1+A 'a';";
    string ab;
    ASSERT_TRUE(MergeStrings<TextFrame>(ab, RGA_RDT_FORM, Strings{parent, childA, childB}));
    string correct = "@1+A :rga! @1b+C 'a', @1a+B :1+A 'b', ";
    ASSERT_TRUE(despace(ab)==despace(correct));
}

TEST(Merge, Multitree) {
    String childA{"@1a+B :1+A 'b';"};
    String childB{"@1b+C :1+A 'a';"};
    RGArrayRDT<TextFrame> reducer{};
    TextFrame::Builder b;
    TextFrame::Cursors c{};
    c.push_back(Cursor{childA});
    c.push_back(Cursor{childB});
    ASSERT_TRUE(reducer.Merge(b, c)==Status::CAUSEBREAK);
}

TEST(Scan, All0) {
    //  !
    //    a   c
    //      b   d
    String SIMPLE{"@1i08e4+path :rga! 'a', @1i08z+path 'b', @1i08k+path :1i08e4+path 'c', 'd',"};
    vector<bool> tombs{};
    ASSERT_TRUE(IsOK(ScanRGA<Frame>(tombs, Frame{SIMPLE})));
    ASSERT_TRUE( tombs[0]);
    ASSERT_TRUE(!tombs[1]);
    ASSERT_TRUE(!tombs[2]);
    ASSERT_TRUE(!tombs[3]);
    ASSERT_TRUE(!tombs[4]);
    ASSERT_TRUE(tombs.size()==5);
}

TEST(Scan, RM) {
    //  !
    //    a
    //      b
    //        rm d
    String RM{"@1i08e4+path :rga! 'a', 'b', @1i08k+path rm, @1i08e40003+path :1i08e40002+path 'd',"};
    vector<bool> tombs{};
    ASSERT_TRUE(IsOK(ScanRGA<Frame>(tombs, Frame{RM})));
    ASSERT_TRUE( tombs[0]);
    ASSERT_TRUE(!tombs[1]);
    ASSERT_TRUE( tombs[2]);
    ASSERT_TRUE( tombs[3]);
    ASSERT_TRUE(!tombs[4]);
}

TEST(Scan, RM2) {
    //  !
    //    a
    //      b
    //        rm    d
    //           rm
    String RMRM{"@1i08e4+path :rga! 'a', 'b', @1i08k+path rm, rm, @1i08e40003+path :1i08e40002+path 'd',"};
    vector<bool> tombs{};
    ASSERT_TRUE(IsOK(ScanRGA<Frame>(tombs, Frame{RMRM})));
    ASSERT_TRUE( tombs[0]);
    ASSERT_TRUE( tombs[1]);
    ASSERT_TRUE( tombs[2]);
    ASSERT_TRUE( tombs[3]);
    ASSERT_TRUE( tombs[4]);
    ASSERT_TRUE(!tombs[5]);
}

TEST(Scan, RM_UN) {
    //  !
    //    a
    //      b
    //        rm       d
    //           rm
    //              un
    String RMUN{"@1i08e4+path :rga! 'a', 'b', @1i08k+path rm, rm, un, @1i08e40003+path :1i08e40002+path 'd',"};
    vector<bool> tombs{};
    ASSERT_TRUE(IsOK(ScanRGA<Frame>(tombs, Frame{RMUN})));
    ASSERT_TRUE( tombs[0]);
    ASSERT_TRUE(!tombs[1]);
    ASSERT_TRUE( tombs[2]);
    ASSERT_TRUE( tombs[3]);
    ASSERT_TRUE( tombs[4]);
    ASSERT_TRUE( tombs[5]);
    ASSERT_TRUE(!tombs[6]);
}

TEST(Scan, Trash) {
    //  !
    //    a
    //      b
    //        rm       d
    //           rm      un
    //              c       rm
    //                         rm
    String ITSCOMPLEX{"@1i08e4+path :rga! 'a', 'b', @1i08k+path rm, rm, 'c', @1i08e40003+path :1i08e40002+path 'd', un, rm, rm,"};
    vector<bool> tombs{};
    ASSERT_TRUE(IsOK(ScanRGA<Frame>(tombs, Frame{ITSCOMPLEX})));
    ASSERT_TRUE( tombs[0]);
    ASSERT_TRUE( tombs[1]);
    ASSERT_TRUE( tombs[2]);
    ASSERT_TRUE( tombs[3]);
    ASSERT_TRUE( tombs[4]);
    ASSERT_TRUE( tombs[5]);
    ASSERT_TRUE(!tombs[6]);
    ASSERT_TRUE( tombs[7]);
    ASSERT_TRUE( tombs[8]);
    ASSERT_TRUE( tombs[9]);
}

TEST(Log2State, Basic) {
    String LOG{"@1000000001+A :rga, 't', 'e', 's', 't', @1000000007+A :1000000004+A rm, @1000000008+A :1000000004+A 'x';"};
    vector<bool> tombs{};
    Frame state;
    ASSERT_TRUE(IsOK(ObjectLogToState<Frame>(state, Frame{LOG})));
    ASSERT_TRUE(IsOK(ScanRGA<Frame>(tombs, Frame{state})));
    ASSERT_TRUE( tombs[0]); // rga
    ASSERT_TRUE(!tombs[1]); // t
    ASSERT_TRUE(!tombs[2]); // e
    ASSERT_TRUE( tombs[3]); // s
    ASSERT_TRUE(!tombs[4]); // x  oopsie
    ASSERT_TRUE( tombs[5]); // rm
    ASSERT_TRUE(!tombs[6]); // t
}

TEST(Log2State, Cutoff) {
    String LOG{"@1000000001+A :rga, 't', 'e', 's', 't', @1000000007+A :1000000004+A rm, @1000000008+A :1000000004+A 'x';"};
    vector<bool> tombs{};
    Frame state;
    ASSERT_TRUE(IsOK(ObjectLogToState<Frame>(state, Frame{LOG}, Uuid{"1000000007+A"})));
    ASSERT_TRUE(IsOK(ScanRGA<Frame>(tombs, Frame{state})));
    ASSERT_TRUE( tombs[0]);
    ASSERT_TRUE(!tombs[1]); // t
    ASSERT_TRUE(!tombs[2]); // e
    ASSERT_TRUE( tombs[3]); // s
    ASSERT_TRUE( tombs[4]); // rm 
    ASSERT_TRUE(!tombs[5]); // t
}

int main (int argc, char** args) {
    ::testing::InitGoogleTest(&argc, args);
    return RUN_ALL_TESTS();
}
