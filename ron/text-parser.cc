
#line 1 "ragel/text-parser.rl"
#include <iostream>
#include "text.hpp"

namespace ron {

#line 7 "ragel/text-parser.rl"

#line 12 "ron/text-parser.cc"
static const int RON_start = 74;
static const int RON_first_final = 74;
static const int RON_error = 0;

static const int RON_en_main = 74;

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
    Codepoint cp{0};
    char term{0};
    Slice value, origin;
    char variety{0}, version{0};

    atoms.clear();
    op_.AddAtom(prev_id_.inc());
    op_.AddAtom(prev_id_);

    // std::cerr<<"starting with "<<cs<<" ["<<p<<"]\n";

#line 80 "ron/text-parser.cc"
    {
        if (p == pe) goto _test_eof;
        switch (cs) {
        tr220 :
#line 54 "ragel/./text-grammar.rl"
        {
            if ((*p) == '\n') {
                line++;
                lineb = p;
            }
        }
            goto st74;
        tr14 :
#line 49 "ragel/./text-grammar.rl"
        {
            term = (*p);
            pos_++;
            if (p < pe - 1) {
                p++;
                cs = 74;
                goto _out;
            }
        }
            goto st74;
        tr60 :
#line 16 "ragel/./text-grammar.rl"
        {
            Slice the_int(intb, p);
            if (the_int.size() >= 19 && int_too_big(the_int)) {
                cs = 0;
                {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            // op_.AddAtom(Atom::Integer(parse_int(the_int),
            // body.range_of(the_int)));
            // TODO atoms.emplace();
            op_.AddAtom(Atom{INT, body.range_of(the_int)});
            uuidb = nullptr;  // sabotage uuid
        }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr70 :
#line 31 "ragel/./text-grammar.rl"
        {
            Slice the_float{floatb, p};
            if (the_float.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            // op_.AddAtom(Atom::Float(parse_float(the_float),
            // body.range_of(the_float)));
            op_.AddAtom(Atom{STRING, body.range_of(the_float)});
        }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr84 :
#line 14 "ragel/././uuid-grammar.rl"
        {
            value = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 37 "ragel/./text-grammar.rl"
            {
                if (word_too_big(value) || word_too_big(origin)) {
                    cs = 0;
                    {
                        p++;
                        cs = 74;
                        goto _out;
                    }
                }
                op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr95 :
#line 17 "ragel/././uuid-grammar.rl"
        {
            origin = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 37 "ragel/./text-grammar.rl"
            {
                if (word_too_big(value) || word_too_big(origin)) {
                    cs = 0;
                    {
                        p++;
                        cs = 74;
                        goto _out;
                    }
                }
                op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr110 :
#line 16 "ragel/./text-grammar.rl"
        {
            Slice the_int(intb, p);
            if (the_int.size() >= 19 && int_too_big(the_int)) {
                cs = 0;
                {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            // op_.AddAtom(Atom::Integer(parse_int(the_int),
            // body.range_of(the_int)));
            // TODO atoms.emplace();
            op_.AddAtom(Atom{INT, body.range_of(the_int)});
            uuidb = nullptr;  // sabotage uuid
        }
#line 14 "ragel/././uuid-grammar.rl"
            { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 41 "ragel/./text-grammar.rl"
            {
                if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 74;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
            }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr123 :
#line 17 "ragel/././uuid-grammar.rl"
        {
            origin = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 41 "ragel/./text-grammar.rl"
            {
                if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 74;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
            }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr132 :
#line 14 "ragel/././uuid-grammar.rl"
        {
            value = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 41 "ragel/./text-grammar.rl"
            {
                if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 74;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
            }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr142 :
#line 31 "ragel/./text-grammar.rl"
        {
            Slice the_float{floatb, p};
            if (the_float.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            // op_.AddAtom(Atom::Float(parse_float(the_float),
            // body.range_of(the_float)));
            op_.AddAtom(Atom{STRING, body.range_of(the_float)});
        }
#line 17 "ragel/././uuid-grammar.rl"
            { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 41 "ragel/./text-grammar.rl"
            {
                if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 74;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
            }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr150 :
#line 31 "ragel/./text-grammar.rl"
        {
            Slice the_float{floatb, p};
            if (the_float.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            // op_.AddAtom(Atom::Float(parse_float(the_float),
            // body.range_of(the_float)));
            op_.AddAtom(Atom{STRING, body.range_of(the_float)});
        }
#line 14 "ragel/././uuid-grammar.rl"
            { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 41 "ragel/./text-grammar.rl"
            {
                if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 74;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
            }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr175 :
#line 14 "ragel/././uuid-grammar.rl"
        {
            value = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 9 "ragel/./text-grammar.rl"
            { op_.SetId(Uuid{variety, value, version, origin}); }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr190 :
#line 14 "ragel/././uuid-grammar.rl"
        {
            value = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 12 "ragel/./text-grammar.rl"
            { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr201 :
#line 17 "ragel/././uuid-grammar.rl"
        {
            origin = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 12 "ragel/./text-grammar.rl"
            { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr211 :
#line 17 "ragel/././uuid-grammar.rl"
        {
            origin = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 9 "ragel/./text-grammar.rl"
            { op_.SetId(Uuid{variety, value, version, origin}); }
#line 49 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        st74:
            if (++p == pe) goto _test_eof74;
            case 74:
#line 377 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr220;
                    case 32u:
                        goto tr220;
                    case 33u:
                        goto tr14;
                    case 39u:
                        goto st1;
                    case 40u:
                        goto st5;
                    case 44u:
                        goto tr14;
                    case 46u:
                        goto st60;
                    case 59u:
                        goto tr14;
                    case 61u:
                        goto st25;
                    case 62u:
                        goto st28;
                    case 63u:
                        goto tr14;
                    case 64u:
                        goto st61;
                    case 94u:
                        goto st32;
                    case 95u:
                        goto tr23;
                    case 126u:
                        goto tr23;
                }
                if ((*p) < 48u) {
                    if ((*p) > 10u) {
                        if (43u <= (*p) && (*p) <= 45u) goto tr20;
                    } else if ((*p) >= 9u)
                        goto tr220;
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
            tr61 :
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
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
                goto st1;
            tr71 :
#line 31 "ragel/./text-grammar.rl"
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
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
                goto st1;
            tr86 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
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
            tr96 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
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
            tr112 :
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
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
            tr124 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
            tr133 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
            tr143 :
#line 31 "ragel/./text-grammar.rl"
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
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
            tr151 :
#line 31 "ragel/./text-grammar.rl"
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
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
            tr177 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st1;
            tr192 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st1;
            tr202 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st1;
            tr212 :
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
#line 612 "ron/text-parser.cc"
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
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st2;
            tr7 :
#line 8 "ragel/././utf8-grammar.rl"
            {
                cp = (*p);
            }
                goto st2;
            tr156 :
#line 60 "ragel/./text-grammar.rl"
            {
                cp = decode_esc((*p));
            }
                goto st2;
            tr162 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st2;
            tr168 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st2;
            st2:
                if (++p == pe) goto _test_eof2;
            case 2:
#line 662 "ron/text-parser.cc"
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr8;
                    case 92u:
                        goto st51;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr10;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr12;
                } else
                    goto tr11;
                goto tr7;
            tr2 :
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 25 "ragel/./text-grammar.rl"
                {
                    Slice the_str{strb, p};
                    // op_.AddAtom(Atom::String(body.range_of(the_str)));
                    op_.AddAtom(Atom{STRING, body.range_of(the_str)});
                }
                goto st3;
            tr8 :
#line 25 "ragel/./text-grammar.rl"
            {
                Slice the_str{strb, p};
                // op_.AddAtom(Atom::String(body.range_of(the_str)));
                op_.AddAtom(Atom{STRING, body.range_of(the_str)});
            }
                goto st3;
            tr163 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 25 "ragel/./text-grammar.rl"
                {
                    Slice the_str{strb, p};
                    // op_.AddAtom(Atom::String(body.range_of(the_str)));
                    op_.AddAtom(Atom{STRING, body.range_of(the_str)});
                }
                goto st3;
            st3:
                if (++p == pe) goto _test_eof3;
            case 3:
#line 716 "ron/text-parser.cc"
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
                        goto st25;
                    case 62u:
                        goto st28;
                    case 63u:
                        goto tr14;
                    case 94u:
                        goto st32;
                }
                if (9u <= (*p) && (*p) <= 10u) goto tr13;
                goto st0;
            tr13 :
#line 54 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st4;
            tr59 :
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
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr69 :
#line 31 "ragel/./text-grammar.rl"
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
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr83 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
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
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr94 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
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
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr109 :
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
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr122 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr131 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr141 :
#line 31 "ragel/./text-grammar.rl"
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
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr149 :
#line 31 "ragel/./text-grammar.rl"
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
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr189 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr200 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 54 "ragel/./text-grammar.rl"
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
#line 991 "ron/text-parser.cc"
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
                        goto st25;
                    case 62u:
                        goto st28;
                    case 63u:
                        goto tr14;
                    case 94u:
                        goto st32;
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
#line 54 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st5;
            tr62 :
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
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
                goto st5;
            tr72 :
#line 31 "ragel/./text-grammar.rl"
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
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
                goto st5;
            tr87 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
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
            tr97 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
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
            tr113 :
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
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
            tr125 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
            tr134 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
            tr144 :
#line 31 "ragel/./text-grammar.rl"
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
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
            tr152 :
#line 31 "ragel/./text-grammar.rl"
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
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
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
            tr178 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st5;
            tr193 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st5;
            tr203 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st5;
            tr213 :
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
#line 1230 "ron/text-parser.cc"
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
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st7;
            tr33 :
#line 8 "ragel/././utf8-grammar.rl"
            {
                cp = (*p);
            }
                goto st7;
            tr43 :
#line 60 "ragel/./text-grammar.rl"
            {
                cp = decode_esc((*p));
            }
                goto st7;
            tr49 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st7;
            tr55 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st7;
            st7:
                if (++p == pe) goto _test_eof7;
            case 7:
#line 1295 "ron/text-parser.cc"
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
                        if (192u <= (*p) && (*p) <= 223u) goto tr36;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr38;
                } else
                    goto tr37;
                goto tr33;
            tr28 :
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 25 "ragel/./text-grammar.rl"
                {
                    Slice the_str{strb, p};
                    // op_.AddAtom(Atom::String(body.range_of(the_str)));
                    op_.AddAtom(Atom{STRING, body.range_of(the_str)});
                }
                goto st8;
            tr34 :
#line 25 "ragel/./text-grammar.rl"
            {
                Slice the_str{strb, p};
                // op_.AddAtom(Atom::String(body.range_of(the_str)));
                op_.AddAtom(Atom{STRING, body.range_of(the_str)});
            }
                goto st8;
            tr39 :
#line 54 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st8;
            tr50 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 25 "ragel/./text-grammar.rl"
                {
                    Slice the_str{strb, p};
                    // op_.AddAtom(Atom::String(body.range_of(the_str)));
                    op_.AddAtom(Atom{STRING, body.range_of(the_str)});
                }
                goto st8;
            st8:
                if (++p == pe) goto _test_eof8;
            case 8:
#line 1358 "ron/text-parser.cc"
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
#line 54 "ragel/./text-grammar.rl"
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
#line 1397 "ron/text-parser.cc"
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
#line 54 "ragel/./text-grammar.rl"
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
#line 1419 "ron/text-parser.cc"
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
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st12;
            tr51 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
                goto st12;
            st12:
                if (++p == pe) goto _test_eof12;
            case 12:
#line 1443 "ron/text-parser.cc"
                switch ((*p)) {
                    case 34u:
                        goto tr43;
                    case 39u:
                        goto tr43;
                    case 47u:
                        goto tr43;
                    case 92u:
                        goto tr43;
                    case 98u:
                        goto tr43;
                    case 110u:
                        goto tr43;
                    case 114u:
                        goto tr43;
                    case 116u:
                        goto tr43;
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
                    if (48u <= (*p) && (*p) <= 57u) goto st17;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st17;
                } else
                    goto st17;
                goto st0;
            st17:
                if (++p == pe) goto _test_eof17;
            case 17:
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr50;
                    case 92u:
                        goto tr51;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr52;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr54;
                } else
                    goto tr53;
                goto tr49;
            tr30 :
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st18;
            tr36 :
#line 9 "ragel/././utf8-grammar.rl"
            {
                cp = (*p) & 0x1f;
            }
                goto st18;
            tr52 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st18;
            tr56 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st18;
            st18:
                if (++p == pe) goto _test_eof18;
            case 18:
#line 1557 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr55;
                goto st0;
            tr31 :
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st19;
            tr37 :
#line 10 "ragel/././utf8-grammar.rl"
            {
                cp = (*p) & 0xf;
            }
                goto st19;
            tr53 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st19;
            tr57 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st19;
            st19:
                if (++p == pe) goto _test_eof19;
            case 19:
#line 1585 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr56;
                goto st0;
            tr32 :
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st20;
            tr38 :
#line 11 "ragel/././utf8-grammar.rl"
            {
                cp = (*p) & 7;
            }
                goto st20;
            tr54 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st20;
            st20:
                if (++p == pe) goto _test_eof20;
            case 20:
#line 1609 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr57;
                goto st0;
            tr20 :
#line 15 "ragel/./text-grammar.rl"
            {
                intb = p;
            }
#line 30 "ragel/./text-grammar.rl"
                { floatb = p; }
                goto st21;
            st21:
                if (++p == pe) goto _test_eof21;
            case 21:
#line 1623 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st22;
                goto st0;
            st22:
                if (++p == pe) goto _test_eof22;
            case 22:
                switch ((*p)) {
                    case 13u:
                        goto tr59;
                    case 32u:
                        goto tr59;
                    case 33u:
                        goto tr60;
                    case 39u:
                        goto tr61;
                    case 40u:
                        goto tr62;
                    case 44u:
                        goto tr60;
                    case 46u:
                        goto st23;
                    case 59u:
                        goto tr60;
                    case 61u:
                        goto tr64;
                    case 62u:
                        goto tr65;
                    case 63u:
                        goto tr60;
                    case 69u:
                        goto st35;
                    case 94u:
                        goto tr67;
                    case 101u:
                        goto st35;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st22;
                } else if ((*p) >= 9u)
                    goto tr59;
                goto st0;
            st23:
                if (++p == pe) goto _test_eof23;
            case 23:
                if (48u <= (*p) && (*p) <= 57u) goto st24;
                goto st0;
            st24:
                if (++p == pe) goto _test_eof24;
            case 24:
                switch ((*p)) {
                    case 13u:
                        goto tr69;
                    case 32u:
                        goto tr69;
                    case 33u:
                        goto tr70;
                    case 39u:
                        goto tr71;
                    case 40u:
                        goto tr72;
                    case 44u:
                        goto tr70;
                    case 59u:
                        goto tr70;
                    case 61u:
                        goto tr73;
                    case 62u:
                        goto tr74;
                    case 63u:
                        goto tr70;
                    case 69u:
                        goto st35;
                    case 94u:
                        goto tr75;
                    case 101u:
                        goto st35;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st24;
                } else if ((*p) >= 9u)
                    goto tr69;
                goto st0;
            tr76 :
#line 54 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st25;
            tr64 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 25;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
                goto st25;
            tr73 :
#line 31 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 25;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
                goto st25;
            tr90 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 25;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st25;
            tr99 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 25;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st25;
            tr116 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 25;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 25;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st25;
            tr127 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 25;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st25;
            tr135 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 25;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st25;
            tr146 :
#line 31 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 25;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 25;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st25;
            tr153 :
#line 31 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 25;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 25;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st25;
            tr182 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st25;
            tr196 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st25;
            tr205 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st25;
            tr216 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st25;
            st25:
                if (++p == pe) goto _test_eof25;
            case 25:
#line 1890 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr76;
                    case 32u:
                        goto tr76;
                    case 43u:
                        goto tr77;
                    case 45u:
                        goto tr77;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr78;
                } else if ((*p) >= 9u)
                    goto tr76;
                goto st0;
            tr77 :
#line 15 "ragel/./text-grammar.rl"
            {
                intb = p;
            }
                goto st26;
            st26:
                if (++p == pe) goto _test_eof26;
            case 26:
#line 1911 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st27;
                goto st0;
            tr78 :
#line 15 "ragel/./text-grammar.rl"
            {
                intb = p;
            }
                goto st27;
            st27:
                if (++p == pe) goto _test_eof27;
            case 27:
#line 1923 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr59;
                    case 32u:
                        goto tr59;
                    case 33u:
                        goto tr60;
                    case 39u:
                        goto tr61;
                    case 40u:
                        goto tr62;
                    case 44u:
                        goto tr60;
                    case 59u:
                        goto tr60;
                    case 61u:
                        goto tr64;
                    case 62u:
                        goto tr65;
                    case 63u:
                        goto tr60;
                    case 94u:
                        goto tr67;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st27;
                } else if ((*p) >= 9u)
                    goto tr59;
                goto st0;
            tr80 :
#line 54 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st28;
            tr65 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 28;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
                goto st28;
            tr74 :
#line 31 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 28;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
                goto st28;
            tr91 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 28;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st28;
            tr100 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 28;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st28;
            tr117 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 28;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 28;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st28;
            tr128 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 28;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st28;
            tr136 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 28;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st28;
            tr147 :
#line 31 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 28;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 28;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st28;
            tr154 :
#line 31 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 28;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 28;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st28;
            tr183 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st28;
            tr197 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st28;
            tr206 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st28;
            tr217 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st28;
            st28:
                if (++p == pe) goto _test_eof28;
            case 28:
#line 2148 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr80;
                    case 32u:
                        goto tr80;
                    case 95u:
                        goto tr82;
                    case 126u:
                        goto tr82;
                }
                if ((*p) < 65u) {
                    if ((*p) > 10u) {
                        if (48u <= (*p) && (*p) <= 57u) goto tr81;
                    } else if ((*p) >= 9u)
                        goto tr80;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr82;
                    } else if ((*p) >= 71u)
                        goto tr82;
                } else
                    goto tr81;
                goto st0;
            tr81 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st29;
            st29:
                if (++p == pe) goto _test_eof29;
            case 29:
#line 2185 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr83;
                    case 32u:
                        goto tr83;
                    case 33u:
                        goto tr84;
                    case 39u:
                        goto tr86;
                    case 40u:
                        goto tr87;
                    case 44u:
                        goto tr84;
                    case 47u:
                        goto tr88;
                    case 59u:
                        goto tr84;
                    case 61u:
                        goto tr90;
                    case 62u:
                        goto tr91;
                    case 63u:
                        goto tr84;
                    case 94u:
                        goto tr92;
                    case 95u:
                        goto st39;
                    case 126u:
                        goto st39;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr85;
                    } else if ((*p) >= 9u)
                        goto tr83;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st39;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st39;
                    } else
                        goto st39;
                } else
                    goto tr85;
                goto st0;
            tr85 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 15 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st30;
            st30:
                if (++p == pe) goto _test_eof30;
            case 30:
#line 2230 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr93;
                    case 126u:
                        goto tr93;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr93;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr93;
                } else
                    goto tr93;
                goto st0;
            tr93 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st31;
            st31:
                if (++p == pe) goto _test_eof31;
            case 31:
#line 2252 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr94;
                    case 32u:
                        goto tr94;
                    case 33u:
                        goto tr95;
                    case 39u:
                        goto tr96;
                    case 40u:
                        goto tr97;
                    case 44u:
                        goto tr95;
                    case 59u:
                        goto tr95;
                    case 61u:
                        goto tr99;
                    case 62u:
                        goto tr100;
                    case 63u:
                        goto tr95;
                    case 94u:
                        goto tr101;
                    case 95u:
                        goto st31;
                    case 126u:
                        goto st31;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr94;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st31;
                    } else if ((*p) >= 65u)
                        goto st31;
                } else
                    goto st31;
                goto st0;
            tr102 :
#line 54 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st32;
            tr67 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 32;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
                goto st32;
            tr75 :
#line 31 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 32;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
                goto st32;
            tr92 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 32;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st32;
            tr101 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 37 "ragel/./text-grammar.rl"
                {
                    if (word_too_big(value) || word_too_big(origin)) {
                        cs = 0;
                        {
                            p++;
                            cs = 32;
                            goto _out;
                        }
                    }
                    op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st32;
            tr120 :
#line 16 "ragel/./text-grammar.rl"
            {
                Slice the_int(intb, p);
                if (the_int.size() >= 19 && int_too_big(the_int)) {
                    cs = 0;
                    {
                        p++;
                        cs = 32;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Integer(parse_int(the_int),
                // body.range_of(the_int)));
                // TODO atoms.emplace();
                op_.AddAtom(Atom{INT, body.range_of(the_int)});
                uuidb = nullptr;  // sabotage uuid
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 32;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st32;
            tr129 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 32;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st32;
            tr137 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 32;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st32;
            tr148 :
#line 31 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 32;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 17 "ragel/././uuid-grammar.rl"
                { origin = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 32;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st32;
            tr155 :
#line 31 "ragel/./text-grammar.rl"
            {
                Slice the_float{floatb, p};
                if (the_float.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 32;
                        goto _out;
                    }
                }
                // op_.AddAtom(Atom::Float(parse_float(the_float),
                // body.range_of(the_float)));
                op_.AddAtom(Atom{STRING, body.range_of(the_float)});
            }
#line 14 "ragel/././uuid-grammar.rl"
                { value = Slice{wordb, p}; }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 41 "ragel/./text-grammar.rl"
                {
                    if (uuidb != nullptr) {  // " 123 " is an int, not an UUID
                        if (word_too_big(value) || word_too_big(origin)) {
                            cs = 0;
                            {
                                p++;
                                cs = 32;
                                goto _out;
                            }
                        }
                        op_.AddAtom(Uuid{variety, value, version, origin});
                    }
                }
                goto st32;
            tr184 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st32;
            tr198 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st32;
            tr207 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st32;
            tr218 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st32;
            st32:
                if (++p == pe) goto _test_eof32;
            case 32:
#line 2485 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr102;
                    case 32u:
                        goto tr102;
                    case 43u:
                        goto tr103;
                    case 45u:
                        goto tr103;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr104;
                } else if ((*p) >= 9u)
                    goto tr102;
                goto st0;
            tr103 :
#line 30 "ragel/./text-grammar.rl"
            {
                floatb = p;
            }
                goto st33;
            st33:
                if (++p == pe) goto _test_eof33;
            case 33:
#line 2506 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st34;
                goto st0;
            tr104 :
#line 30 "ragel/./text-grammar.rl"
            {
                floatb = p;
            }
                goto st34;
            st34:
                if (++p == pe) goto _test_eof34;
            case 34:
#line 2518 "ron/text-parser.cc"
                switch ((*p)) {
                    case 46u:
                        goto st23;
                    case 69u:
                        goto st35;
                    case 101u:
                        goto st35;
                }
                if (48u <= (*p) && (*p) <= 57u) goto st34;
                goto st0;
            st35:
                if (++p == pe) goto _test_eof35;
            case 35:
                switch ((*p)) {
                    case 43u:
                        goto st36;
                    case 45u:
                        goto st36;
                }
                if (48u <= (*p) && (*p) <= 57u) goto st37;
                goto st0;
            st36:
                if (++p == pe) goto _test_eof36;
            case 36:
                if (48u <= (*p) && (*p) <= 57u) goto st37;
                goto st0;
            st37:
                if (++p == pe) goto _test_eof37;
            case 37:
                switch ((*p)) {
                    case 13u:
                        goto tr69;
                    case 32u:
                        goto tr69;
                    case 33u:
                        goto tr70;
                    case 39u:
                        goto tr71;
                    case 40u:
                        goto tr72;
                    case 44u:
                        goto tr70;
                    case 59u:
                        goto tr70;
                    case 61u:
                        goto tr73;
                    case 62u:
                        goto tr74;
                    case 63u:
                        goto tr70;
                    case 94u:
                        goto tr75;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st37;
                } else if ((*p) >= 9u)
                    goto tr69;
                goto st0;
            tr88 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st38;
            st38:
                if (++p == pe) goto _test_eof38;
            case 38:
#line 2576 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr108;
                    case 126u:
                        goto tr108;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr108;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr108;
                } else
                    goto tr108;
                goto st0;
            tr82 :
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
            tr108 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st39;
            st39:
                if (++p == pe) goto _test_eof39;
            case 39:
#line 2609 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr83;
                    case 32u:
                        goto tr83;
                    case 33u:
                        goto tr84;
                    case 39u:
                        goto tr86;
                    case 40u:
                        goto tr87;
                    case 44u:
                        goto tr84;
                    case 59u:
                        goto tr84;
                    case 61u:
                        goto tr90;
                    case 62u:
                        goto tr91;
                    case 63u:
                        goto tr84;
                    case 94u:
                        goto tr92;
                    case 95u:
                        goto st39;
                    case 126u:
                        goto st39;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr85;
                    } else if ((*p) >= 9u)
                        goto tr83;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st39;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st39;
                    } else
                        goto st39;
                } else
                    goto tr85;
                goto st0;
            tr21 :
#line 15 "ragel/./text-grammar.rl"
            {
                intb = p;
            }
#line 30 "ragel/./text-grammar.rl"
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
                goto st40;
            st40:
                if (++p == pe) goto _test_eof40;
            case 40:
#line 2662 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr109;
                    case 32u:
                        goto tr109;
                    case 33u:
                        goto tr110;
                    case 39u:
                        goto tr112;
                    case 40u:
                        goto tr113;
                    case 44u:
                        goto tr110;
                    case 46u:
                        goto st23;
                    case 47u:
                        goto tr114;
                    case 59u:
                        goto tr110;
                    case 61u:
                        goto tr116;
                    case 62u:
                        goto tr117;
                    case 63u:
                        goto tr110;
                    case 69u:
                        goto st46;
                    case 94u:
                        goto tr120;
                    case 95u:
                        goto st44;
                    case 101u:
                        goto st46;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr111;
                    } else if ((*p) >= 9u)
                        goto tr109;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st45;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr111;
                goto st0;
            tr111 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 15 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st41;
            st41:
                if (++p == pe) goto _test_eof41;
            case 41:
#line 2710 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr121;
                    case 126u:
                        goto tr121;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr121;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr121;
                } else
                    goto tr121;
                goto st0;
            tr121 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st42;
            st42:
                if (++p == pe) goto _test_eof42;
            case 42:
#line 2732 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr122;
                    case 32u:
                        goto tr122;
                    case 33u:
                        goto tr123;
                    case 39u:
                        goto tr124;
                    case 40u:
                        goto tr125;
                    case 44u:
                        goto tr123;
                    case 59u:
                        goto tr123;
                    case 61u:
                        goto tr127;
                    case 62u:
                        goto tr128;
                    case 63u:
                        goto tr123;
                    case 94u:
                        goto tr129;
                    case 95u:
                        goto st42;
                    case 126u:
                        goto st42;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr122;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st42;
                    } else if ((*p) >= 65u)
                        goto st42;
                } else
                    goto st42;
                goto st0;
            tr114 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st43;
            st43:
                if (++p == pe) goto _test_eof43;
            case 43:
#line 2768 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr130;
                    case 126u:
                        goto tr130;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr130;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr130;
                } else
                    goto tr130;
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
                goto st44;
            tr130 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st44;
            st44:
                if (++p == pe) goto _test_eof44;
            case 44:
#line 2801 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr131;
                    case 32u:
                        goto tr131;
                    case 33u:
                        goto tr132;
                    case 39u:
                        goto tr133;
                    case 40u:
                        goto tr134;
                    case 44u:
                        goto tr132;
                    case 59u:
                        goto tr132;
                    case 61u:
                        goto tr135;
                    case 62u:
                        goto tr136;
                    case 63u:
                        goto tr132;
                    case 94u:
                        goto tr137;
                    case 95u:
                        goto st44;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr111;
                    } else if ((*p) >= 9u)
                        goto tr131;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st44;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr111;
                goto st0;
            st45:
                if (++p == pe) goto _test_eof45;
            case 45:
                switch ((*p)) {
                    case 13u:
                        goto tr109;
                    case 32u:
                        goto tr109;
                    case 33u:
                        goto tr110;
                    case 39u:
                        goto tr112;
                    case 40u:
                        goto tr113;
                    case 44u:
                        goto tr110;
                    case 46u:
                        goto st23;
                    case 59u:
                        goto tr110;
                    case 61u:
                        goto tr116;
                    case 62u:
                        goto tr117;
                    case 63u:
                        goto tr110;
                    case 69u:
                        goto st46;
                    case 94u:
                        goto tr120;
                    case 95u:
                        goto st44;
                    case 101u:
                        goto st46;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr111;
                    } else if ((*p) >= 9u)
                        goto tr109;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st45;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr111;
                goto st0;
            st46:
                if (++p == pe) goto _test_eof46;
            case 46:
                switch ((*p)) {
                    case 13u:
                        goto tr131;
                    case 32u:
                        goto tr131;
                    case 33u:
                        goto tr132;
                    case 39u:
                        goto tr133;
                    case 40u:
                        goto tr134;
                    case 44u:
                        goto tr132;
                    case 59u:
                        goto tr132;
                    case 61u:
                        goto tr135;
                    case 62u:
                        goto tr136;
                    case 63u:
                        goto tr132;
                    case 94u:
                        goto tr137;
                    case 95u:
                        goto st44;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr111;
                    } else if ((*p) >= 9u)
                        goto tr131;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st49;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr138;
                goto st0;
            tr138 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 15 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st47;
            st47:
                if (++p == pe) goto _test_eof47;
            case 47:
#line 2922 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr121;
                    case 126u:
                        goto tr121;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr140;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr121;
                } else
                    goto tr121;
                goto st0;
            tr140 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st48;
            st48:
                if (++p == pe) goto _test_eof48;
            case 48:
#line 2944 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr141;
                    case 32u:
                        goto tr141;
                    case 33u:
                        goto tr142;
                    case 39u:
                        goto tr143;
                    case 40u:
                        goto tr144;
                    case 44u:
                        goto tr142;
                    case 59u:
                        goto tr142;
                    case 61u:
                        goto tr146;
                    case 62u:
                        goto tr147;
                    case 63u:
                        goto tr142;
                    case 94u:
                        goto tr148;
                    case 95u:
                        goto st42;
                    case 126u:
                        goto st42;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr141;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st42;
                    } else if ((*p) >= 65u)
                        goto st42;
                } else
                    goto st48;
                goto st0;
            st49:
                if (++p == pe) goto _test_eof49;
            case 49:
                switch ((*p)) {
                    case 13u:
                        goto tr149;
                    case 32u:
                        goto tr149;
                    case 33u:
                        goto tr150;
                    case 39u:
                        goto tr151;
                    case 40u:
                        goto tr152;
                    case 44u:
                        goto tr150;
                    case 59u:
                        goto tr150;
                    case 61u:
                        goto tr153;
                    case 62u:
                        goto tr154;
                    case 63u:
                        goto tr150;
                    case 94u:
                        goto tr155;
                    case 95u:
                        goto st44;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr111;
                    } else if ((*p) >= 9u)
                        goto tr149;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st49;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr111;
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
                goto st50;
            st50:
                if (++p == pe) goto _test_eof50;
            case 50:
#line 3024 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr131;
                    case 32u:
                        goto tr131;
                    case 33u:
                        goto tr132;
                    case 39u:
                        goto tr133;
                    case 40u:
                        goto tr134;
                    case 44u:
                        goto tr132;
                    case 47u:
                        goto tr114;
                    case 59u:
                        goto tr132;
                    case 61u:
                        goto tr135;
                    case 62u:
                        goto tr136;
                    case 63u:
                        goto tr132;
                    case 94u:
                        goto tr137;
                    case 95u:
                        goto st44;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr111;
                    } else if ((*p) >= 9u)
                        goto tr131;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st44;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr111;
                goto st0;
            tr3 :
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
                goto st51;
            tr164 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
                goto st51;
            st51:
                if (++p == pe) goto _test_eof51;
            case 51:
#line 3071 "ron/text-parser.cc"
                switch ((*p)) {
                    case 34u:
                        goto tr156;
                    case 39u:
                        goto tr156;
                    case 47u:
                        goto tr156;
                    case 92u:
                        goto tr156;
                    case 98u:
                        goto tr156;
                    case 110u:
                        goto tr156;
                    case 114u:
                        goto tr156;
                    case 116u:
                        goto tr156;
                    case 117u:
                        goto st52;
                }
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
                    if (48u <= (*p) && (*p) <= 57u) goto st55;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st55;
                } else
                    goto st55;
                goto st0;
            st55:
                if (++p == pe) goto _test_eof55;
            case 55:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st56;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st56;
                } else
                    goto st56;
                goto st0;
            st56:
                if (++p == pe) goto _test_eof56;
            case 56:
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr163;
                    case 92u:
                        goto tr164;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr165;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr167;
                } else
                    goto tr166;
                goto tr162;
            tr4 :
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st57;
            tr10 :
#line 9 "ragel/././utf8-grammar.rl"
            {
                cp = (*p) & 0x1f;
            }
                goto st57;
            tr165 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st57;
            tr169 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st57;
            st57:
                if (++p == pe) goto _test_eof57;
            case 57:
#line 3185 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr168;
                goto st0;
            tr5 :
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st58;
            tr11 :
#line 10 "ragel/././utf8-grammar.rl"
            {
                cp = (*p) & 0xf;
            }
                goto st58;
            tr166 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st58;
            tr170 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st58;
            st58:
                if (++p == pe) goto _test_eof58;
            case 58:
#line 3213 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr169;
                goto st0;
            tr6 :
#line 24 "ragel/./text-grammar.rl"
            {
                strb = p;
            }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st59;
            tr12 :
#line 11 "ragel/././utf8-grammar.rl"
            {
                cp = (*p) & 7;
            }
                goto st59;
            tr167 :
#line 61 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st59;
            st59:
                if (++p == pe) goto _test_eof59;
            case 59:
#line 3237 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr170;
                goto st0;
            st60:
                if (++p == pe) goto _test_eof60;
            case 60:
                if ((*p) == 10u) goto st75;
                goto st0;
            st75:
                if (++p == pe) goto _test_eof75;
            case 75:
                goto st0;
            st61:
                if (++p == pe) goto _test_eof61;
            case 61:
                switch ((*p)) {
                    case 95u:
                        goto tr173;
                    case 126u:
                        goto tr173;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr172;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr173;
                    } else if ((*p) >= 71u)
                        goto tr173;
                } else
                    goto tr172;
                goto st0;
            tr172 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st62;
            st62:
                if (++p == pe) goto _test_eof62;
            case 62:
#line 3288 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr174;
                    case 32u:
                        goto tr174;
                    case 33u:
                        goto tr175;
                    case 39u:
                        goto tr177;
                    case 40u:
                        goto tr178;
                    case 44u:
                        goto tr175;
                    case 47u:
                        goto tr179;
                    case 58u:
                        goto tr181;
                    case 59u:
                        goto tr175;
                    case 61u:
                        goto tr182;
                    case 62u:
                        goto tr183;
                    case 63u:
                        goto tr175;
                    case 94u:
                        goto tr184;
                    case 95u:
                        goto st73;
                    case 126u:
                        goto st73;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr176;
                    } else if ((*p) >= 9u)
                        goto tr174;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st73;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st73;
                    } else
                        goto st73;
                } else
                    goto tr176;
                goto st0;
            tr185 :
#line 54 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st63;
            tr174 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st63;
            tr210 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
#line 54 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st63;
            st63:
                if (++p == pe) goto _test_eof63;
            case 63:
#line 3375 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr185;
                    case 32u:
                        goto tr185;
                    case 33u:
                        goto tr14;
                    case 39u:
                        goto st1;
                    case 40u:
                        goto st5;
                    case 44u:
                        goto tr14;
                    case 58u:
                        goto st64;
                    case 59u:
                        goto tr14;
                    case 61u:
                        goto st25;
                    case 62u:
                        goto st28;
                    case 63u:
                        goto tr14;
                    case 94u:
                        goto st32;
                    case 95u:
                        goto tr23;
                    case 126u:
                        goto tr23;
                }
                if ((*p) < 48u) {
                    if ((*p) > 10u) {
                        if (43u <= (*p) && (*p) <= 45u) goto tr20;
                    } else if ((*p) >= 9u)
                        goto tr185;
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
            tr181 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st64;
            tr215 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st64;
            st64:
                if (++p == pe) goto _test_eof64;
            case 64:
#line 3438 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr188;
                    case 126u:
                        goto tr188;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr187;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr188;
                    } else if ((*p) >= 71u)
                        goto tr188;
                } else
                    goto tr187;
                goto st0;
            tr187 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st65;
            st65:
                if (++p == pe) goto _test_eof65;
            case 65:
#line 3470 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr189;
                    case 32u:
                        goto tr189;
                    case 33u:
                        goto tr190;
                    case 39u:
                        goto tr192;
                    case 40u:
                        goto tr193;
                    case 44u:
                        goto tr190;
                    case 47u:
                        goto tr194;
                    case 59u:
                        goto tr190;
                    case 61u:
                        goto tr196;
                    case 62u:
                        goto tr197;
                    case 63u:
                        goto tr190;
                    case 94u:
                        goto tr198;
                    case 95u:
                        goto st69;
                    case 126u:
                        goto st69;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr191;
                    } else if ((*p) >= 9u)
                        goto tr189;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st69;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st69;
                    } else
                        goto st69;
                } else
                    goto tr191;
                goto st0;
            tr191 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 15 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st66;
            st66:
                if (++p == pe) goto _test_eof66;
            case 66:
#line 3515 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr199;
                    case 126u:
                        goto tr199;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr199;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr199;
                } else
                    goto tr199;
                goto st0;
            tr199 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st67;
            st67:
                if (++p == pe) goto _test_eof67;
            case 67:
#line 3537 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr200;
                    case 32u:
                        goto tr200;
                    case 33u:
                        goto tr201;
                    case 39u:
                        goto tr202;
                    case 40u:
                        goto tr203;
                    case 44u:
                        goto tr201;
                    case 59u:
                        goto tr201;
                    case 61u:
                        goto tr205;
                    case 62u:
                        goto tr206;
                    case 63u:
                        goto tr201;
                    case 94u:
                        goto tr207;
                    case 95u:
                        goto st67;
                    case 126u:
                        goto st67;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr200;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st67;
                    } else if ((*p) >= 65u)
                        goto st67;
                } else
                    goto st67;
                goto st0;
            tr194 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st68;
            st68:
                if (++p == pe) goto _test_eof68;
            case 68:
#line 3573 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr208;
                    case 126u:
                        goto tr208;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr208;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr208;
                } else
                    goto tr208;
                goto st0;
            tr188 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st69;
            tr208 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st69;
            st69:
                if (++p == pe) goto _test_eof69;
            case 69:
#line 3606 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr189;
                    case 32u:
                        goto tr189;
                    case 33u:
                        goto tr190;
                    case 39u:
                        goto tr192;
                    case 40u:
                        goto tr193;
                    case 44u:
                        goto tr190;
                    case 59u:
                        goto tr190;
                    case 61u:
                        goto tr196;
                    case 62u:
                        goto tr197;
                    case 63u:
                        goto tr190;
                    case 94u:
                        goto tr198;
                    case 95u:
                        goto st69;
                    case 126u:
                        goto st69;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr191;
                    } else if ((*p) >= 9u)
                        goto tr189;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st69;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st69;
                    } else
                        goto st69;
                } else
                    goto tr191;
                goto st0;
            tr176 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 15 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st70;
            st70:
                if (++p == pe) goto _test_eof70;
            case 70:
#line 3650 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr209;
                    case 126u:
                        goto tr209;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr209;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr209;
                } else
                    goto tr209;
                goto st0;
            tr209 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st71;
            st71:
                if (++p == pe) goto _test_eof71;
            case 71:
#line 3672 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr210;
                    case 32u:
                        goto tr210;
                    case 33u:
                        goto tr211;
                    case 39u:
                        goto tr212;
                    case 40u:
                        goto tr213;
                    case 44u:
                        goto tr211;
                    case 58u:
                        goto tr215;
                    case 59u:
                        goto tr211;
                    case 61u:
                        goto tr216;
                    case 62u:
                        goto tr217;
                    case 63u:
                        goto tr211;
                    case 94u:
                        goto tr218;
                    case 95u:
                        goto st71;
                    case 126u:
                        goto st71;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr210;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st71;
                    } else if ((*p) >= 65u)
                        goto st71;
                } else
                    goto st71;
                goto st0;
            tr179 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st72;
            st72:
                if (++p == pe) goto _test_eof72;
            case 72:
#line 3709 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr219;
                    case 126u:
                        goto tr219;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr219;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr219;
                } else
                    goto tr219;
                goto st0;
            tr173 :
#line 6 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = Slice{};
                uuidb = p;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { wordb = p; }
                goto st73;
            tr219 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st73;
            st73:
                if (++p == pe) goto _test_eof73;
            case 73:
#line 3742 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr174;
                    case 32u:
                        goto tr174;
                    case 33u:
                        goto tr175;
                    case 39u:
                        goto tr177;
                    case 40u:
                        goto tr178;
                    case 44u:
                        goto tr175;
                    case 58u:
                        goto tr181;
                    case 59u:
                        goto tr175;
                    case 61u:
                        goto tr182;
                    case 62u:
                        goto tr183;
                    case 63u:
                        goto tr175;
                    case 94u:
                        goto tr184;
                    case 95u:
                        goto st73;
                    case 126u:
                        goto st73;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr176;
                    } else if ((*p) >= 9u)
                        goto tr174;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st73;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st73;
                    } else
                        goto st73;
                } else
                    goto tr176;
                goto st0;
        }
    _test_eof74:
        cs = 74;
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
    _test_eof59:
        cs = 59;
        goto _test_eof;
    _test_eof60:
        cs = 60;
        goto _test_eof;
    _test_eof75:
        cs = 75;
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
    _test_eof72:
        cs = 72;
        goto _test_eof;
    _test_eof73:
        cs = 73;
        goto _test_eof;

    _test_eof : {}
    _out : {}
    }

#line 64 "ragel/text-parser.rl"

    at_ = off_;
    off_ = p - pb;
    line_ = line;

    if (op_.size()) prev_id_ = op_.id();

    // std::cerr << "ending with [" <<p<<"] state "<<cs<<" "<<op_.size()<<"
    // atoms "<<(pe-p)<<" bytes left, prev_id_ "<<prev_id_.str()<<'\n';

    if (term && cs != RON_error) {
        op_.term_ = chr2term(term);  // FIXME gen a fn
        return Status::OK;
    } else if (cs >= RON_first_final) {
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
