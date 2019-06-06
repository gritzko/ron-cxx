
#line 1 "ragel/utf8-parser.rl"
#include "slice.hpp"
#include "status.hpp"

using namespace std;

namespace ron {

#line 10 "ragel/utf8-parser.rl"

#line 15 "ron/utf8-parser.cc"
static const int UTF8CP_start = 4;
static const int UTF8CP_first_final = 4;
static const int UTF8CP_error = 0;

static const int UTF8CP_en_main = 4;

#line 11 "ragel/utf8-parser.rl"

Result ParseUtf8(Codepoints& into, const String& from) {
    CharRef p = reinterpret_cast<CharRef>(from.data());
    CharRef pe = p + from.size();
    CharRef eof = pe;
    Codepoint cp = 0;
    int cs = 0;

#line 32 "ron/utf8-parser.cc"
    { cs = UTF8CP_start; }

#line 19 "ragel/utf8-parser.rl"

#line 39 "ron/utf8-parser.cc"
    {
        if (p == pe) goto _test_eof;
        switch (cs) {
            case 4:
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr5;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr7;
                } else
                    goto tr6;
                goto tr4;
            tr0 :
#line 7 "ragel/./utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st5;
            tr4 :
#line 8 "ragel/./utf8-grammar.rl"
            {
                cp = (*p);
            }
                goto st5;
            tr8 :
#line 21 "ragel/utf8-parser.rl"
            {
                into.push_back(cp);
                cp = 0;
            }
#line 8 "ragel/./utf8-grammar.rl"
                { cp = (*p); }
                goto st5;
            st5:
                if (++p == pe) goto _test_eof5;
            case 5:
#line 82 "ron/utf8-parser.cc"
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr9;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr11;
                } else
                    goto tr10;
                goto tr8;
            st0:
                cs = 0;
                goto _out;
            tr2 :
#line 7 "ragel/./utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st1;
            tr5 :
#line 9 "ragel/./utf8-grammar.rl"
            {
                cp = (*p) & 0x1f;
            }
                goto st1;
            tr9 :
#line 21 "ragel/utf8-parser.rl"
            {
                into.push_back(cp);
                cp = 0;
            }
#line 9 "ragel/./utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st1;
            st1:
                if (++p == pe) goto _test_eof1;
            case 1:
#line 122 "ron/utf8-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr0;
                goto st0;
            tr3 :
#line 7 "ragel/./utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st2;
            tr6 :
#line 10 "ragel/./utf8-grammar.rl"
            {
                cp = (*p) & 0xf;
            }
                goto st2;
            tr10 :
#line 21 "ragel/utf8-parser.rl"
            {
                into.push_back(cp);
                cp = 0;
            }
#line 10 "ragel/./utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st2;
            st2:
                if (++p == pe) goto _test_eof2;
            case 2:
#line 147 "ron/utf8-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr2;
                goto st0;
            tr7 :
#line 11 "ragel/./utf8-grammar.rl"
            {
                cp = (*p) & 7;
            }
                goto st3;
            tr11 :
#line 21 "ragel/utf8-parser.rl"
            {
                into.push_back(cp);
                cp = 0;
            }
#line 11 "ragel/./utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st3;
            st3:
                if (++p == pe) goto _test_eof3;
            case 3:
#line 168 "ron/utf8-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr3;
                goto st0;
        }
    _test_eof5:
        cs = 5;
        goto _test_eof;
    _test_eof1:
        cs = 1;
        goto _test_eof;
    _test_eof2:
        cs = 2;
        goto _test_eof;
    _test_eof3:
        cs = 3;
        goto _test_eof;

    _test_eof : {}
        if (p == eof) {
            switch (cs) {
                case 5:
#line 21 "ragel/utf8-parser.rl"
                {
                    into.push_back(cp);
                    cp = 0;
                } break;
#line 189 "ron/utf8-parser.cc"
            }
        }

    _out : {}
    }

#line 28 "ragel/utf8-parser.rl"

    return cs >= UTF8CP_first_final ? OK : BADSYNTAX;
}

}  // namespace ron
