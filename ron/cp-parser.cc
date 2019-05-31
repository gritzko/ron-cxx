
#line 1 "ragel/cp-parser.rl"
#include <iostream>
#include "encdec.hpp"
#include "text.hpp"

using namespace std;

namespace ron {

#line 10 "ragel/cp-parser.rl"

#line 15 "ron/cp-parser.cc"
static const int CP_start = 9;
static const int CP_first_final = 9;
static const int CP_error = 0;

static const int CP_en_main = 9;

#line 11 "ragel/cp-parser.rl"

Result TextFrame::Cursor::NextCodepoint(Atom& a) const {
    if (!a.value.cp_size) {
        a.value.cp = 0;
        return ENDOFINPUT;
    }
    Slice str_data = data(a);
    CharRef p = str_data.begin();
    CharRef pe = str_data.end();
    CharRef pb = p;
    CharRef eof = 0;
    Codepoint cp = 0;
    fsize_t cp_size{0};
    int cs = 0;

#line 39 "ron/cp-parser.cc"
    { cs = CP_start; }

#line 26 "ragel/cp-parser.rl"

#line 46 "ron/cp-parser.cc"
    {
        if (p == pe) goto _test_eof;
        switch (cs) {
            case 9:
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto st0;
                    case 92u:
                        goto st1;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr12;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr14;
                } else
                    goto tr13;
                goto tr10;
            tr0 :
#line 58 "ragel/./text-grammar.rl"
            {
                cp = decode_esc((*p));
            }
                goto st10;
            tr7 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st10;
            tr10 :
#line 8 "ragel/././utf8-grammar.rl"
            {
                cp = (*p);
            }
                goto st10;
            tr15 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 28 "ragel/cp-parser.rl"
                {
                    --p;
                    {
                        p++;
                        cs = 10;
                        goto _out;
                    }
                }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st10;
            tr20 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 28 "ragel/cp-parser.rl"
                {
                    --p;
                    {
                        p++;
                        cs = 10;
                        goto _out;
                    }
                }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st10;
            st10:
                if (++p == pe) goto _test_eof10;
            case 10:
#line 112 "ron/cp-parser.cc"
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto st0;
                    case 92u:
                        goto tr16;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr17;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr19;
                } else
                    goto tr18;
                goto tr15;
            st0:
                cs = 0;
                goto _out;
            tr16 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 28 "ragel/cp-parser.rl"
                {
                    --p;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                goto st1;
            tr21 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 28 "ragel/cp-parser.rl"
                {
                    --p;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                goto st1;
            st1:
                if (++p == pe) goto _test_eof1;
            case 1:
#line 159 "ron/cp-parser.cc"
                switch ((*p)) {
                    case 34u:
                        goto tr0;
                    case 39u:
                        goto tr0;
                    case 47u:
                        goto tr0;
                    case 92u:
                        goto tr0;
                    case 98u:
                        goto tr0;
                    case 110u:
                        goto tr0;
                    case 114u:
                        goto tr0;
                    case 116u:
                        goto tr0;
                    case 117u:
                        goto st2;
                }
                goto st0;
            st2:
                if (++p == pe) goto _test_eof2;
            case 2:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st3;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st3;
                } else
                    goto st3;
                goto st0;
            st3:
                if (++p == pe) goto _test_eof3;
            case 3:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st4;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st4;
                } else
                    goto st4;
                goto st0;
            st4:
                if (++p == pe) goto _test_eof4;
            case 4:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st5;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st5;
                } else
                    goto st5;
                goto st0;
            st5:
                if (++p == pe) goto _test_eof5;
            case 5:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st11;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st11;
                } else
                    goto st11;
                goto st0;
            st11:
                if (++p == pe) goto _test_eof11;
            case 11:
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto st0;
                    case 92u:
                        goto tr21;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr22;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr24;
                } else
                    goto tr23;
                goto tr20;
            tr8 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st6;
            tr12 :
#line 9 "ragel/././utf8-grammar.rl"
            {
                cp = (*p) & 0x1f;
            }
                goto st6;
            tr17 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 28 "ragel/cp-parser.rl"
                {
                    --p;
                    {
                        p++;
                        cs = 6;
                        goto _out;
                    }
                }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st6;
            tr22 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 28 "ragel/cp-parser.rl"
                {
                    --p;
                    {
                        p++;
                        cs = 6;
                        goto _out;
                    }
                }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st6;
            st6:
                if (++p == pe) goto _test_eof6;
            case 6:
#line 283 "ron/cp-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr7;
                goto st0;
            tr9 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st7;
            tr13 :
#line 10 "ragel/././utf8-grammar.rl"
            {
                cp = (*p) & 0xf;
            }
                goto st7;
            tr18 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 28 "ragel/cp-parser.rl"
                {
                    --p;
                    {
                        p++;
                        cs = 7;
                        goto _out;
                    }
                }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st7;
            tr23 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 28 "ragel/cp-parser.rl"
                {
                    --p;
                    {
                        p++;
                        cs = 7;
                        goto _out;
                    }
                }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st7;
            st7:
                if (++p == pe) goto _test_eof7;
            case 7:
#line 321 "ron/cp-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr8;
                goto st0;
            tr14 :
#line 11 "ragel/././utf8-grammar.rl"
            {
                cp = (*p) & 7;
            }
                goto st8;
            tr19 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 28 "ragel/cp-parser.rl"
                {
                    --p;
                    {
                        p++;
                        cs = 8;
                        goto _out;
                    }
                }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st8;
            tr24 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 28 "ragel/cp-parser.rl"
                {
                    --p;
                    {
                        p++;
                        cs = 8;
                        goto _out;
                    }
                }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st8;
            st8:
                if (++p == pe) goto _test_eof8;
            case 8:
#line 355 "ron/cp-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr9;
                goto st0;
        }
    _test_eof10:
        cs = 10;
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
    _test_eof4:
        cs = 4;
        goto _test_eof;
    _test_eof5:
        cs = 5;
        goto _test_eof;
    _test_eof11:
        cs = 11;
        goto _test_eof;
    _test_eof6:
        cs = 6;
        goto _test_eof;
    _test_eof7:
        cs = 7;
        goto _test_eof;
    _test_eof8:
        cs = 8;
        goto _test_eof;

    _test_eof : {}
        if (p == eof) {
            switch (cs) {
                case 10:
#line 43 "ragel/./text-grammar.rl"
                {
                    cp_size++;
                }
#line 28 "ragel/cp-parser.rl"
                    {
                        --p;
                        {
                            p++;
                            cs = 0;
                            goto _out;
                        }
                    }
                    break;
                case 11:
#line 59 "ragel/./text-grammar.rl"
                {
                    cp = decode_hex_cp(Slice{p - 4, 4});
                }
#line 43 "ragel/./text-grammar.rl"
                    { cp_size++; }
#line 28 "ragel/cp-parser.rl"
                    {
                        --p;
                        {
                            p++;
                            cs = 0;
                            goto _out;
                        }
                    }
                    break;
#line 393 "ron/cp-parser.cc"
            }
        }

    _out : {}
    }

#line 32 "ragel/cp-parser.rl"

    if (cs != CP_error) {
        a.value.cp = cp;
        --a.value.cp_size;
        a.origin.as_range.consume(p - pb);
        assert(a.origin.as_range.valid());
        return OK;
    } else {
        a.value.cp = 0;
        return BADSYNTAX;
    }
}

}  // namespace ron
