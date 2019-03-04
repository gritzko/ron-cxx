
#line 1 "ragel/uuid-parser.rl"
#include "uuid.hpp"

namespace ron {

Uuid::Uuid(Slice data) {
#line 8 "ragel/uuid-parser.rl"

#line 13 "ron/uuid-parser.cc"
    static const int UUID_start = 1;
    static const int UUID_first_final = 4;
    static const int UUID_error = 0;

    static const int UUID_en_main = 1;

#line 9 "ragel/uuid-parser.rl"

    CharRef pb = data.buf_;
    CharRef pe = pb + data.size_;
    CharRef p = pb;
    CharRef eof = pe;
    CharRef uuidb{p}, wordb{p};
    int cs = 0;
    Slice value{}, origin{};
    char variety{'0'}, version{'$'};

#line 33 "ron/uuid-parser.cc"
    { cs = UUID_start; }

#line 38 "ron/uuid-parser.cc"
    {
        if (p == pe) goto _test_eof;
        switch (cs) {
            case 1:
                switch ((*p)) {
                    case 95u:
                        goto tr2;
                    case 126u:
                        goto tr2;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr0;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr2;
                    } else if ((*p) >= 71u)
                        goto tr2;
                } else
                    goto tr0;
                goto st0;
            st0:
                cs = 0;
                goto _out;
            tr0 :
#line 6 "ragel/./uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/./uuid-grammar.rl"
                { wordb = p; }
                goto st4;
            st4:
                if (++p == pe) goto _test_eof4;
            case 4:
#line 79 "ron/uuid-parser.cc"
                switch ((*p)) {
                    case 43u:
                        goto tr5;
                    case 45u:
                        goto tr5;
                    case 47u:
                        goto tr6;
                    case 95u:
                        goto st6;
                    case 126u:
                        goto st6;
                }
                if ((*p) < 48u) {
                    if (36u <= (*p) && (*p) <= 37u) goto tr5;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st6;
                    } else if ((*p) >= 65u)
                        goto st6;
                } else
                    goto st6;
                goto st0;
            tr5 :
#line 14 "ragel/./uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 15 "ragel/./uuid-grammar.rl"
                { version = (*p); }
                goto st2;
            st2:
                if (++p == pe) goto _test_eof2;
            case 2:
#line 109 "ron/uuid-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr3;
                    case 126u:
                        goto tr3;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr3;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr3;
                } else
                    goto tr3;
                goto st0;
            tr3 :
#line 16 "ragel/./uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st5;
            st5:
                if (++p == pe) goto _test_eof5;
            case 5:
#line 131 "ron/uuid-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto st5;
                    case 126u:
                        goto st5;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st5;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto st5;
                } else
                    goto st5;
                goto st0;
            tr6 :
#line 12 "ragel/./uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st3;
            st3:
                if (++p == pe) goto _test_eof3;
            case 3:
#line 153 "ron/uuid-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr4;
                    case 126u:
                        goto tr4;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr4;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr4;
                } else
                    goto tr4;
                goto st0;
            tr2 :
#line 6 "ragel/./uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/./uuid-grammar.rl"
                { wordb = p; }
                goto st6;
            tr4 :
#line 13 "ragel/./uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st6;
            st6:
                if (++p == pe) goto _test_eof6;
            case 6:
#line 186 "ron/uuid-parser.cc"
                switch ((*p)) {
                    case 43u:
                        goto tr5;
                    case 45u:
                        goto tr5;
                    case 95u:
                        goto st6;
                    case 126u:
                        goto st6;
                }
                if ((*p) < 48u) {
                    if (36u <= (*p) && (*p) <= 37u) goto tr5;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st6;
                    } else if ((*p) >= 65u)
                        goto st6;
                } else
                    goto st6;
                goto st0;
        }
    _test_eof4:
        cs = 4;
        goto _test_eof;
    _test_eof2:
        cs = 2;
        goto _test_eof;
    _test_eof5:
        cs = 5;
        goto _test_eof;
    _test_eof3:
        cs = 3;
        goto _test_eof;
    _test_eof6:
        cs = 6;
        goto _test_eof;

    _test_eof : {}
        if (p == eof) {
            switch (cs) {
                case 4:
                case 6:
#line 14 "ragel/./uuid-grammar.rl"
                {
                    value = Slice{wordb, p};
                }
#line 18 "ragel/./uuid-grammar.rl"
                    {}
                    break;
                case 5:
#line 17 "ragel/./uuid-grammar.rl"
                {
                    origin = Slice{wordb, p};
                }
#line 18 "ragel/./uuid-grammar.rl"
                    {}
                    break;
#line 233 "ron/uuid-parser.cc"
            }
        }

    _out : {}
    }

#line 26 "ragel/uuid-parser.rl"

    if (cs && value.size() <= Word::MAX_BASE64_SIZE &&
        origin.size() <= Word::MAX_BASE64_SIZE) {
        words_.first = Word{ABC[variety], value};
        words_.second = Word{ABC[version], origin};
    } else {
        *this = FATAL;
    }
}

}  // namespace ron
