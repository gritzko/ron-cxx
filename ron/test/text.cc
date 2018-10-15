#include <iostream>
#include <cassert>
#include "ron/ron.hpp"

using namespace ron;
using namespace std;


int main (int argn, char** args) {
    TextFrame::Builder builder;
    AddOp(builder, "1+src", "lww");
    AddOp(builder, "2+src", "1+src", "key", "value");
    // TODO escaping
    // TODO coverage: uuid, float, int
    TextFrame frame = builder.frame();
    const string &data = frame.data();
    assert(data.find("1+src")!=string::npos);
    assert(data.find("key")!=string::npos);
    assert(data.find("value")!=string::npos);
    TextFrame::Cursor cursor = frame.cursor();
    const Op& op = cursor.op();
    assert(op.size()==2);
    assert(op.ref()==Uuid{"lww"});
    assert(op.id()=="1+src");
    assert(cursor.Next());
    assert(op.ref()=="1+src");
    assert(op.id()=="2+src");
    assert(op.value_string(2, frame.data())=="key");
    assert(op.value_string(3, frame.data())=="value");
    assert(!cursor.Next());
    // TODO TextFrame::unescape()
    // TODO Op to see buf as &
}