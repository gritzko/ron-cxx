#include <iostream>
#include <cassert>
#include <unistd.h>
#include "../ron.hpp"
#include "../hash.hpp"
#define DEBUG 1

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
    builder.AppendNewOp(HEADER, Uuid{"1+src"}, Uuid{"lww"});
    builder.AppendNewOp(REDUCED, Uuid{"2+orig"}, Uuid{"1+src"}, "key", "value");
    TextFrame frame = builder.Release();
    const string &data = frame.data();
    Cursor cur = frame.cursor();
    SHA2 SRC_HASH{Uuid{"0+src"}};
    SHA2 LWW_HASH{Uuid{"lww"}};
//    IOStream out{IOSink{STDOUT_FILENO}};
//    WriteOpHashable<Frame, IOStream>(cur, out, SRC_HASH, LWW_HASH);
    SHA2Stream ophash;
    WriteOpHashable<Cursor, SHA2Stream>(cur, ophash, SRC_HASH, LWW_HASH);
    SHA2 OP_HASH;
    ophash.close(OP_HASH.bits_);
    string okhex =  "97fa0525e009867adffe5e2c71f93057dfb8293c25c27292cd4caf230a0e39ec";
    string okbase = "a~d59U09XcgV~athSV_lLyztAJlalcAIoKnk8ldEEUl";
    assert(OP_HASH.hex()==okhex);
    assert(OP_HASH.base64()==okbase);
    assert(SHA2::ParseBase64(okbase)==OP_HASH);
    assert(SHA2::valid(okbase));
    string not_a_hash = okbase;
    not_a_hash[SHA2::BASE64_SIZE-1] = '1';
    assert(!SHA2::valid(not_a_hash));
    SHA2 op2 = SHA2::ParseBase64(okbase);
    SHA2 op3 = SHA2::ParseHex(okhex);
    assert(op2.known_bits());
    assert(op2 == OP_HASH);
    assert(op3.known_bits());
    assert(op3 == OP_HASH);
}

void test_partial_match () {
    SHA2 a = SHA2::ParseHex("97fa0525e009867adffe5e2c71f93057dfb8293c25c27292cd4caf230a0e39ec");
    SHA2 a2 = SHA2::ParseHex("97fa");
    assert(a.known_bits());
    assert(a2.known_bits());
    assert(a.matches(a2));
    assert(a!=a2);
    SHA2 caps = SHA2::ParseHex("97fA0525E");
    SHA2 half = SHA2::ParseHex("97fa0");
    SHA2 badhalf = SHA2::ParseHex("97fa1");
    SHA2 nothex = SHA2::ParseHex("z");
    assert(caps.known_bits());
    assert(half.known_bits());
    assert(badhalf.known_bits());
    assert(caps.matches(a));
    assert(half.matches(a));
    assert(!badhalf.matches(a));
    assert(!nothex.known_bits());
}

int main (int argn, char** args) {
    test_serialization();
    test_partial_match();
    return 0;
}
