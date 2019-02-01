#include <iostream>
#include <cassert>
#include "ron/ron.hpp"

using namespace ron;
using namespace std;

typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Frame::Builder Builder;

void test_simple_meta () {
    string chain = "@1gN97b+gritzko :lww! 'key' 'value'";
}

void test_ct_basic () {
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
}

int main (int argn, char** args) {
    test_simple_meta();
    test_ct_basic();
}