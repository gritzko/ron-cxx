#include <iostream>
#include <cassert>
#include "text.hpp"
#include "frames.hpp"

using namespace ron;
using namespace std;


int main (int argn, char** args) {
    TextFrame::Builder builder;
    Op new_op;
    AddOp(builder, "1+src", "lww");
    AddOp(builder, "2+src", "1+src", "key", "value");
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
    assert(GetString(cursor, 0)=="key");
    assert(GetString(cursor, 1)=="value");
    assert(!cursor.Next());
}