#include <iostream>
#include <cassert>
#include "../../ron/ron.hpp"
#include "../rdt.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

typedef LastWriteWinsRDT<typename ron::TextFrame> TextLWW;
typedef TextFrame::Cursors Cursors;

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

int main (int argn, char** args) {

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
    assert(scan(abc)=="_,a,b,c");

    inputs.push_back(b2_builder.Release());

    Cursors i2 = cursors(inputs);
    lww.Merge(abbc_builder, i2);
    TextFrame abbc = abbc_builder.Release();
    assert(scan(abbc)=="_,a,b,c,b");

    lww.GC(ab2c_builder, abbc);
    TextFrame ab2c = ab2c_builder.Release();
    assert(scan(ab2c)=="_,a,c,b");

    Cursors i3 = cursors(inputs);
    lww.MergeGC(ab2c_builder2, i3);
    TextFrame ab2c2 = ab2c_builder2.Release();
    assert(scan(ab2c2)=="_,a,c,b");
    assert(ab2c.data()==ab2c2.data());

}
