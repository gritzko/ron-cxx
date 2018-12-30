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

int main (int argn, char** args) {
    test_simple_meta();
}