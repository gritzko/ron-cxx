#include <iostream>
#include "gtest/gtest.h"
#include "../../ron/ron.hpp"
#include "../rdt.hpp"
#include "../rdt/lww_obj.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

using Frame = TextFrame;
using TextLWW = LastWriteWinsRDT<Frame>;
using Cursors = TextFrame::Cursors;

string scan (const TextFrame& frame) {
    string ret;
    TextFrame::Cursor cur = frame.cursor();
    do {
        if (!ret.empty())
            ret.append(",");
        const Op& op = cur.op();
        if (op.size()>2 && op.atom(2).type()==ATOM::STRING) {
            ret.append(cur.string(2));
        } else {
            ret.append("_");
        }
    } while (cur.Next());
    return ret;
}


TEST(LWW, Reduction) {
    TextFrame::Builder ab_builder, c_builder, abc_builder, b2_builder,
    abbc_builder, ab2c_builder, ab2c_builder2;
    vector<TextFrame> inputs;
    TextLWW lww;

    ab_builder.AppendNewOp(Uuid{"1+src"}, Uuid{"lww"});
    ab_builder.AppendNewOp(Uuid{"2+src"}, Uuid{"1+src"}, "a", "A");
    ab_builder.AppendNewOp(Uuid{"3+src"}, Uuid{"2+src"}, "b", "B");
    c_builder.AppendNewOp(Uuid{"3+xyz"}, Uuid{"2+src"}, "c", "C");
    b2_builder.AppendNewOp(Uuid{"4+xyz"}, Uuid{"3+src"}, "b", "B2");

    inputs.push_back(ab_builder.Release());
    inputs.push_back(c_builder.Release());

    Cursors i1 = cursors(inputs);
    lww.Merge(abc_builder, i1);
    TextFrame abc = abc_builder.Release();
    ASSERT_TRUE(scan(abc)=="_,a,b,c");

    inputs.push_back(b2_builder.Release());

    Cursors i2 = cursors(inputs);
    lww.Merge(abbc_builder, i2);
    TextFrame abbc = abbc_builder.Release();
    ASSERT_TRUE(scan(abbc)=="_,a,b,c,b");

    lww.GC(ab2c_builder, abbc);
    TextFrame ab2c = ab2c_builder.Release();
    ASSERT_TRUE(scan(ab2c)=="_,a,c,b");

    Cursors i3 = cursors(inputs);
    lww.MergeGC(ab2c_builder2, i3);
    TextFrame ab2c2 = ab2c_builder2.Release();
    ASSERT_TRUE(scan(ab2c2)=="_,a,c,b");
    ASSERT_TRUE(ab2c.data()==ab2c2.data());
}

TEST(LWW, Object) {
    Frame init{"@12345+orig :lww, abc 123, str 'string';"};
    LWWObject<Frame> obj{init};
    ASSERT_EQ(obj.integer(Uuid{"abc"}), 123);
    ASSERT_EQ(obj.string(Uuid{"abc"}), "");
    ASSERT_EQ(obj.string(Uuid{"str"}), "string");
    Frame update{"@123456+orig :12345+orig abc 234;"};
    obj.Update(update);
    ASSERT_EQ(obj.integer(Uuid{"abc"}), 234);
}

TEST(RDT, SplitLog) {
    using Frames = std::vector<Frame>;
    Frame trivial{" @1lD5lN+A :lww, 'A' 'a', 'B' 'b', @1lD5z+B :1lD5lN0001+A 'C' 'c'; "};
    Cursors chains;
    ASSERT_TRUE(SplitLogIntoChains(chains, trivial, Uuid{"1lD5z+A"}));
    Frames splits;
    ASSERT_TRUE(Reserialize(splits, chains));
    ASSERT_TRUE(CompareFrames<Frame>(splits[0], Frame{" @1lD5lN+A :lww, 'A' 'a', 'B' 'b'; "}));
    ASSERT_TRUE(CompareFrames<Frame>(splits[1], Frame{" @1lD5z+B :1lD5lN0001+A 'C' 'c'; "}));
    ASSERT_EQ(splits.size(), 2);
}

int main (int argn, char** args) {
    ::testing::InitGoogleTest(&argn, args);
    return RUN_ALL_TESTS();
}
