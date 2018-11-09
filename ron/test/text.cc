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


void test_basic_cycle () {
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
    cerr<<cursor.string(2)<<'\n';
    assert(cursor.string(2)=="key");
    assert(cursor.string(3)=="value");
    assert(!cursor.Next());
}

void test_optional_chars () {
    Frame opt{"@1A 234 56K;28'abc' 3, @id 3.1415 >uuid;"};
    Cursor copt = opt.cursor();
    assert(copt.valid());
    assert(copt.op().size()==4);
    assert(copt.op().value(2).type()==ATOM::INT);
    assert(copt.op().value(3).type()==ATOM::UUID);
    assert(copt.op().value_uuid(3)=="56K");
    assert(!copt.op().id().zero());
    assert(copt.op().ref().zero());

    assert(copt.Next()); // start state: space :)
    std::cerr<<copt.op().id().str()<<'\n';
    assert(copt.op().id()=="1A00000001");
    assert(copt.op().ref()=="1A");
    std::cerr<<copt.integer(2)<<'\n';
    assert(copt.integer(2)==28);
    std::cerr<<copt.string(3)<<'\n';
    assert(copt.string(3)=="abc");
    assert(copt.integer(4)==3);

    assert(copt.Next());
    assert(copt.number(2)==3.1415); // :)

    assert(!copt.Next());
    assert(!copt.valid());
}

void test_signs () {
    Frame signs{"@2:1 -1 ,-1.2, +1.23, -1.0e+2, -2.0e+1,"}; // FIXME 1e+1
    Cursor cur = signs.cursor();
    assert(cur.integer(2)==-1);
    assert(cur.Next());
    assert(cur.number(2)==-1.2);
    assert(cur.Next());
    assert(cur.number(2)==1.23);
    assert(cur.Next());
    assert(cur.number(2)==-100.0);
    assert(cur.Next());
    assert(cur.number(2)==-20);
    assert(!cur.Next());
}

void test_size_limits () {
    Frame toolong{"=1,=1000000000000000000001,"};
    Cursor cur = toolong.cursor();
    assert(cur.valid());
    assert(!cur.Next());
}

void test_string_escapes () {
    Builder builder;
    auto STR1 = "'esc'";
    auto STR2 = "=\r\n\t\\=";
    AddOp(builder, "1+a", "2+b", STR1, STR2);
    Frame cycle = builder.frame();
    Cursor cc = cycle.cursor();
    assert(cc.valid());
    std::cerr<<cc.string(2)<<'\n';
    std::cerr<<cc.string(3)<<'\n';
    assert(cc.string(2)==STR1);
    assert(cc.string(3)==STR2);

    //Frame good{" 'esc \\'', '\\u0020', '\\r\\n\\t\\\\', "};
    //Cursor cur = good.cursor();

}

int main (int argn, char** args) {
    test_basic_cycle();
    test_optional_chars();
    test_signs();
    test_size_limits();
    test_string_escapes();
    return 0;
}