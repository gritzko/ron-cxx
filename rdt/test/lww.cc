#include <iostream>
#include <cassert>
#include "ron/ron.hpp"
#include "rdt/lww.hpp"

using namespace ron;
using namespace std;

typedef LastWriteWins<typename ron::TextFrame> LWW;

string scan (const TextFrame& frame) {
    string ret;
    TextFrame::Cursor cur = frame.cursor();
    do {
        if (!ret.empty())
            ret.append(",");
        const Op& op = cur.op();
        if (op.size()>2 && op.value(2).type()==ATOM::STRING) {
            ret.append(cur.string(2));
        } else {
            ret.append("_");
        }
    } while (cur.Next());
    return ret;
}

int main (int argn, char** args) {

    TextFrame::Builder ab_builder, c_builder, abc_builder, b2_builder, 
    abbc_builder, ab2c_builder, ab2c_builder2;
    vector<TextFrame> inputs;
    LWW lww;

    AddOp(ab_builder, "1+src", "lww");
    AddOp(ab_builder, "2+src", "1+src", "a", "A");
    AddOp(ab_builder, "3+src", "2+src", "b", "B");
    AddOp(c_builder, "3+xyz", "2+src", "c", "C");
    AddOp(b2_builder, "4+xyz", "3+src", "b", "B2");

    inputs.push_back(ab_builder.frame());
    inputs.push_back(c_builder.frame());

    lww.Merge(abc_builder, inputs);
    TextFrame abc = abc_builder.frame();
    assert(scan(abc)=="_,a,b,c");

    inputs.push_back(b2_builder.frame());

    lww.Merge(abbc_builder, inputs);
    TextFrame abbc = abbc_builder.frame();
    assert(scan(abbc)=="_,a,b,c,b");

    lww.GC(ab2c_builder, abbc);
    TextFrame ab2c = ab2c_builder.frame();
    assert(scan(ab2c)=="_,a,c,b");

    lww.MergeGC(ab2c_builder2, inputs);
    TextFrame ab2c2 = ab2c_builder2.frame();
    assert(scan(ab2c2)=="_,a,c,b");
    assert(ab2c.data()==ab2c2.data());

}
