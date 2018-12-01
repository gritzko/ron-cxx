#include <iostream>
#include <cassert>
#include <unistd.h>
#include "ron/ron.hpp"
#include "ron/hash.hpp"

using namespace ron;
using namespace std;

typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Frame::Builder Builder;

struct IOSink {
    int out;
    IOSink(int fd) : out{fd} {}
    inline void update(uint8_t* buf, size_t size) {
        write(out, buf, size);
    }
};
typedef Stream<IOSink> IOStream;

void test_serialization () {
    Builder builder;
    AddOp(builder, "1+src", "lww");
    AddOp(builder, "2+orig", "1+src", "key", "value");
    TextFrame frame = builder.frame();
    const string &data = frame.data();
    Cursor cur = frame.cursor();
    SHA2 SRC_HASH;
    SHA2 LWW_HASH;
//    IOStream out{IOSink{STDOUT_FILENO}};
    hash_uuid("0+src", SRC_HASH);
    hash_uuid("lww", LWW_HASH);
//    WriteOpHashable<Frame, IOStream>(cur, out, SRC_HASH, LWW_HASH);
    SHA2Stream ophash;
    WriteOpHashable<Frame, SHA2Stream>(cur, ophash, SRC_HASH, LWW_HASH);
    SHA2 OP_HASH;
    ophash.close(OP_HASH);
    assert(hex(OP_HASH)=="97fa0525e009867adffe5e2c71f93057dfb8293c25c27292cd4caf230a0e39ec");
}

int main (int argn, char** args) {
    test_serialization();
    return 0;
}
