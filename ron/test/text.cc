#include <iostream>
#include <gtest/gtest.h>
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


TEST(TextFrame, basic_cycle ) {
    Builder builder;
    String TIME1{"1+src"};
    String TIME2{"2+orig"};
    String LWW{"lww"};
    String KEY{"key"};
    String VALUE{"value"};
    builder.AppendNewOp(Uuid{TIME1}, Uuid{LWW});
    builder.AppendNewOp(Uuid{TIME2}, Uuid{TIME1}, KEY, VALUE);
    // TODO escaping
    // TODO coverage: uuid, float, int
    TextFrame frame = builder.Release();
    const String &data = frame.data();
    ASSERT_TRUE(data.find(TIME1)!=string::npos);
    ASSERT_TRUE(data.find(KEY)!=string::npos);
    ASSERT_TRUE(data.find(VALUE)!=string::npos);

    TextFrame::Cursor cursor = frame.cursor();
    const Op& op = cursor.op();
    ASSERT_EQ(op.size(), 2);
    ASSERT_TRUE(op.ref()==Uuid{LWW});
    ASSERT_TRUE(op.id().str()==TIME1);
    ASSERT_TRUE(cursor.term()==REDUCED);
    ASSERT_TRUE(cursor.Next());
    ASSERT_TRUE(cursor.term()==RAW);
    ASSERT_TRUE(op.ref()==TIME1);
    ASSERT_TRUE(op.id()==TIME2);
    ASSERT_TRUE(cursor.string(2)==KEY);
    ASSERT_TRUE(cursor.string(3)==VALUE);
    ASSERT_TRUE(!cursor.Next());
}

TEST(TextFrame, optional_chars) {
    String TANGLED{"@1A 234 56K;+9223372036854775807'abc' 3, @id 3.1415 >uuid;"};
    String ABC{"abc"};
    Frame opt{TANGLED};
    Cursor copt = opt.cursor();
    ASSERT_TRUE(copt.valid());
    ASSERT_TRUE(copt.op().size()==4);
    ASSERT_TRUE(copt.op().atom(2).type()==ATOM::INT);
    ASSERT_TRUE(copt.op().atom(3).type()==ATOM::UUID);
    ASSERT_TRUE(copt.op().uuid(3)=="56K");
    ASSERT_TRUE(!copt.op().id().zero());
    ASSERT_TRUE(copt.op().ref().zero());

    Status ok = copt.Next();
    ASSERT_TRUE(ok); // start state: space :)
    ASSERT_TRUE(copt.op().id()=="1A00000001");
    ASSERT_TRUE(copt.op().ref()=="1A");
    ASSERT_TRUE(copt.has(2, INT));
    //ASSERT_TRUE(copt.integer(2)==9223372036854775807L);
    ASSERT_TRUE(copt.string(3)==ABC);
    ASSERT_TRUE(copt.integer(4)==3);

    ASSERT_TRUE(copt.Next());
    ASSERT_TRUE(copt.number(2)==3.1415); // :)

    ASSERT_TRUE(!copt.Next());
    ASSERT_TRUE(!copt.valid());
}

TEST(TextFrame, signs ) {
    String SIGNS{"@2:1 -1 ,-1.2, +1.23,-1e+2, -2.0e+1,"};
    Frame signs{SIGNS};
    Cursor cur = signs.cursor();
    ASSERT_EQ(cur.integer(2), -1);
    ASSERT_TRUE(cur.Next());
    ASSERT_TRUE(cur.number(2)==-1.2);
    ASSERT_TRUE(cur.Next());
    ASSERT_TRUE(cur.number(2)==1.23);
    ASSERT_TRUE(cur.Next());
    ASSERT_TRUE(cur.number(2)==-100.0);
    ASSERT_TRUE(cur.Next());
    ASSERT_TRUE(cur.number(2)==-20);
    ASSERT_TRUE(!cur.Next());
}

TEST(TextFrame, size_limits ) {
    String OVERLIMIT{"=1,=1000000000000000000001,"};
    Frame toolong{OVERLIMIT};
    Cursor cur = toolong.cursor();
    ASSERT_TRUE(cur.valid());
    ASSERT_TRUE(!cur.Next());
}

TEST(TextFrame, string_escapes) {
    Builder builder;
    String STR1{"'esc'"};
    String STR2{"=\r\n\t\\="};
    builder.AppendNewOp(Uuid{"1+a"}, Uuid{"2+b"}, STR1, STR2);
    Frame cycle = builder.Release();
    Cursor cc = cycle.cursor();
    ASSERT_TRUE(cc.valid());
    ASSERT_TRUE(cc.string(2)==STR1);
    ASSERT_TRUE(cc.string(3)==STR2);

    // FIXME \u
    //Frame good{" 'esc \\'', '\\u0020', '\\r\\n\\t\\\\', "};
    //Cursor cur = good.cursor();
}

TEST(TextFrame, string_metrics ) {
    String BADUTF8{"@id :ref 'bad string \x80';"};
    Cursor bad{BADUTF8};
    ASSERT_TRUE(!bad.valid());
}

TEST(TextFrame, terms) {
    String COMMAS{"@1+A:2+B 1,2 ,\n,\t4   ,,"};
    Cursor c{COMMAS};
    int i = 1;
    while (c.Next()) i++;
    ASSERT_TRUE(i==5);
}

TEST(TextFrame, defaults ) {
    Frame::Builder b;
    String RAW{"@12345+test :lww; @1234500001+test :12345+test 'key' 'value';"};
    b.AppendFrame(Frame{RAW});
    Frame nice = b.Release();
    String CORRECT{"@12345+test :lww;\n 'key' 'value';\n"};
    ASSERT_TRUE(nice.data()==CORRECT);
    Cursor nc = nice.cursor();
    ASSERT_TRUE(nc.op().id()==Uuid{"12345+test"});
    ASSERT_TRUE(nc.op().ref()==Uuid{"lww"});
    nc.Next();
    ASSERT_TRUE(nc.op().id()==Uuid{"1234500001+test"});
    ASSERT_TRUE(nc.op().ref()==Uuid{"12345+test"});
}

TEST(TextFrame, span_spread ) {
    String RAW{"@1iDEKK+gYpLcnUnF6 :1iDEKA+gYpLcnUnF6 ('abcd' 4);"};
    Cursor c{RAW};
    ASSERT_TRUE(c.valid());
}

TEST(TextFrame, syntax_errors) {
    String INVALID{"@line+ok\n:bad/"};
    Cursor cur{Slice{INVALID}, false};
    Status ok = cur.Next();
    String MSG{"syntax error at line 2 col 5 (offset 13)"};
    ASSERT_TRUE(ok.comment()==MSG);
}

TEST(TextFrame, UTF16) {
    String PIKACHU{"'пикачу\\u0020ピカチュウ'!"};
    ASSERT_TRUE(PIKACHU.size()==36);
    Frame frame{PIKACHU};
    Cursor cur = frame.cursor();
    ASSERT_TRUE(cur.valid());
    ASSERT_TRUE(cur.has(2, STRING));
    Atom str = cur.atom(2);
    //auto parsed = frame.utf16string(str);
    //ASSERT_TRUE(parsed==u"пикачу ピカチュウ");
    //ASSERT_TRUE(parsed.size()==12);
    
    using StringIterator = typename Frame::StringIterator;
    Slice pikachu_slice = cur.string_slice(2);
    StringIterator i{pikachu_slice};
    Codepoints cps;
    while (i) {
        cps.push_back(*i);
        ++i;
    }
    ASSERT_EQ(cps.size(), 12);
    ASSERT_EQ(cps[6], 0x20);
}

TEST(TextFrame, END) {
    String frame{"@1kK7vk+0 :lww ;\n"};
    Cursor c{frame};
    ASSERT_TRUE(c.valid());
    ASSERT_EQ(c.Next(), Status::ENDOFFRAME);
}


int main (int argn, char** args) {
    ::testing::InitGoogleTest(&argn, args);
    return RUN_ALL_TESTS();
}
