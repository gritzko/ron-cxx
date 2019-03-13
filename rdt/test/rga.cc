#include <iostream>
#include <cassert>
#include "../../ron/ron.hpp"
#include "../rdt.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

using Frame = TextFrame;
typedef typename Frame::Cursor Cursor;
typedef RGArrayRDT<typename ron::TextFrame> RGA;
using Cursors = typename Frame::Cursors;

string despace (string orig) {
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
    Cursors pieces{Cursor{abc}, Cursor{def}};
    Frame abcdef;
    Status ok = MergeCursors<Frame>(abcdef, RGA_RDT, pieces);
    assert(ok);
    string correct = "@1+A :rga! 'a', 'b', 'c', @1000000004+B 'D', 'E', 'F', ";
    assert(despace(abcdef.data())==despace(correct));
}

void test_sibling_merge () {
    string parent = "@1+A :rga!";
    string childA = "@1a+B :1+A 'b';";
    string childB = "@1b+C :1+A 'a';";
    Frame ab;
    Cursors pieces{Cursor{parent}, Cursor{childA}, Cursor{childB}};
    assert(MergeCursors<Frame>(ab, RGA_RDT, pieces));
    string correct = "@1+A :rga! @1b+C 'a', @1a+B :1+A 'b', ";
    assert(despace(ab.data())==despace(correct));
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
    assert(ObjectLog2State<Frame>(state, RGA_RDT, Frame{LOG}));
    assert(ScanRGA<Frame>(tombs, Frame{state}));
    assert( tombs[0]);
    assert(!tombs[1]);
    assert(!tombs[2]);
    assert( tombs[3]);
    assert(!tombs[4]);
    assert( tombs[5]);
    assert(!tombs[6]);
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
    return 0;
}
