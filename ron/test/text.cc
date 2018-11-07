#include <iostream>
#include <cassert>
#include "ron/ron.hpp"

using namespace ron;
using namespace std;

typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Frame::Builder Builder;

string pattern (Frame frame) {
    string ret = "";
    auto c = frame.cursor();
    do {
        if (c.op().size()) ret.push_back('@');
        if (c.op().size()>1) ret.push_back(':');
        for(int i=2; i<c.op().size(); i++) {
            ret.push_back(ATOM_PUNCT[c.op().value(i).type()]);
        }
        ret.push_back(TERM_PUNCT[c.op().term()]);
    } while (c.Next());
    return ret;
}


int main (int argn, char** args) {
    Builder builder;
    AddOp(builder, "1+src", "lww");
    AddOp(builder, "2+orig", "1+src", "key", "value");
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
    assert(op.id()=="2+orig");
    assert(op.value_string(2, frame.data())=="key");
    assert(op.value_string(3, frame.data())=="value");
    assert(!cursor.Next());
    // TODO TextFrame::unescape()
    // TODO Op to see buf as &

    Frame digit{"@1A 234 56K;28'abc' 3, @id 3.1415 >uuid;"};
    Cursor cdigit = digit.cursor();
    assert(cdigit.valid());
    assert(cdigit.op().size()==4);
    assert(cdigit.op().value(2).type()==ATOM::INT);
    assert(cdigit.op().value(3).type()==ATOM::UUID);
    assert(cdigit.op().value_uuid(3)=="56K");
//    assert(cdigit.uuid(2)=="2B");
    assert(!cdigit.op().id().zero());
    assert(cdigit.op().ref().zero());

    assert(cdigit.Next()); // start state: space :)
    std::cerr<<cdigit.op().id().str()<<'\n';
    assert(cdigit.op().id()=="1A00000001");
    assert(cdigit.op().ref()=="1A");
    assert(cdigit.integer(2)==28);
    std::cerr<<cdigit.string(3)<<'\n';
    assert(cdigit.string(3)=="abc");
    assert(cdigit.integer(4)==3);

    assert(cdigit.Next());
    assert(cdigit.number(2)==3.1415); // :)
//    assert(cdigit.uuid(3)=="uuid");

    //assert(pattern(digit)=="@:>;@:='=;@:^>;");
    // [ ] int or uuid (sep name)
    // [x] 'str'123 separator :) fuck it
    // [x] ,1 fbreak - entry action

//    Builder builder2;

    /*AddOp(builder, "1+src", "lww");
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
    // TODO Op to see buf as &*/
}