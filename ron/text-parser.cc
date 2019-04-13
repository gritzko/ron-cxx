
#line 1 "ragel/text-parser.rl"
#include <iostream>
#include "text.hpp"

namespace ron {

#line 7 "ragel/text-parser.rl"

#line 12 "ron/text-parser.cc"
static const int RON_start = 72;
static const int RON_first_final = 72;
static const int RON_error = 0;

static const int RON_en_main = 72;

#line 8 "ragel/text-parser.rl"

Status TextFrame::Cursor::Next() {
    Atoms& atoms = op_.atoms_;

    int line = line_;

    switch (cs) {
        case RON_error:
            if (off_ != 0) {
                return Status::BAD_STATE;
            }

#line 36 "ron/text-parser.cc"
            { cs = RON_start; }

#line 23 "ragel/text-parser.rl"
            break;

        case RON_FULL_STOP:
            cs = RON_error;
            return Status::ENDOFFRAME;

        default:
            break;
    }

    if (data().size() <= off_) {
        cs = RON_error;
        return Status::ENDOFFRAME;
    }

    Slice body{data()};
    CharRef pb = body.buf_;
    CharRef p = pb + off_;
    CharRef pe = pb + body.size();
    CharRef eof = pe;
    CharRef lineb = pb;
    CharRef intb{p};
    CharRef floatb{p};
    CharRef strb{p};
    CharRef uuidb{p};
    CharRef wordb{p};
    char term{0};
    Slice value, origin;
    char variety, version;

    atoms.clear();
    op_.AddAtom(prev_id_.inc());
    op_.AddAtom(prev_id_);

    // std::cerr<<"starting with "<<cs<<" ["<<p<<"]\n";

#line 79 "ron/text-parser.cc"
    {
        if (p == pe) goto _test_eof;
        switch (cs) {
        tr198 :
#line 50 "ragel/./text-grammar.rl"
        {
            if ((*p) == '\n') {
                line++;
                lineb = p;
            }
        }
            goto st72;
        tr14 :
#line 45 "ragel/./text-grammar.rl"
        {
            term = (*p);
            pos_++;
            if (p < pe - 1) {
                p++;
                cs = 72;
                goto _out;
            }
        }
            goto st72;
        tr49 :
#line 16 "ragel/./text-grammar.rl"
        {
            Slice the_int(intb, p);
            if (the_int.size() >= 19 && int_too_big(the_int)) {
                cs = 0;
                {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            op_.AddAtom(
                Atom::Integer(parse_int(the_int), body.range_of(the_int)));
            uuidb = nullptr;  // sabotage uuid
        }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr59 :
#line 28 "ragel/./text-grammar.rl"
        {
            Slice the_float{floatb, p};
            if (the_float.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            op_.AddAtom(
                Atom::Float(parse_float(the_float), body.range_of(the_float)));
        }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr73 :
#line 14 "ragel/././uuid-grammar.rl"
        {
            value = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 33 "ragel/./text-grammar.rl"
            {
                if (word_too_big(value) || word_too_big(origin)) {
                    cs = 0;
                    {
                        p++;
                        cs = 72;
                        goto _out;
                    }
                }
                op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr84 :
#line 17 "ragel/././uuid-grammar.rl"
        {
            origin = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 33 "ragel/./text-grammar.rl"
            {
                if (word_too_big(value) || word_too_big(origin)) {
                    cs = 0;
                    {
                        p++;
                        cs = 72;
                        goto _out;
                    }
                }
                op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr99 :
#line 16 "ragel/./text-grammar.rl"
        {
            Slice the_int(intb, p);
            if (the_int.size() >= 19 && int_too_big(the_int)) {
                cs = 0;
                {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            op_.AddAtom(
                Atom::Integer(parse_int(the_int), body.range_of(the_int)));
            uuidb = nullptr;  // sabotage uuid
        }
#line 14 "ragel/././uuid-grammar.rl"
            { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 37 "ragel/./text-grammar.rl"
            {
                if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 72;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
            }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr112 :
#line 17 "ragel/././uuid-grammar.rl"
        {
            origin = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 37 "ragel/./text-grammar.rl"
            {
                if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 72;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
            }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr121 :
#line 14 "ragel/././uuid-grammar.rl"
        {
            value = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 37 "ragel/./text-grammar.rl"
            {
                if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 72;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
            }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr131 :
#line 28 "ragel/./text-grammar.rl"
        {
            Slice the_float{floatb, p};
            if (the_float.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            op_.AddAtom(
                Atom::Float(parse_float(the_float), body.range_of(the_float)));
        }
#line 17 "ragel/././uuid-grammar.rl"
            { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 37 "ragel/./text-grammar.rl"
            {
                if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 72;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
            }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr139 :
#line 28 "ragel/./text-grammar.rl"
        {
            Slice the_float{floatb, p};
            if (the_float.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            op_.AddAtom(
                Atom::Float(parse_float(the_float), body.range_of(the_float)));
        }
#line 14 "ragel/././uuid-grammar.rl"
            { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 37 "ragel/./text-grammar.rl"
            {
                if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 72;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
            }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr153 :
#line 14 "ragel/././uuid-grammar.rl"
        {
            value = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 9 "ragel/./text-grammar.rl"
            { op_.SetId(Uuid{variety, value, version, origin}); }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr168 :
#line 14 "ragel/././uuid-grammar.rl"
        {
            value = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 12 "ragel/./text-grammar.rl"
            { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr179 :
#line 17 "ragel/././uuid-grammar.rl"
        {
            origin = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 12 "ragel/./text-grammar.rl"
            { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        tr189 :
#line 17 "ragel/././uuid-grammar.rl"
        {
            origin = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 9 "ragel/./text-grammar.rl"
            { op_.SetId(Uuid{variety, value, version, origin}); }
#line 45 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            goto st72;
        st72:
            if (++p == pe) goto _test_eof72;
            case 72:
#line 369 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr198;
                    case 32u:
                        goto tr198;
                    case 33u:
                        goto tr14;
                    case 39u:
                        goto st1;
                    case 40u:
                        goto st5;
                    case 44u:
                        goto tr14;
                    case 46u:
                        goto st58;
                    case 59u:
                        goto tr14;
                    case 61u:
                        goto st24;
                    case 62u:
                        goto st27;
                    case 63u:
                        goto tr14;
                    case 64u:
                        goto st59;
                    case 94u:
                        goto st31;
                    case 95u:
                        goto tr23;
                    case 126u:
                        goto tr23;
                }
                if ((*p) < 48u) {
                    if ((*p) > 10u) {
                        if (43u <= (*p) && (*p) <= 45u) goto tr20;
                    } else if ((*p) >= 9u)
                        goto tr198;
                } else if ((*p) > 57u) {
                    if ((*p) < 71u) {
                        if (65u <= (*p) && (*p) <= 70u) goto tr22;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr23;
                    } else
                        goto tr23;
                } else
                    goto tr21;
                goto st0;
            st0:
                cs = 0;
                goto _out;
            tr50 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
                goto st1;
            tr60 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
                goto st1;
            tr75 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr85 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr101 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 1;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st1;
            tr113 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 1;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st1;
            tr122 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 1;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st1;
            tr132 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 1;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st1;
            tr140 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 1;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st1;
            tr155 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st1;
            tr170 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st1;
            tr180 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st1;
            tr190 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st1;
            st1:
                if (++p == pe) goto _test_eof1;
            case 1:
#line 597 "ron/text-parser.cc"
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr2;
                    case 92u:
                        goto tr3;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr4;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr6;
                } else
                    goto tr5;
                goto tr0;
            tr0 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st2;
            st2:
                if (++p == pe) goto _test_eof2;
            case 2:
#line 627 "ron/text-parser.cc"
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr8;
                    case 92u:
                        goto st50;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto st55;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto st57;
                } else
                    goto st56;
                goto st2;
            tr2 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 23 "ragel/./text-grammar.rl"
                {
                    Slice the_str{strb, p};
                    op_.AddAtom(Atom::String(body.range_of(the_str)));
                }
                goto st3;
            tr8 :
#line 23 "ragel/./text-grammar.rl"
            {
                Slice the_str{strb, p};
                op_.AddAtom(Atom::String(body.range_of(the_str)));
            }
                goto st3;
            st3:
                if (++p == pe) goto _test_eof3;
            case 3:
#line 669 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr13;
                    case 32u:
                        goto tr13;
                    case 33u:
                        goto tr14;
                    case 39u:
                        goto st1;
                    case 40u:
                        goto st5;
                    case 44u:
                        goto tr14;
                    case 59u:
                        goto tr14;
                    case 61u:
                        goto st24;
                    case 62u:
                        goto st27;
                    case 63u:
                        goto tr14;
                    case 94u:
                        goto st31;
                }
                if (9u <= (*p) && (*p) <= 10u) goto tr13;
                goto st0;
            tr13 :
#line 50 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st4;
            tr48 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr58 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr72 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 4;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr83 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 4;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr98 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 4;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr111 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 4;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr120 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 4;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr130 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 4;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr138 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 4;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr167 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr178 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            st4:
                if (++p == pe) goto _test_eof4;
            case 4:
#line 937 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr13;
                    case 32u:
                        goto tr13;
                    case 33u:
                        goto tr14;
                    case 39u:
                        goto st1;
                    case 40u:
                        goto st5;
                    case 44u:
                        goto tr14;
                    case 59u:
                        goto tr14;
                    case 61u:
                        goto st24;
                    case 62u:
                        goto st27;
                    case 63u:
                        goto tr14;
                    case 94u:
                        goto st31;
                    case 95u:
                        goto tr23;
                    case 126u:
                        goto tr23;
                }
                if ((*p) < 48u) {
                    if ((*p) > 10u) {
                        if (43u <= (*p) && (*p) <= 45u) goto tr20;
                    } else if ((*p) >= 9u)
                        goto tr13;
                } else if ((*p) > 57u) {
                    if ((*p) < 71u) {
                        if (65u <= (*p) && (*p) <= 70u) goto tr22;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr23;
                    } else
                        goto tr23;
                } else
                    goto tr21;
                goto st0;
            tr24 :
#line 50 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st5;
            tr51 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
                goto st5;
            tr61 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
                goto st5;
            tr76 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 5;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st5;
            tr86 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 5;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st5;
            tr102 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 5;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st5;
            tr114 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 5;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st5;
            tr123 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 5;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st5;
            tr133 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 5;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st5;
            tr141 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 5;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st5;
            tr156 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st5;
            tr171 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st5;
            tr181 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st5;
            tr191 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st5;
            st5:
                if (++p == pe) goto _test_eof5;
            case 5:
#line 1169 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr24;
                    case 32u:
                        goto tr24;
                    case 39u:
                        goto st6;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st9;
                } else if ((*p) >= 9u)
                    goto tr24;
                goto st0;
            st6:
                if (++p == pe) goto _test_eof6;
            case 6:
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr28;
                    case 92u:
                        goto tr29;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr30;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr32;
                } else
                    goto tr31;
                goto tr27;
            tr27 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st7;
            st7:
                if (++p == pe) goto _test_eof7;
            case 7:
#line 1214 "ron/text-parser.cc"
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr34;
                    case 92u:
                        goto st12;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto st17;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto st19;
                } else
                    goto st18;
                goto st7;
            tr28 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 23 "ragel/./text-grammar.rl"
                {
                    Slice the_str{strb, p};
                    op_.AddAtom(Atom::String(body.range_of(the_str)));
                }
                goto st8;
            tr34 :
#line 23 "ragel/./text-grammar.rl"
            {
                Slice the_str{strb, p};
                op_.AddAtom(Atom::String(body.range_of(the_str)));
            }
                goto st8;
            tr39 :
#line 50 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st8;
            st8:
                if (++p == pe) goto _test_eof8;
            case 8:
#line 1265 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr39;
                    case 32u:
                        goto tr39;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st9;
                } else if ((*p) >= 9u)
                    goto tr39;
                goto st0;
            st9:
                if (++p == pe) goto _test_eof9;
            case 9:
                switch ((*p)) {
                    case 13u:
                        goto tr40;
                    case 32u:
                        goto tr40;
                    case 41u:
                        goto st11;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st9;
                } else if ((*p) >= 9u)
                    goto tr40;
                goto st0;
            tr40 :
#line 50 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st10;
            st10:
                if (++p == pe) goto _test_eof10;
            case 10:
#line 1304 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr40;
                    case 32u:
                        goto tr40;
                    case 41u:
                        goto st11;
                }
                if (9u <= (*p) && (*p) <= 10u) goto tr40;
                goto st0;
            tr42 :
#line 50 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st11;
            st11:
                if (++p == pe) goto _test_eof11;
            case 11:
#line 1326 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr42;
                    case 32u:
                        goto tr42;
                    case 33u:
                        goto tr14;
                    case 44u:
                        goto tr14;
                    case 59u:
                        goto tr14;
                    case 63u:
                        goto tr14;
                }
                if (9u <= (*p) && (*p) <= 10u) goto tr42;
                goto st0;
            tr29 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st12;
            st12:
                if (++p == pe) goto _test_eof12;
            case 12:
#line 1346 "ron/text-parser.cc"
                switch ((*p)) {
                    case 34u:
                        goto st7;
                    case 39u:
                        goto st7;
                    case 47u:
                        goto st7;
                    case 92u:
                        goto st7;
                    case 98u:
                        goto st7;
                    case 110u:
                        goto st7;
                    case 114u:
                        goto st7;
                    case 116u:
                        goto st7;
                    case 117u:
                        goto st13;
                }
                goto st0;
            st13:
                if (++p == pe) goto _test_eof13;
            case 13:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st14;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st14;
                } else
                    goto st14;
                goto st0;
            st14:
                if (++p == pe) goto _test_eof14;
            case 14:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st15;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st15;
                } else
                    goto st15;
                goto st0;
            st15:
                if (++p == pe) goto _test_eof15;
            case 15:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st16;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st16;
                } else
                    goto st16;
                goto st0;
            st16:
                if (++p == pe) goto _test_eof16;
            case 16:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st7;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st7;
                } else
                    goto st7;
                goto st0;
            tr30 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st17;
            st17:
                if (++p == pe) goto _test_eof17;
            case 17:
#line 1419 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st7;
                goto st0;
            tr31 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st18;
            st18:
                if (++p == pe) goto _test_eof18;
            case 18:
#line 1431 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st17;
                goto st0;
            tr32 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st19;
            st19:
                if (++p == pe) goto _test_eof19;
            case 19:
#line 1443 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st18;
                goto st0;
            tr20 :
#line 15 "ragel/./text-grammar.rl"
            {
                intb = p;
            }
#line 27 "ragel/./text-grammar.rl"
                { floatb = p; }
                goto st20;
            st20:
                if (++p == pe) goto _test_eof20;
            case 20:
#line 1457 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st21;
                goto st0;
            st21:
                if (++p == pe) goto _test_eof21;
            case 21:
                switch ((*p)) {
                    case 13u:
                        goto tr48;
                    case 32u:
                        goto tr48;
                    case 33u:
                        goto tr49;
                    case 39u:
                        goto tr50;
                    case 40u:
                        goto tr51;
                    case 44u:
                        goto tr49;
                    case 46u:
                        goto st22;
                    case 59u:
                        goto tr49;
                    case 61u:
                        goto tr53;
                    case 62u:
                        goto tr54;
                    case 63u:
                        goto tr49;
                    case 69u:
                        goto st34;
                    case 94u:
                        goto tr56;
                    case 101u:
                        goto st34;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st21;
                } else if ((*p) >= 9u)
                    goto tr48;
                goto st0;
            st22:
                if (++p == pe) goto _test_eof22;
            case 22:
                if (48u <= (*p) && (*p) <= 57u) goto st23;
                goto st0;
            st23:
                if (++p == pe) goto _test_eof23;
            case 23:
                switch ((*p)) {
                    case 13u:
                        goto tr58;
                    case 32u:
                        goto tr58;
                    case 33u:
                        goto tr59;
                    case 39u:
                        goto tr60;
                    case 40u:
                        goto tr61;
                    case 44u:
                        goto tr59;
                    case 59u:
                        goto tr59;
                    case 61u:
                        goto tr62;
                    case 62u:
                        goto tr63;
                    case 63u:
                        goto tr59;
                    case 69u:
                        goto st34;
                    case 94u:
                        goto tr64;
                    case 101u:
                        goto st34;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st23;
                } else if ((*p) >= 9u)
                    goto tr58;
                goto st0;
            tr65 :
#line 50 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st24;
            tr53 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 24;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
                goto st24;
            tr62 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 24;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
                goto st24;
            tr79 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 24;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st24;
            tr88 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 24;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st24;
            tr105 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 24;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 24;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st24;
            tr116 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 24;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st24;
            tr124 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 24;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st24;
            tr135 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 24;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 24;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st24;
            tr142 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 24;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 24;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st24;
            tr160 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st24;
            tr174 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st24;
            tr183 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st24;
            tr194 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st24;
            st24:
                if (++p == pe) goto _test_eof24;
            case 24:
#line 1717 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr65;
                    case 32u:
                        goto tr65;
                    case 43u:
                        goto tr66;
                    case 45u:
                        goto tr66;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr67;
                } else if ((*p) >= 9u)
                    goto tr65;
                goto st0;
            tr66 :
#line 15 "ragel/./text-grammar.rl"
            {
                intb = p;
            }
                goto st25;
            st25:
                if (++p == pe) goto _test_eof25;
            case 25:
#line 1738 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st26;
                goto st0;
            tr67 :
#line 15 "ragel/./text-grammar.rl"
            {
                intb = p;
            }
                goto st26;
            st26:
                if (++p == pe) goto _test_eof26;
            case 26:
#line 1750 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr48;
                    case 32u:
                        goto tr48;
                    case 33u:
                        goto tr49;
                    case 39u:
                        goto tr50;
                    case 40u:
                        goto tr51;
                    case 44u:
                        goto tr49;
                    case 59u:
                        goto tr49;
                    case 61u:
                        goto tr53;
                    case 62u:
                        goto tr54;
                    case 63u:
                        goto tr49;
                    case 94u:
                        goto tr56;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st26;
                } else if ((*p) >= 9u)
                    goto tr48;
                goto st0;
            tr69 :
#line 50 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st27;
            tr54 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 27;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
                goto st27;
            tr63 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 27;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
                goto st27;
            tr80 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 27;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st27;
            tr89 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 27;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st27;
            tr106 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 27;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 27;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st27;
            tr117 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 27;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st27;
            tr125 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 27;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st27;
            tr136 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 27;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 27;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st27;
            tr143 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 27;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 27;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st27;
            tr161 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st27;
            tr175 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st27;
            tr184 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st27;
            tr195 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st27;
            st27:
                if (++p == pe) goto _test_eof27;
            case 27:
#line 1968 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr69;
                    case 32u:
                        goto tr69;
                    case 95u:
                        goto tr71;
                    case 126u:
                        goto tr71;
                }
                if ((*p) < 65u) {
                    if ((*p) > 10u) {
                        if (48u <= (*p) && (*p) <= 57u) goto tr70;
                    } else if ((*p) >= 9u)
                        goto tr69;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr71;
                    } else if ((*p) >= 71u)
                        goto tr71;
                } else
                    goto tr70;
                goto st0;
            tr70 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st28;
            st28:
                if (++p == pe) goto _test_eof28;
            case 28:
#line 2005 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr72;
                    case 32u:
                        goto tr72;
                    case 33u:
                        goto tr73;
                    case 39u:
                        goto tr75;
                    case 40u:
                        goto tr76;
                    case 44u:
                        goto tr73;
                    case 47u:
                        goto tr77;
                    case 59u:
                        goto tr73;
                    case 61u:
                        goto tr79;
                    case 62u:
                        goto tr80;
                    case 63u:
                        goto tr73;
                    case 94u:
                        goto tr81;
                    case 95u:
                        goto st38;
                    case 126u:
                        goto st38;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr74;
                    } else if ((*p) >= 9u)
                        goto tr72;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st38;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st38;
                    } else
                        goto st38;
                } else
                    goto tr74;
                goto st0;
            tr74 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 15 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st29;
            st29:
                if (++p == pe) goto _test_eof29;
            case 29:
#line 2050 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr82;
                    case 126u:
                        goto tr82;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr82;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr82;
                } else
                    goto tr82;
                goto st0;
            tr82 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st30;
            st30:
                if (++p == pe) goto _test_eof30;
            case 30:
#line 2072 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr83;
                    case 32u:
                        goto tr83;
                    case 33u:
                        goto tr84;
                    case 39u:
                        goto tr85;
                    case 40u:
                        goto tr86;
                    case 44u:
                        goto tr84;
                    case 59u:
                        goto tr84;
                    case 61u:
                        goto tr88;
                    case 62u:
                        goto tr89;
                    case 63u:
                        goto tr84;
                    case 94u:
                        goto tr90;
                    case 95u:
                        goto st30;
                    case 126u:
                        goto st30;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr83;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st30;
                    } else if ((*p) >= 65u)
                        goto st30;
                } else
                    goto st30;
                goto st0;
            tr91 :
#line 50 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st31;
            tr56 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 31;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
                goto st31;
            tr64 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 31;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
                goto st31;
            tr81 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 31;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st31;
            tr90 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 33 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 31;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st31;
            tr109 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 31;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(the_int), body.range_of(the_int)));
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 31;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st31;
            tr118 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 31;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st31;
            tr126 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 31;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st31;
            tr137 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 31;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 31;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st31;
            tr144 :
#line 28 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 31;
                        goto _out;
                    }
                }
                op_.AddAtom(Atom::Float(parse_float(the_float),
                                        body.range_of(the_float)));
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 31;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st31;
            tr162 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st31;
            tr176 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st31;
            tr185 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st31;
            tr196 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st31;
            st31:
                if (++p == pe) goto _test_eof31;
            case 31:
#line 2298 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr91;
                    case 32u:
                        goto tr91;
                    case 43u:
                        goto tr92;
                    case 45u:
                        goto tr92;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr93;
                } else if ((*p) >= 9u)
                    goto tr91;
                goto st0;
            tr92 :
#line 27 "ragel/./text-grammar.rl"
            {
                floatb = p;
            }
                goto st32;
            st32:
                if (++p == pe) goto _test_eof32;
            case 32:
#line 2319 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st33;
                goto st0;
            tr93 :
#line 27 "ragel/./text-grammar.rl"
            {
                floatb = p;
            }
                goto st33;
            st33:
                if (++p == pe) goto _test_eof33;
            case 33:
#line 2331 "ron/text-parser.cc"
                switch ((*p)) {
                    case 46u:
                        goto st22;
                    case 69u:
                        goto st34;
                    case 101u:
                        goto st34;
                }
                if (48u <= (*p) && (*p) <= 57u) goto st33;
                goto st0;
            st34:
                if (++p == pe) goto _test_eof34;
            case 34:
                switch ((*p)) {
                    case 43u:
                        goto st35;
                    case 45u:
                        goto st35;
                }
                if (48u <= (*p) && (*p) <= 57u) goto st36;
                goto st0;
            st35:
                if (++p == pe) goto _test_eof35;
            case 35:
                if (48u <= (*p) && (*p) <= 57u) goto st36;
                goto st0;
            st36:
                if (++p == pe) goto _test_eof36;
            case 36:
                switch ((*p)) {
                    case 13u:
                        goto tr58;
                    case 32u:
                        goto tr58;
                    case 33u:
                        goto tr59;
                    case 39u:
                        goto tr60;
                    case 40u:
                        goto tr61;
                    case 44u:
                        goto tr59;
                    case 59u:
                        goto tr59;
                    case 61u:
                        goto tr62;
                    case 62u:
                        goto tr63;
                    case 63u:
                        goto tr59;
                    case 94u:
                        goto tr64;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st36;
                } else if ((*p) >= 9u)
                    goto tr58;
                goto st0;
            tr77 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st37;
            st37:
                if (++p == pe) goto _test_eof37;
            case 37:
#line 2389 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr97;
                    case 126u:
                        goto tr97;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr97;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr97;
                } else
                    goto tr97;
                goto st0;
            tr71 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st38;
            tr97 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st38;
            st38:
                if (++p == pe) goto _test_eof38;
            case 38:
#line 2422 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr72;
                    case 32u:
                        goto tr72;
                    case 33u:
                        goto tr73;
                    case 39u:
                        goto tr75;
                    case 40u:
                        goto tr76;
                    case 44u:
                        goto tr73;
                    case 59u:
                        goto tr73;
                    case 61u:
                        goto tr79;
                    case 62u:
                        goto tr80;
                    case 63u:
                        goto tr73;
                    case 94u:
                        goto tr81;
                    case 95u:
                        goto st38;
                    case 126u:
                        goto st38;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr74;
                    } else if ((*p) >= 9u)
                        goto tr72;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st38;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st38;
                    } else
                        goto st38;
                } else
                    goto tr74;
                goto st0;
            tr21 :
#line 15 "ragel/./text-grammar.rl"
            {
                intb = p;
            }
#line 27 "ragel/./text-grammar.rl"
                { floatb = p; }
#line 6 "ragel/././uuid-grammar.rl"
                {
                    variety = '0';
                    version = '$';
                    origin = Slice{};
                    uuidb = p;
                }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st39;
            st39:
                if (++p == pe) goto _test_eof39;
            case 39:
#line 2475 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr98;
                    case 32u:
                        goto tr98;
                    case 33u:
                        goto tr99;
                    case 39u:
                        goto tr101;
                    case 40u:
                        goto tr102;
                    case 44u:
                        goto tr99;
                    case 46u:
                        goto st22;
                    case 47u:
                        goto tr103;
                    case 59u:
                        goto tr99;
                    case 61u:
                        goto tr105;
                    case 62u:
                        goto tr106;
                    case 63u:
                        goto tr99;
                    case 69u:
                        goto st45;
                    case 94u:
                        goto tr109;
                    case 95u:
                        goto st43;
                    case 101u:
                        goto st45;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr100;
                    } else if ((*p) >= 9u)
                        goto tr98;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st44;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr100;
                goto st0;
            tr100 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 15 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st40;
            st40:
                if (++p == pe) goto _test_eof40;
            case 40:
#line 2523 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr110;
                    case 126u:
                        goto tr110;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr110;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr110;
                } else
                    goto tr110;
                goto st0;
            tr110 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st41;
            st41:
                if (++p == pe) goto _test_eof41;
            case 41:
#line 2545 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr111;
                    case 32u:
                        goto tr111;
                    case 33u:
                        goto tr112;
                    case 39u:
                        goto tr113;
                    case 40u:
                        goto tr114;
                    case 44u:
                        goto tr112;
                    case 59u:
                        goto tr112;
                    case 61u:
                        goto tr116;
                    case 62u:
                        goto tr117;
                    case 63u:
                        goto tr112;
                    case 94u:
                        goto tr118;
                    case 95u:
                        goto st41;
                    case 126u:
                        goto st41;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr111;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st41;
                    } else if ((*p) >= 65u)
                        goto st41;
                } else
                    goto st41;
                goto st0;
            tr103 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st42;
            st42:
                if (++p == pe) goto _test_eof42;
            case 42:
#line 2581 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr119;
                    case 126u:
                        goto tr119;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr119;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr119;
                } else
                    goto tr119;
                goto st0;
            tr23 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st43;
            tr119 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st43;
            st43:
                if (++p == pe) goto _test_eof43;
            case 43:
#line 2614 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr120;
                    case 32u:
                        goto tr120;
                    case 33u:
                        goto tr121;
                    case 39u:
                        goto tr122;
                    case 40u:
                        goto tr123;
                    case 44u:
                        goto tr121;
                    case 59u:
                        goto tr121;
                    case 61u:
                        goto tr124;
                    case 62u:
                        goto tr125;
                    case 63u:
                        goto tr121;
                    case 94u:
                        goto tr126;
                    case 95u:
                        goto st43;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr100;
                    } else if ((*p) >= 9u)
                        goto tr120;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st43;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr100;
                goto st0;
            st44:
                if (++p == pe) goto _test_eof44;
            case 44:
                switch ((*p)) {
                    case 13u:
                        goto tr98;
                    case 32u:
                        goto tr98;
                    case 33u:
                        goto tr99;
                    case 39u:
                        goto tr101;
                    case 40u:
                        goto tr102;
                    case 44u:
                        goto tr99;
                    case 46u:
                        goto st22;
                    case 59u:
                        goto tr99;
                    case 61u:
                        goto tr105;
                    case 62u:
                        goto tr106;
                    case 63u:
                        goto tr99;
                    case 69u:
                        goto st45;
                    case 94u:
                        goto tr109;
                    case 95u:
                        goto st43;
                    case 101u:
                        goto st45;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr100;
                    } else if ((*p) >= 9u)
                        goto tr98;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st44;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr100;
                goto st0;
            st45:
                if (++p == pe) goto _test_eof45;
            case 45:
                switch ((*p)) {
                    case 13u:
                        goto tr120;
                    case 32u:
                        goto tr120;
                    case 33u:
                        goto tr121;
                    case 39u:
                        goto tr122;
                    case 40u:
                        goto tr123;
                    case 44u:
                        goto tr121;
                    case 59u:
                        goto tr121;
                    case 61u:
                        goto tr124;
                    case 62u:
                        goto tr125;
                    case 63u:
                        goto tr121;
                    case 94u:
                        goto tr126;
                    case 95u:
                        goto st43;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr100;
                    } else if ((*p) >= 9u)
                        goto tr120;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st48;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr127;
                goto st0;
            tr127 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 15 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st46;
            st46:
                if (++p == pe) goto _test_eof46;
            case 46:
#line 2735 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr110;
                    case 126u:
                        goto tr110;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr129;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr110;
                } else
                    goto tr110;
                goto st0;
            tr129 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st47;
            st47:
                if (++p == pe) goto _test_eof47;
            case 47:
#line 2757 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr130;
                    case 32u:
                        goto tr130;
                    case 33u:
                        goto tr131;
                    case 39u:
                        goto tr132;
                    case 40u:
                        goto tr133;
                    case 44u:
                        goto tr131;
                    case 59u:
                        goto tr131;
                    case 61u:
                        goto tr135;
                    case 62u:
                        goto tr136;
                    case 63u:
                        goto tr131;
                    case 94u:
                        goto tr137;
                    case 95u:
                        goto st41;
                    case 126u:
                        goto st41;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr130;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st41;
                    } else if ((*p) >= 65u)
                        goto st41;
                } else
                    goto st47;
                goto st0;
            st48:
                if (++p == pe) goto _test_eof48;
            case 48:
                switch ((*p)) {
                    case 13u:
                        goto tr138;
                    case 32u:
                        goto tr138;
                    case 33u:
                        goto tr139;
                    case 39u:
                        goto tr140;
                    case 40u:
                        goto tr141;
                    case 44u:
                        goto tr139;
                    case 59u:
                        goto tr139;
                    case 61u:
                        goto tr142;
                    case 62u:
                        goto tr143;
                    case 63u:
                        goto tr139;
                    case 94u:
                        goto tr144;
                    case 95u:
                        goto st43;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr100;
                    } else if ((*p) >= 9u)
                        goto tr138;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st48;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr100;
                goto st0;
            tr22 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st49;
            st49:
                if (++p == pe) goto _test_eof49;
            case 49:
#line 2837 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr120;
                    case 32u:
                        goto tr120;
                    case 33u:
                        goto tr121;
                    case 39u:
                        goto tr122;
                    case 40u:
                        goto tr123;
                    case 44u:
                        goto tr121;
                    case 47u:
                        goto tr103;
                    case 59u:
                        goto tr121;
                    case 61u:
                        goto tr124;
                    case 62u:
                        goto tr125;
                    case 63u:
                        goto tr121;
                    case 94u:
                        goto tr126;
                    case 95u:
                        goto st43;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr100;
                    } else if ((*p) >= 9u)
                        goto tr120;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st43;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr100;
                goto st0;
            tr3 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st50;
            st50:
                if (++p == pe) goto _test_eof50;
            case 50:
#line 2880 "ron/text-parser.cc"
                switch ((*p)) {
                    case 34u:
                        goto st2;
                    case 39u:
                        goto st2;
                    case 47u:
                        goto st2;
                    case 92u:
                        goto st2;
                    case 98u:
                        goto st2;
                    case 110u:
                        goto st2;
                    case 114u:
                        goto st2;
                    case 116u:
                        goto st2;
                    case 117u:
                        goto st51;
                }
                goto st0;
            st51:
                if (++p == pe) goto _test_eof51;
            case 51:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st52;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st52;
                } else
                    goto st52;
                goto st0;
            st52:
                if (++p == pe) goto _test_eof52;
            case 52:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st53;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st53;
                } else
                    goto st53;
                goto st0;
            st53:
                if (++p == pe) goto _test_eof53;
            case 53:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st54;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st54;
                } else
                    goto st54;
                goto st0;
            st54:
                if (++p == pe) goto _test_eof54;
            case 54:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st2;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st2;
                } else
                    goto st2;
                goto st0;
            tr4 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st55;
            st55:
                if (++p == pe) goto _test_eof55;
            case 55:
#line 2953 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st2;
                goto st0;
            tr5 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st56;
            st56:
                if (++p == pe) goto _test_eof56;
            case 56:
#line 2965 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st55;
                goto st0;
            tr6 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st57;
            st57:
                if (++p == pe) goto _test_eof57;
            case 57:
#line 2977 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st56;
                goto st0;
            st58:
                if (++p == pe) goto _test_eof58;
            case 58:
                if ((*p) == 10u) goto st73;
                goto st0;
            st73:
                if (++p == pe) goto _test_eof73;
            case 73:
                goto st0;
            st59:
                if (++p == pe) goto _test_eof59;
            case 59:
                switch ((*p)) {
                    case 95u:
                        goto tr151;
                    case 126u:
                        goto tr151;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr150;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr151;
                    } else if ((*p) >= 71u)
                        goto tr151;
                } else
                    goto tr150;
                goto st0;
            tr150 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st60;
            st60:
                if (++p == pe) goto _test_eof60;
            case 60:
#line 3028 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr152;
                    case 32u:
                        goto tr152;
                    case 33u:
                        goto tr153;
                    case 39u:
                        goto tr155;
                    case 40u:
                        goto tr156;
                    case 44u:
                        goto tr153;
                    case 47u:
                        goto tr157;
                    case 58u:
                        goto tr159;
                    case 59u:
                        goto tr153;
                    case 61u:
                        goto tr160;
                    case 62u:
                        goto tr161;
                    case 63u:
                        goto tr153;
                    case 94u:
                        goto tr162;
                    case 95u:
                        goto st71;
                    case 126u:
                        goto st71;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr154;
                    } else if ((*p) >= 9u)
                        goto tr152;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st71;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st71;
                    } else
                        goto st71;
                } else
                    goto tr154;
                goto st0;
            tr163 :
#line 50 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st61;
            tr152 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st61;
            tr188 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
#line 50 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st61;
            st61:
                if (++p == pe) goto _test_eof61;
            case 61:
#line 3115 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr163;
                    case 32u:
                        goto tr163;
                    case 33u:
                        goto tr14;
                    case 39u:
                        goto st1;
                    case 40u:
                        goto st5;
                    case 44u:
                        goto tr14;
                    case 58u:
                        goto st62;
                    case 59u:
                        goto tr14;
                    case 61u:
                        goto st24;
                    case 62u:
                        goto st27;
                    case 63u:
                        goto tr14;
                    case 94u:
                        goto st31;
                    case 95u:
                        goto tr23;
                    case 126u:
                        goto tr23;
                }
                if ((*p) < 48u) {
                    if ((*p) > 10u) {
                        if (43u <= (*p) && (*p) <= 45u) goto tr20;
                    } else if ((*p) >= 9u)
                        goto tr163;
                } else if ((*p) > 57u) {
                    if ((*p) < 71u) {
                        if (65u <= (*p) && (*p) <= 70u) goto tr22;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr23;
                    } else
                        goto tr23;
                } else
                    goto tr21;
                goto st0;
            tr159 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st62;
            tr193 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st62;
            st62:
                if (++p == pe) goto _test_eof62;
            case 62:
#line 3178 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr166;
                    case 126u:
                        goto tr166;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr165;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr166;
                    } else if ((*p) >= 71u)
                        goto tr166;
                } else
                    goto tr165;
                goto st0;
            tr165 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st63;
            st63:
                if (++p == pe) goto _test_eof63;
            case 63:
#line 3210 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr167;
                    case 32u:
                        goto tr167;
                    case 33u:
                        goto tr168;
                    case 39u:
                        goto tr170;
                    case 40u:
                        goto tr171;
                    case 44u:
                        goto tr168;
                    case 47u:
                        goto tr172;
                    case 59u:
                        goto tr168;
                    case 61u:
                        goto tr174;
                    case 62u:
                        goto tr175;
                    case 63u:
                        goto tr168;
                    case 94u:
                        goto tr176;
                    case 95u:
                        goto st67;
                    case 126u:
                        goto st67;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr169;
                    } else if ((*p) >= 9u)
                        goto tr167;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st67;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st67;
                    } else
                        goto st67;
                } else
                    goto tr169;
                goto st0;
            tr169 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 15 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st64;
            st64:
                if (++p == pe) goto _test_eof64;
            case 64:
#line 3255 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr177;
                    case 126u:
                        goto tr177;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr177;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr177;
                } else
                    goto tr177;
                goto st0;
            tr177 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st65;
            st65:
                if (++p == pe) goto _test_eof65;
            case 65:
#line 3277 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr178;
                    case 32u:
                        goto tr178;
                    case 33u:
                        goto tr179;
                    case 39u:
                        goto tr180;
                    case 40u:
                        goto tr181;
                    case 44u:
                        goto tr179;
                    case 59u:
                        goto tr179;
                    case 61u:
                        goto tr183;
                    case 62u:
                        goto tr184;
                    case 63u:
                        goto tr179;
                    case 94u:
                        goto tr185;
                    case 95u:
                        goto st65;
                    case 126u:
                        goto st65;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr178;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st65;
                    } else if ((*p) >= 65u)
                        goto st65;
                } else
                    goto st65;
                goto st0;
            tr172 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st66;
            st66:
                if (++p == pe) goto _test_eof66;
            case 66:
#line 3313 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr186;
                    case 126u:
                        goto tr186;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr186;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr186;
                } else
                    goto tr186;
                goto st0;
            tr166 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st67;
            tr186 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st67;
            st67:
                if (++p == pe) goto _test_eof67;
            case 67:
#line 3346 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr167;
                    case 32u:
                        goto tr167;
                    case 33u:
                        goto tr168;
                    case 39u:
                        goto tr170;
                    case 40u:
                        goto tr171;
                    case 44u:
                        goto tr168;
                    case 59u:
                        goto tr168;
                    case 61u:
                        goto tr174;
                    case 62u:
                        goto tr175;
                    case 63u:
                        goto tr168;
                    case 94u:
                        goto tr176;
                    case 95u:
                        goto st67;
                    case 126u:
                        goto st67;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr169;
                    } else if ((*p) >= 9u)
                        goto tr167;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st67;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st67;
                    } else
                        goto st67;
                } else
                    goto tr169;
                goto st0;
            tr154 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 15 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st68;
            st68:
                if (++p == pe) goto _test_eof68;
            case 68:
#line 3390 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr187;
                    case 126u:
                        goto tr187;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr187;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr187;
                } else
                    goto tr187;
                goto st0;
            tr187 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st69;
            st69:
                if (++p == pe) goto _test_eof69;
            case 69:
#line 3412 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr188;
                    case 32u:
                        goto tr188;
                    case 33u:
                        goto tr189;
                    case 39u:
                        goto tr190;
                    case 40u:
                        goto tr191;
                    case 44u:
                        goto tr189;
                    case 58u:
                        goto tr193;
                    case 59u:
                        goto tr189;
                    case 61u:
                        goto tr194;
                    case 62u:
                        goto tr195;
                    case 63u:
                        goto tr189;
                    case 94u:
                        goto tr196;
                    case 95u:
                        goto st69;
                    case 126u:
                        goto st69;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr188;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st69;
                    } else if ((*p) >= 65u)
                        goto st69;
                } else
                    goto st69;
                goto st0;
            tr157 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st70;
            st70:
                if (++p == pe) goto _test_eof70;
            case 70:
#line 3449 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr197;
                    case 126u:
                        goto tr197;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr197;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr197;
                } else
                    goto tr197;
                goto st0;
            tr151 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st71;
            tr197 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st71;
            st71:
                if (++p == pe) goto _test_eof71;
            case 71:
#line 3482 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr152;
                    case 32u:
                        goto tr152;
                    case 33u:
                        goto tr153;
                    case 39u:
                        goto tr155;
                    case 40u:
                        goto tr156;
                    case 44u:
                        goto tr153;
                    case 58u:
                        goto tr159;
                    case 59u:
                        goto tr153;
                    case 61u:
                        goto tr160;
                    case 62u:
                        goto tr161;
                    case 63u:
                        goto tr153;
                    case 94u:
                        goto tr162;
                    case 95u:
                        goto st71;
                    case 126u:
                        goto st71;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr154;
                    } else if ((*p) >= 9u)
                        goto tr152;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st71;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st71;
                    } else
                        goto st71;
                } else
                    goto tr154;
                goto st0;
        }
    _test_eof72:
        cs = 72;
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
    _test_eof6:
        cs = 6;
        goto _test_eof;
    _test_eof7:
        cs = 7;
        goto _test_eof;
    _test_eof8:
        cs = 8;
        goto _test_eof;
    _test_eof9:
        cs = 9;
        goto _test_eof;
    _test_eof10:
        cs = 10;
        goto _test_eof;
    _test_eof11:
        cs = 11;
        goto _test_eof;
    _test_eof12:
        cs = 12;
        goto _test_eof;
    _test_eof13:
        cs = 13;
        goto _test_eof;
    _test_eof14:
        cs = 14;
        goto _test_eof;
    _test_eof15:
        cs = 15;
        goto _test_eof;
    _test_eof16:
        cs = 16;
        goto _test_eof;
    _test_eof17:
        cs = 17;
        goto _test_eof;
    _test_eof18:
        cs = 18;
        goto _test_eof;
    _test_eof19:
        cs = 19;
        goto _test_eof;
    _test_eof20:
        cs = 20;
        goto _test_eof;
    _test_eof21:
        cs = 21;
        goto _test_eof;
    _test_eof22:
        cs = 22;
        goto _test_eof;
    _test_eof23:
        cs = 23;
        goto _test_eof;
    _test_eof24:
        cs = 24;
        goto _test_eof;
    _test_eof25:
        cs = 25;
        goto _test_eof;
    _test_eof26:
        cs = 26;
        goto _test_eof;
    _test_eof27:
        cs = 27;
        goto _test_eof;
    _test_eof28:
        cs = 28;
        goto _test_eof;
    _test_eof29:
        cs = 29;
        goto _test_eof;
    _test_eof30:
        cs = 30;
        goto _test_eof;
    _test_eof31:
        cs = 31;
        goto _test_eof;
    _test_eof32:
        cs = 32;
        goto _test_eof;
    _test_eof33:
        cs = 33;
        goto _test_eof;
    _test_eof34:
        cs = 34;
        goto _test_eof;
    _test_eof35:
        cs = 35;
        goto _test_eof;
    _test_eof36:
        cs = 36;
        goto _test_eof;
    _test_eof37:
        cs = 37;
        goto _test_eof;
    _test_eof38:
        cs = 38;
        goto _test_eof;
    _test_eof39:
        cs = 39;
        goto _test_eof;
    _test_eof40:
        cs = 40;
        goto _test_eof;
    _test_eof41:
        cs = 41;
        goto _test_eof;
    _test_eof42:
        cs = 42;
        goto _test_eof;
    _test_eof43:
        cs = 43;
        goto _test_eof;
    _test_eof44:
        cs = 44;
        goto _test_eof;
    _test_eof45:
        cs = 45;
        goto _test_eof;
    _test_eof46:
        cs = 46;
        goto _test_eof;
    _test_eof47:
        cs = 47;
        goto _test_eof;
    _test_eof48:
        cs = 48;
        goto _test_eof;
    _test_eof49:
        cs = 49;
        goto _test_eof;
    _test_eof50:
        cs = 50;
        goto _test_eof;
    _test_eof51:
        cs = 51;
        goto _test_eof;
    _test_eof52:
        cs = 52;
        goto _test_eof;
    _test_eof53:
        cs = 53;
        goto _test_eof;
    _test_eof54:
        cs = 54;
        goto _test_eof;
    _test_eof55:
        cs = 55;
        goto _test_eof;
    _test_eof56:
        cs = 56;
        goto _test_eof;
    _test_eof57:
        cs = 57;
        goto _test_eof;
    _test_eof58:
        cs = 58;
        goto _test_eof;
    _test_eof73:
        cs = 73;
        goto _test_eof;
    _test_eof59:
        cs = 59;
        goto _test_eof;
    _test_eof60:
        cs = 60;
        goto _test_eof;
    _test_eof61:
        cs = 61;
        goto _test_eof;
    _test_eof62:
        cs = 62;
        goto _test_eof;
    _test_eof63:
        cs = 63;
        goto _test_eof;
    _test_eof64:
        cs = 64;
        goto _test_eof;
    _test_eof65:
        cs = 65;
        goto _test_eof;
    _test_eof66:
        cs = 66;
        goto _test_eof;
    _test_eof67:
        cs = 67;
        goto _test_eof;
    _test_eof68:
        cs = 68;
        goto _test_eof;
    _test_eof69:
        cs = 69;
        goto _test_eof;
    _test_eof70:
        cs = 70;
        goto _test_eof;
    _test_eof71:
        cs = 71;
        goto _test_eof;

    _test_eof : {}
    _out : {}
    }

#line 63 "ragel/text-parser.rl"

    at_ = off_;
    off_ = p - pb;
    line_ = line;

    if (op_.size()) prev_id_ = op_.id();

    // std::cerr << "ending with [" <<p<<"] state "<<cs<<" "<<op_.size()<<"
    // atoms "<<(pe-p)<<" bytes left, prev_id_ "<<prev_id_.str()<<'\n';

    if (term && cs != RON_error) {
        op_.term_ = chr2term(term);  // FIXME gen a fn
        return Status::OK;
    } else if (cs > RON_first_final) {
        cs = RON_error;
        return Status::ENDOFFRAME;
    } else {
        cs = RON_error;
        char msg[64];
        size_t msglen =
            sprintf((char*)msg, "syntax error at line %d col %d (offset %d)",
                    line, (int)(p - lineb), (int)(p - pb));
        return Status::BAD_STATE.comment(msg);
    }
}

}  // namespace ron
