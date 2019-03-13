#include <iostream>
#include <cassert>
#include "../ron.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Frame::Builder Builder;

string pattern (const Frame& frame) {
    string ret;
    auto c = frame.cursor();
    do {
        if (c.op().size()) ret.push_back('@');
        if (c.op().size()>1) ret.push_back(':');
        for(int i=2; i<c.op().size(); i++) {
            ret.push_back(ATOM_PUNCT[c.op().atom(i).type()]);
        }
        ret.push_back(TERM_PUNCT[c.op().term()]);
    } while (c.Next());
    return ret;
}


void test_basic_cycle () {
    Builder builder;
    String TIME1{"1+src"};
    String TIME2{"2+orig"};
    String LWW{"lww"};
    String KEY{"key"};
    String VALUE{"value"};
    builder.AppendNewOp(HEADER, Uuid{TIME1}, Uuid{LWW});
    builder.AppendNewOp(REDUCED, Uuid{TIME2}, Uuid{TIME1}, KEY, VALUE);
    // TODO escaping
    // TODO coverage: uuid, float, int
    TextFrame frame = builder.frame();
    const String &data = frame.data();
    assert(data.find(TIME1)!=string::npos);
    assert(data.find(KEY)!=string::npos);
    assert(data.find(VALUE)!=string::npos);

    TextFrame::Cursor cursor = frame.cursor();
    const Op& op = cursor.op();
    assert(op.size()==2);
    assert(op.ref()==Uuid{LWW});
    assert(op.id().str()==TIME1);
    assert(cursor.term()==HEADER);
    assert(cursor.Next());
    assert(cursor.term()==REDUCED);
    assert(op.ref()==TIME1);
    assert(op.id()==TIME2);
    assert(cursor.string(2)==KEY);
    assert(cursor.string(3)==VALUE);
    assert(!cursor.Next());
}

void test_optional_chars () {
    String TANGLED{"@1A 234 56K;+9223372036854775807'abc' 3, @id 3.1415 >uuid;"};
    String ABC{"abc"};
    Frame opt{TANGLED};
    Cursor copt = opt.cursor();
    assert(copt.valid());
    assert(copt.op().size()==4);
    assert(copt.op().atom(2).type()==ATOM::INT);
    assert(copt.op().atom(3).type()==ATOM::UUID);
    assert(copt.op().uuid(3)=="56K");
    assert(!copt.op().id().zero());
    assert(copt.op().ref().zero());

    Status ok = copt.Next();
    assert(ok); // start state: space :)
    assert(copt.op().id()=="1A00000001");
    assert(copt.op().ref()=="1A");
    assert(copt.has(2, INT));
    assert(copt.integer(2)==9223372036854775807L);
    assert(copt.string(3)==ABC);
    assert(copt.integer(4)==3);

    assert(copt.Next());
    assert(copt.number(2)==3.1415); // :)

    assert(!copt.Next());
    assert(!copt.valid());
}

void test_signs () {
    String SIGNS{"@2:1 -1 ,-1.2, +1.23,-1e+2, -2.0e+1,"};
    Frame signs{SIGNS};
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
    String OVERLIMIT{"=1,=1000000000000000000001,"};
    Frame toolong{OVERLIMIT};
    Cursor cur = toolong.cursor();
    assert(cur.valid());
    assert(!cur.Next());
}

void test_string_escapes () {
    Builder builder;
    String STR1{"'esc'"};
    String STR2{"=\r\n\t\\="};
    builder.AppendNewOp(RAW, Uuid{"1+a"}, Uuid{"2+b"}, STR1, STR2);
    Frame cycle = builder.frame();
    Cursor cc = cycle.cursor();
    assert(cc.valid());
    assert(cc.string(2)==STR1);
    assert(cc.string(3)==STR2);

    // FIXME \u
    //Frame good{" 'esc \\'', '\\u0020', '\\r\\n\\t\\\\', "};
    //Cursor cur = good.cursor();
}

void test_string_metrics () {
    String BADUTF8{"@id :ref 'bad string \x80';"};
    Cursor bad{BADUTF8};
    assert(!bad.valid());
}

void test_terms() {
    String COMMAS{"@1+A:2+B 1,2 ,\n,\t4   ,,"};
    Cursor c{COMMAS};
    int i = 1;
    while (c.Next()) i++;
    assert(i==5);
}

void test_defaults () {
    Frame::Builder b;
    String RAW{"@12345+test :lww; @1234500001+test :12345+test 'key' 'value';"};
    b.AppendFrame(Frame{RAW});
    Frame nice = b.frame();
    String CORRECT{"@12345+test :lww;\n 'key' 'value';\n"};
    assert(nice.data()==CORRECT);
    Cursor nc = nice.cursor();
    assert(nc.op().id()==Uuid{"12345+test"});
    assert(nc.op().ref()==Uuid{"lww"});
    nc.Next();
    assert(nc.op().id()==Uuid{"1234500001+test"});
    assert(nc.op().ref()==Uuid{"12345+test"});
}

void test_span_spread () {
    String RAW{"@1iDEKK+gYpLcnUnF6 :1iDEKA+gYpLcnUnF6 ('abcd' 4);"};
    Cursor c{RAW};
    assert(c.valid());
}

void test_syntax_errors () {
    String INVALID{"@line+ok\n:bad/"};
    Cursor cur{Slice{INVALID}, false};
    Status ok = cur.Next();
    String MSG{"syntax error at line 2 col 5 (offset 13)"};
    assert(ok.comment()==MSG);
}

void test_utf16 () {
    String PIKACHU{"'пикачу\\u0020ピカチュウ'!"};
    assert(PIKACHU.size()==36);
    Frame frame{PIKACHU};
    Cursor cur = frame.cursor();
    assert(cur.valid());
    assert(cur.has(2, STRING));
    Atom str = cur.atom(2);
    auto parsed = frame.utf16string(str);
    assert(parsed==u"пикачу ピカチュウ");
    assert(parsed.size()==12);
}

int main (int argn, char** args) {
    test_basic_cycle();
    test_optional_chars();
    test_signs();
    test_size_limits();
    test_string_escapes();
    test_terms();
    test_defaults();
    test_string_metrics();
    test_span_spread();
    test_syntax_errors();
    test_utf16();
    return 0;
}
