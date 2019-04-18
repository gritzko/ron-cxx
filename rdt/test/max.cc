#include <iostream>
#include <gtest/gtest.h>
#include "../../ron/ron.hpp"
#include "../rdt.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

using Frame = TextFrame;
using Max = MaxRDT<Frame>;
using Builder = typename Frame::Builder;
using Cursor = typename Frame::Cursor;
using Cursors = typename Frame::Cursors;

TEST (Max, Ints) {
    Max rdt;
    Builder res;
    Frame A{"@1kGIU+0046cGqZgm :max, =1;"};
    Frame B{"@1kGIu+0046cGqZgm :1kGIU+0046cGqZgm =2;"};
    Cursors ab{A.cursor(), B.cursor()};
    Status ok = rdt.Merge(res, ab);
    Frame AB = res.Release();
    ASSERT_EQ(AB.data(), "@1kGIU+0046cGqZgm :max,\n@1kGIu+0046cGqZgm 2;\n");
}

TEST (Max, Uuids) {
}

TEST (Max, Strange) {
}

int main (int argn, char** args) {
    ::testing::InitGoogleTest(&argn, args);
    return RUN_ALL_TESTS();
}
