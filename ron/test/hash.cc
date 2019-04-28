#include <iostream>
#include <gtest/gtest.h>
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

TEST (SHA2, Serialization) {
    Builder builder;
    builder.AppendNewOp(Uuid{"1+src"}, Uuid{"lww"});
    builder.AppendNewOp(Uuid{"2+orig"}, Uuid{"1+src"}, "key", "value");
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
    ASSERT_EQ(OP_HASH.hex(), okhex);
    ASSERT_EQ(OP_HASH.base64(), okbase);
    ASSERT_EQ(SHA2::ParseBase64(okbase), OP_HASH);
    ASSERT_TRUE(SHA2::valid(okbase));
    string not_a_hash = okbase;
    not_a_hash[SHA2::BASE64_SIZE-1] = '1';
    ASSERT_FALSE(SHA2::valid(not_a_hash));
    SHA2 op2 = SHA2::ParseBase64(okbase);
    SHA2 op3 = SHA2::ParseHex(okhex);
    ASSERT_GE(op2.known_bits(), 0);
    ASSERT_EQ(op2, OP_HASH);
    ASSERT_GE(op3.known_bits(), 0);
    ASSERT_EQ(op3, OP_HASH);
}

TEST (SHA2, PartialMatch) {
    SHA2 a = SHA2::ParseHex("97fa0525e009867adffe5e2c71f93057dfb8293c25c27292cd4caf230a0e39ec");
    SHA2 a2 = SHA2::ParseHex("97fa");
    ASSERT_TRUE(a.known_bits());
    ASSERT_TRUE(a2.known_bits());
    ASSERT_TRUE(a.matches(a2));
    ASSERT_NE(a, a2);
    SHA2 caps = SHA2::ParseHex("97fA0525E");
    SHA2 half = SHA2::ParseHex("97fa0");
    SHA2 badhalf = SHA2::ParseHex("97fa1");
    SHA2 nothex = SHA2::ParseHex("z");
    ASSERT_TRUE(caps.known_bits());
    ASSERT_TRUE(half.known_bits());
    ASSERT_TRUE(badhalf.known_bits());
    ASSERT_TRUE(caps.matches(a));
    ASSERT_TRUE(half.matches(a));
    ASSERT_FALSE(badhalf.matches(a));
    ASSERT_FALSE(nothex.known_bits());
}


int main (int argn, char** args) {
    ::testing::InitGoogleTest(&argn, args);
    return RUN_ALL_TESTS();
}

