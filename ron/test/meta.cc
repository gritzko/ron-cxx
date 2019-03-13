#include <iostream>
#include <cassert>
#include "../ron.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Frame::Builder Builder;

void test_simple_meta () {
    string chain = "@1gN97b+gritzko :lww! 'key' 'value'";
}

/*void test_ct_basic () {
    Uuid head{"1i08e4+gYpLcnUnF6"};
    string frame{"@1i08e4+gYpLcnUnF6 :rga! 'a', 'b', 'c',"};
    CT ct;
    TextFrame::Cursor cur{frame};
    ct.AppendAll(cur);
    auto i = ct.begin();
    assert(*i==head);
    ++i;
    assert(*i==head.inc());
    ++i;
    assert(*i==head.inc(2));
    ++i;
    assert(*i==head.inc(3));
    --i;
    assert(*i==head.inc(2));
    --i;
    assert(*i==head.inc());
    i.CausallyPrev();
    assert(*i==head);
}*/

/*void test_ct_path () {
    string frame{"@1i08e4+path :rga! 'a', @1i08z+path 'b', @1i08k+path :1i08e4+path 'c', 'd',"};
    fsize_t depths[] = {0, 1, 2, 1, 2};
    Cursor c{frame};
    CTPath path{c.id()};
    int p=0;
    assert(path.depth() == depths[p]);
    assert(path.position() == p);
    while (c.Next()) {
        ++p;
        path.AddNext(c);
        assert(path.depth() == depths[p]);
        assert(path.position() == p);
    }
    assert(p== sizeof(depths)/sizeof(fsize_t) - 1);
}

void test_ct_path_fail () {
    string frame{"@1i08e4+path :rga! 'a', @1i08z+path 'b', @1i08k+path :1i0FAIL+path 'c',"};
    Cursor c{frame};
    CTPath path{c.id()};
    c.Next();
    assert(path.AddNext(c)); //a
    c.Next();
    assert(path.AddNext(c)); //b
    c.Next();
    assert(path.AddNext(c)==Status::CAUSEBREAK); //c
}*/
/*
void test_ct_scan_rmun () {
    string frame{"@1i08e4+path :rga! 'a', 'b', @1i08k+path rm, un, @1i08e40003+path :1i08e40002+path 'd',"};
    Cursor c{frame};
    CTScan<Cursor> scan{c};
    const vector<bool>& tombs = scan.visibility();
    assert(!tombs[0]);
    assert(!tombs[1]);
    assert(!tombs[2]);
    assert( tombs[3]);
    assert( tombs[4]);
    assert(!tombs[5]);
}
*/

void test_inc_stack () {
    inc_stack<fsize_t> is;
    for(fsize_t i=0; i<1000; i++) {
        is.push_back(i);
    }
    assert(is.size()==1000);
    assert(is.span_size()==1);
    is.push_back(0);
    assert(is.size()==1001);
    assert(is.span_size()==2);
    is.pop_back();
    is.pop_back();
    assert(is.size()==999);
    assert(is.span_size()==1);
}

int main (int argn, char** args) {
    test_simple_meta();
    test_inc_stack();
    //test_ct_basic();
    //test_ct_path();
    //test_ct_path_fail();
    //test_ct_scan_all0();
    //test_ct_scan_rm();
    //test_ct_scan_rmun();
}
