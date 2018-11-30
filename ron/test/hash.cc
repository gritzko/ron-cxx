#include <iostream>
#include <cassert>
#include "ron/ron.hpp"
#include "ron/hash.hpp"

using namespace ron;
using namespace std;

typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Frame::Builder Builder;

void test_serialization () {
    Builder builder;
    AddOp(builder, "1+src", "lww");
    AddOp(builder, "2+orig", "1+src", "key", "value");
    TextFrame frame = builder.frame();
    const string &data = frame.data();
    Cursor cur = frame.cursor();
    SHA3_256 SRC_HASH;
    SHA3_256 LWW_HASH;
    hash_uuid("0+src", SRC_HASH);
    hash_uuid("lww", LWW_HASH);
    hash_feed<Frame, ostream>(cur, cout, SRC_HASH, LWW_HASH);
}

int main (int argn, char** args) {
    test_serialization();
    return 0;
}
