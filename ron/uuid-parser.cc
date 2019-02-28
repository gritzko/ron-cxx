
#line 1 "ragel/uuid-parser.rl"
#include "uuid.hpp"

namespace ron {

void report(const char* pb, const char* p, const char* event) {
    std::cerr << "at " << (p - pb) << " '" << *p << "' " << event << "\n";
}

Uuid::Uuid(slice_t data) {
#line 11 "ragel/uuid-parser.rl"

#line 16 "ron/uuid-parser.cc"
    static const int UUID_start = 1;
    static const int UUID_first_final = 4;
    static const int UUID_error = 0;

    static const int UUID_en_main = 1;

#line 12 "ragel/uuid-parser.rl"

    const char* pb = data.buf_;
    const char* pe = data.buf_ + data.size_;
    const char* p = data.buf_;
    const char* eof = pe;
    int cs = 0;

    slice_t value{}, origin{}, uuidb;
    char variety{'0'}, version{'$'};

#line 36 "ron/uuid-parser.cc"
    { cs = UUID_start; }

#line 41 "ron/uuid-parser.cc"
    {
        if (p == pe) goto _test_eof;
        switch (cs) {
            case 1:
                switch ((*p)) {
                    case 95:
                        goto tr2;
                    case 126:
                        goto tr2;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr0;
                } else if ((*p) > 70) {
                    if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto tr2;
                    } else if ((*p) >= 71)
                        goto tr2;
                } else
                    goto tr0;
                goto st0;
            st0:
                cs = 0;
                goto _out;
            tr0 :
#line 5 "ragel/./uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/./uuid-grammar.rl"
                { value.begin(p); }
                goto st4;
            st4:
                if (++p == pe) goto _test_eof4;
            case 4:
#line 82 "ron/uuid-parser.cc"
                switch ((*p)) {
                    case 43:
                        goto tr5;
                    case 45:
                        goto tr5;
                    case 47:
                        goto tr6;
                    case 95:
                        goto st6;
                    case 126:
                        goto st6;
                }
                if ((*p) < 48) {
                    if (36 <= (*p) && (*p) <= 37) goto tr5;
                } else if ((*p) > 57) {
                    if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st6;
                    } else if ((*p) >= 65)
                        goto st6;
                } else
                    goto st6;
                goto st0;
            tr5 :
#line 13 "ragel/./uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/./uuid-grammar.rl"
                { version = (*p); }
                goto st2;
            st2:
                if (++p == pe) goto _test_eof2;
            case 2:
#line 112 "ron/uuid-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr3;
                    case 126:
                        goto tr3;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr3;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto tr3;
                } else
                    goto tr3;
                goto st0;
            tr3 :
#line 15 "ragel/./uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st5;
            st5:
                if (++p == pe) goto _test_eof5;
            case 5:
#line 134 "ron/uuid-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto st5;
                    case 126:
                        goto st5;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto st5;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto st5;
                } else
                    goto st5;
                goto st0;
            tr6 :
#line 11 "ragel/./uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st3;
            st3:
                if (++p == pe) goto _test_eof3;
            case 3:
#line 156 "ron/uuid-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr4;
                    case 126:
                        goto tr4;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr4;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto tr4;
                } else
                    goto tr4;
                goto st0;
            tr2 :
#line 5 "ragel/./uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/./uuid-grammar.rl"
                { value.begin(p); }
                goto st6;
            tr4 :
#line 12 "ragel/./uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st6;
            st6:
                if (++p == pe) goto _test_eof6;
            case 6:
#line 189 "ron/uuid-parser.cc"
                switch ((*p)) {
                    case 43:
                        goto tr5;
                    case 45:
                        goto tr5;
                    case 95:
                        goto st6;
                    case 126:
                        goto st6;
                }
                if ((*p) < 48) {
                    if (36 <= (*p) && (*p) <= 37) goto tr5;
                } else if ((*p) > 57) {
                    if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st6;
                    } else if ((*p) >= 65)
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
#line 13 "ragel/./uuid-grammar.rl"
                {
                    value.end(p);
                }
#line 17 "ragel/./uuid-grammar.rl"
                    { uuidb.end(p); }
                    break;
                case 5:
#line 16 "ragel/./uuid-grammar.rl"
                {
                    origin.end(p);
                }
#line 17 "ragel/./uuid-grammar.rl"
                    { uuidb.end(p); }
                    break;
#line 236 "ron/uuid-parser.cc"
            }
        }

    _out : {}
    }

#line 29 "ragel/uuid-parser.rl"

    if (cs && value.size() <= Word::MAX_BASE64_SIZE &&
        origin.size() <= Word::MAX_BASE64_SIZE) {
        words_.first = Word{ABC[variety], value};
        words_.second = Word{ABC[version], origin};
    } else {
        *this = FATAL;
    }
}

}  // namespace ron
