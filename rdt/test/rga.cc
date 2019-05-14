#include <cassert>
#include "../../ron/ron.hpp"
#include "../rdt.hpp"
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

void test_chain_merge () {
    string abc = "@1+A :rga! 'a', 'b', 'c', ";
    string def = "@1000000004+B :1000000003+A 'D', 'E', 'F', ";
    string abcdef;
    Status ok = MergeStrings<TextFrame>(abcdef, RGA_RDT_FORM, Strings{abc, def});
    assert(ok);
    string correct = "@1+A :rga! 'a', 'b', 'c', @1000000004+B 'D', 'E', 'F', ";
    assert(despace(abcdef)==despace(correct));
}

void test_sibling_merge () {
    string parent = "@1+A :rga!";
    string childA = "@1a+B :1+A 'b';";
    string childB = "@1b+C :1+A 'a';";
    string ab;
    assert(MergeStrings<TextFrame>(ab, RGA_RDT_FORM, Strings{parent, childA, childB}));
    string correct = "@1+A :rga! @1b+C 'a', @1a+B :1+A 'b', ";
    assert(despace(ab)==despace(correct));
}

void test_multitree () {
    String childA{"@1a+B :1+A 'b';"};
    String childB{"@1b+C :1+A 'a';"};
    RGArrayRDT<TextFrame> reducer{};
    TextFrame::Builder b;
    TextFrame::Cursors c{};
    c.push_back(Cursor{childA});
    c.push_back(Cursor{childB});
    assert(reducer.Merge(b, c)==Status::CAUSEBREAK);
}

void test_ct_scan_all0 () {
    //  !
    //    a   c
    //      b   d
    String SIMPLE{"@1i08e4+path :rga! 'a', @1i08z+path 'b', @1i08k+path :1i08e4+path 'c', 'd',"};
    vector<bool> tombs{};
    assert(ScanRGA<Frame>(tombs, Frame{SIMPLE}));
    assert( tombs[0]);
    assert(!tombs[1]);
    assert(!tombs[2]);
    assert(!tombs[3]);
    assert(!tombs[4]);
    assert(tombs.size()==5);
}

void test_ct_scan_rm () {
    //  !
    //    a
    //      b
    //        rm d
    String RM{"@1i08e4+path :rga! 'a', 'b', @1i08k+path rm, @1i08e40003+path :1i08e40002+path 'd',"};
    vector<bool> tombs{};
    assert(ScanRGA<Frame>(tombs, Frame{RM}));
    assert( tombs[0]);
    assert(!tombs[1]);
    assert( tombs[2]);
    assert( tombs[3]);
    assert(!tombs[4]);
}

void test_ct_scan_rm2 () {
    //  !
    //    a
    //      b
    //        rm    d
    //           rm
    String RMRM{"@1i08e4+path :rga! 'a', 'b', @1i08k+path rm, rm, @1i08e40003+path :1i08e40002+path 'd',"};
    vector<bool> tombs{};
    assert(ScanRGA<Frame>(tombs, Frame{RMRM}));
    assert( tombs[0]);
    assert( tombs[1]);
    assert( tombs[2]);
    assert( tombs[3]);
    assert( tombs[4]);
    assert(!tombs[5]);
}


void test_ct_scan_rm_un () {
    //  !
    //    a
    //      b
    //        rm       d
    //           rm
    //              un
    String RMUN{"@1i08e4+path :rga! 'a', 'b', @1i08k+path rm, rm, un, @1i08e40003+path :1i08e40002+path 'd',"};
    vector<bool> tombs{};
    assert(ScanRGA<Frame>(tombs, Frame{RMUN}));
    assert( tombs[0]);
    assert(!tombs[1]);
    assert( tombs[2]);
    assert( tombs[3]);
    assert( tombs[4]);
    assert( tombs[5]);
    assert(!tombs[6]);
}

void test_ct_scan_trash () {
    //  !
    //    a
    //      b
    //        rm       d
    //           rm      un
    //              c       rm
    //                         rm
    String ITSCOMPLEX{"@1i08e4+path :rga! 'a', 'b', @1i08k+path rm, rm, 'c', @1i08e40003+path :1i08e40002+path 'd', un, rm, rm,"};
    vector<bool> tombs{};
    assert(ScanRGA<Frame>(tombs, Frame{ITSCOMPLEX}));
    assert( tombs[0]);
    assert( tombs[1]);
    assert( tombs[2]);
    assert( tombs[3]);
    assert( tombs[4]);
    assert( tombs[5]);
    assert(!tombs[6]);
    assert( tombs[7]);
    assert( tombs[8]);
    assert( tombs[9]);
}

void test_ct_log2state () {
    String LOG{"@1000000001+A :rga, 't', 'e', 's', 't', @1000000007+A :1000000004+A rm, @1000000008+A :1000000004+A 'x';"};
    vector<bool> tombs{};
    Frame state;
    Status ok = ObjectLogToState<Frame>(state, Frame{LOG});
    assert(ok);
    assert(ScanRGA<Frame>(tombs, Frame{state}));
    assert( tombs[0]); // rga
    assert(!tombs[1]); // t
    assert(!tombs[2]); // e
    assert( tombs[3]); // s
    assert(!tombs[4]); // x  oopsie
    assert( tombs[5]); // rm
    assert(!tombs[6]); // t
}

void test_ct_log2state_cutoff () {
    String LOG{"@1000000001+A :rga, 't', 'e', 's', 't', @1000000007+A :1000000004+A rm, @1000000008+A :1000000004+A 'x';"};
    vector<bool> tombs{};
    Frame state;
    Status ok = ObjectLogToState<Frame>(state, Frame{LOG}, Uuid{"1000000007+A"});
    assert(ok);
    assert(ScanRGA<Frame>(tombs, Frame{state}));
    assert( tombs[0]);
    assert(!tombs[1]); // t
    assert(!tombs[2]); // e
    assert( tombs[3]); // s
    assert( tombs[4]); // rm 
    assert(!tombs[5]); // t
}

int main (int argn, char** args) {
    test_chain_merge();
    test_sibling_merge();
    test_multitree();
    test_ct_scan_all0();
    test_ct_scan_rm();
    test_ct_scan_rm2();
    test_ct_scan_rm_un();
    test_ct_scan_trash();
    test_ct_log2state();
    test_ct_log2state_cutoff();
    return 0;
}
