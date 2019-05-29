
#line 1 "ragel/string-parser.rl"
#include <iostream>
#include <string>
#include "stdlib.h"
#include "text.hpp"

#line 26 "ragel/string-parser.rl"

namespace ron {

using namespace std;

using iterator = const unsigned char*;

void push_back_codepoint(u16string& res, uint32_t codepoint) {
    if (codepoint < 0xd800) {
        res.push_back((char16_t)codepoint);
    } else if (codepoint >= 0xe000 && codepoint < 0x10000) {
        res.push_back((char16_t)codepoint);
    } else {
        res.push_back(0xd800 + (codepoint >> 10));
        res.push_back(0xdc00 + (codepoint & 1023));
    }
}

void push_back_uniesc(u16string& res, iterator b, iterator e) {
    int sz = e - b;
    assert(sz == 6);
    char num[5];
    memcpy(num, b + 2, 4);
    num[4] = 0;
    uint32_t codepoint = strtol(num, NULL, 16);
    push_back_codepoint(res, codepoint);
    // FIXME validity
    // FIXME surrogates
}

void push_back_esc(u16string& res, iterator b, iterator e) {
    assert(*b == '\\');
    unsigned char c{*(b + 1)};
    switch (c) {
        case 'b':
            c = '\b';
            break;
        case 'f':
            c = '\f';
            break;
        case 'n':
            c = '\n';
            break;
        case 'r':
            c = '\r';
            break;
        case 't':
            c = '\t';
            break;
    }
    res.push_back(c);
}

void push_back_cp(u16string& res, iterator b, iterator e) {
    // deserialize
    const unsigned int sz = e - b;
    assert(sz && sz <= 4);
    static uint8_t MASK[5] = {0, 127, 31, 15, 7};
    uint32_t codepoint{static_cast<uint32_t>(*b & MASK[sz])};
    switch (sz) {
        case 4:
            codepoint <<= 6;
            codepoint |= *(++b) & 63;
        case 3:
            codepoint <<= 6;
            codepoint |= *(++b) & 63;
        case 2:
            codepoint <<= 6;
            codepoint |= *(++b) & 63;
        case 1:
            break;
        default:
            assert(false);
    }
    // serialized
    push_back_codepoint(res, codepoint);
}

u16string TextFrame::utf16string(Atom str_atom) const {
    assert(str_atom.origin().flags() == STRING << 2);

    Range range = str_atom.origin().range();
    fsize_t offset = range.offset, size = range.length;

    u16string ret;
    ret.reserve(size);

    iterator pb = (iterator)data_.data() + offset;
    auto pe = pb + size;
    auto p = pb;
    auto eof = pe;
    int cs = 0;

    iterator uniesc, esc, cp;

#line 115 "ragel/string-parser.rl"

#line 102 "ron/string-parser.cc"
    static const int UTF8ESC_start = 9;
    static const int UTF8ESC_first_final = 9;
    static const int UTF8ESC_error = 0;

    static const int UTF8ESC_en_main = 9;

#line 116 "ragel/string-parser.rl"

#line 112 "ron/string-parser.cc"
    { cs = UTF8ESC_start; }

#line 117 "ragel/string-parser.rl"

#line 120 "ron/string-parser.cc"
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
                        goto tr11;
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
            tr3 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st10;
            tr10 :
#line 18 "ragel/string-parser.rl"
            {
                cp = p;
            }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st10;
            tr15 :
#line 19 "ragel/string-parser.rl"
            {
                push_back_cp(ret, cp, p);
            }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st10;
            tr20 :
#line 16 "ragel/string-parser.rl"
            {
                push_back_esc(ret, esc, p);
            }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st10;
            tr25 :
#line 66 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 13 "ragel/string-parser.rl"
                { push_back_uniesc(ret, uniesc, p); }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st10;
            st10:
                if (++p == pe) goto _test_eof10;
            case 10:
#line 188 "ron/string-parser.cc"
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
            tr11 :
#line 12 "ragel/string-parser.rl"
            {
                uniesc = p;
            }
#line 15 "ragel/string-parser.rl"
                { esc = p; }
                goto st1;
            tr16 :
#line 19 "ragel/string-parser.rl"
            {
                push_back_cp(ret, cp, p);
            }
#line 12 "ragel/string-parser.rl"
                { uniesc = p; }
#line 15 "ragel/string-parser.rl"
                { esc = p; }
                goto st1;
            tr21 :
#line 16 "ragel/string-parser.rl"
            {
                push_back_esc(ret, esc, p);
            }
#line 12 "ragel/string-parser.rl"
                { uniesc = p; }
#line 15 "ragel/string-parser.rl"
                { esc = p; }
                goto st1;
            tr26 :
#line 66 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 13 "ragel/string-parser.rl"
                { push_back_uniesc(ret, uniesc, p); }
#line 12 "ragel/string-parser.rl"
                { uniesc = p; }
#line 15 "ragel/string-parser.rl"
                { esc = p; }
                goto st1;
            st1:
                if (++p == pe) goto _test_eof1;
            case 1:
#line 249 "ron/string-parser.cc"
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
                        goto st5;
                }
                goto st0;
            tr0 :
#line 65 "ragel/./text-grammar.rl"
            {
                cp = decode_esc((*p));
            }
                goto st11;
            st11:
                if (++p == pe) goto _test_eof11;
            case 11:
#line 270 "ron/string-parser.cc"
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
            tr4 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st2;
            tr12 :
#line 18 "ragel/string-parser.rl"
            {
                cp = p;
            }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st2;
            tr17 :
#line 19 "ragel/string-parser.rl"
            {
                push_back_cp(ret, cp, p);
            }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st2;
            tr22 :
#line 16 "ragel/string-parser.rl"
            {
                push_back_esc(ret, esc, p);
            }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st2;
            tr27 :
#line 66 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 13 "ragel/string-parser.rl"
                { push_back_uniesc(ret, uniesc, p); }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st2;
            st2:
                if (++p == pe) goto _test_eof2;
            case 2:
#line 332 "ron/string-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr3;
                goto st0;
            tr5 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st3;
            tr13 :
#line 18 "ragel/string-parser.rl"
            {
                cp = p;
            }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st3;
            tr18 :
#line 19 "ragel/string-parser.rl"
            {
                push_back_cp(ret, cp, p);
            }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st3;
            tr23 :
#line 16 "ragel/string-parser.rl"
            {
                push_back_esc(ret, esc, p);
            }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st3;
            tr28 :
#line 66 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 13 "ragel/string-parser.rl"
                { push_back_uniesc(ret, uniesc, p); }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st3;
            st3:
                if (++p == pe) goto _test_eof3;
            case 3:
#line 376 "ron/string-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr4;
                goto st0;
            tr14 :
#line 18 "ragel/string-parser.rl"
            {
                cp = p;
            }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st4;
            tr19 :
#line 19 "ragel/string-parser.rl"
            {
                push_back_cp(ret, cp, p);
            }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st4;
            tr24 :
#line 16 "ragel/string-parser.rl"
            {
                push_back_esc(ret, esc, p);
            }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st4;
            tr29 :
#line 66 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 13 "ragel/string-parser.rl"
                { push_back_uniesc(ret, uniesc, p); }
#line 18 "ragel/string-parser.rl"
                { cp = p; }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st4;
            st4:
                if (++p == pe) goto _test_eof4;
            case 4:
#line 416 "ron/string-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr5;
                goto st0;
            st5:
                if (++p == pe) goto _test_eof5;
            case 5:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st6;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st6;
                } else
                    goto st6;
                goto st0;
            st6:
                if (++p == pe) goto _test_eof6;
            case 6:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st7;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st7;
                } else
                    goto st7;
                goto st0;
            st7:
                if (++p == pe) goto _test_eof7;
            case 7:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st8;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st8;
                } else
                    goto st8;
                goto st0;
            st8:
                if (++p == pe) goto _test_eof8;
            case 8:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st12;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st12;
                } else
                    goto st12;
                goto st0;
            st12:
                if (++p == pe) goto _test_eof12;
            case 12:
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto st0;
                    case 92u:
                        goto tr26;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr27;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr29;
                } else
                    goto tr28;
                goto tr25;
        }
    _test_eof10:
        cs = 10;
        goto _test_eof;
    _test_eof1:
        cs = 1;
        goto _test_eof;
    _test_eof11:
        cs = 11;
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
    _test_eof6:
        cs = 6;
        goto _test_eof;
    _test_eof7:
        cs = 7;
        goto _test_eof;
    _test_eof8:
        cs = 8;
        goto _test_eof;
    _test_eof12:
        cs = 12;
        goto _test_eof;

    _test_eof : {}
        if (p == eof) {
            switch (cs) {
                case 11:
#line 16 "ragel/string-parser.rl"
                {
                    push_back_esc(ret, esc, p);
                } break;
                case 10:
#line 19 "ragel/string-parser.rl"
                {
                    push_back_cp(ret, cp, p);
                } break;
                case 12:
#line 66 "ragel/./text-grammar.rl"
                {
                    cp = decode_hex_cp(Slice{p - 4, 4});
                }
#line 13 "ragel/string-parser.rl"
                    { push_back_uniesc(ret, uniesc, p); }
                    break;
#line 528 "ron/string-parser.cc"
            }
        }

    _out : {}
    }

#line 123 "ragel/string-parser.rl"

    // The string was already pre-parsed by Cursor::Next()
    assert(cs != 0);

    return ret;
}

}  // namespace ron
