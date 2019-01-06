
#line 1 "ragel/text-parser.rl"
#include <iostream>
#include "text.hpp"

namespace ron {

#line 7 "ragel/text-parser.rl"

#line 12 "ron/text-parser.cc"
static const int RON_start = 57;
static const int RON_first_final = 57;
static const int RON_error = 0;

static const int RON_en_main = 57;

#line 8 "ragel/text-parser.rl"

bool TextFrame::Cursor::Next() {
    Atoms& atoms = op_.atoms_;

    switch (cs) {
        case RON_error:
            if (off_ != 0) {
                return false;
            }

#line 36 "ron/text-parser.cc"
            { cs = RON_start; }

#line 23 "ragel/text-parser.rl"
            break;

        case RON_FULL_STOP:
            cs = RON_error;
            return false;

        default:
            break;
    }

    if (data().size() <= off_) {
        cs = RON_error;
        return false;
    }

    slice_t body{data()};
    const char* pb = body.buf_;
    const char* p = pb + off_;
    const char* pe = pb + body.size();
    const char* eof = pe;

    slice_t intb{p, 0};
    slice_t floatb{p, 0};
    slice_t strb{p, 0};
    slice_t uuidb{p, 0};
    const char* lastintb{0};
    char term{0};
    slice_t value, origin;
    char variety, version;

    atoms.resize(0);
    op_.AddAtom(prev_id_.inc());
    op_.AddAtom(prev_id_);

    // std::cerr<<"starting with "<<cs<<" ["<<p<<"]\n";

#line 79 "ron/text-parser.cc"
    {
        if (p == pe) goto _test_eof;
        switch (cs) {
        tr14 :
#line 38 "ragel/./text-grammar.rl"
        {
            term = (*p);
            pos_++;
            if (p < pe - 1) {
                p++;
                cs = 57;
                goto _out;
            }
        }
            goto st57;
        tr25 :
#line 14 "ragel/./text-grammar.rl"
        {
            intb.end(p);
            if (intb.size() > 21) {
                cs = 0;
                {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            op_.AddAtom(Atom::Integer(parse_int(intb), body.range_of(intb)));
            lastintb = intb.buf_;
        }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr34 :
#line 26 "ragel/./text-grammar.rl"
        {
            floatb.end(p);
            if (floatb.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            op_.AddAtom(
                Atom::Float(parse_float(floatb), body.range_of(floatb)));
        }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr45 :
#line 13 "ragel/././uuid-grammar.rl"
        {
            value.end(p);
        }
#line 17 "ragel/././uuid-grammar.rl"
            {
                if (value.size() > Word::MAX_BASE64_SIZE ||
                    origin.size() > Word::MAX_BASE64_SIZE) {
                    cs = 0;
                    {
                        p++;
                        cs = 57;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 31 "ragel/./text-grammar.rl"
            { op_.AddAtom(Uuid{variety, value, version, origin}); }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr55 :
#line 16 "ragel/././uuid-grammar.rl"
        {
            origin.end(p);
        }
#line 17 "ragel/././uuid-grammar.rl"
            {
                if (value.size() > Word::MAX_BASE64_SIZE ||
                    origin.size() > Word::MAX_BASE64_SIZE) {
                    cs = 0;
                    {
                        p++;
                        cs = 57;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 31 "ragel/./text-grammar.rl"
            { op_.AddAtom(Uuid{variety, value, version, origin}); }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr68 :
#line 14 "ragel/./text-grammar.rl"
        {
            intb.end(p);
            if (intb.size() > 21) {
                cs = 0;
                {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            op_.AddAtom(Atom::Integer(parse_int(intb), body.range_of(intb)));
            lastintb = intb.buf_;
        }
#line 13 "ragel/././uuid-grammar.rl"
            { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
            {
                if (value.size() > Word::MAX_BASE64_SIZE ||
                    origin.size() > Word::MAX_BASE64_SIZE) {
                    cs = 0;
                    {
                        p++;
                        cs = 57;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 34 "ragel/./text-grammar.rl"
            {
                if (!intb.same(uuidb))
                    op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr80 :
#line 16 "ragel/././uuid-grammar.rl"
        {
            origin.end(p);
        }
#line 17 "ragel/././uuid-grammar.rl"
            {
                if (value.size() > Word::MAX_BASE64_SIZE ||
                    origin.size() > Word::MAX_BASE64_SIZE) {
                    cs = 0;
                    {
                        p++;
                        cs = 57;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 34 "ragel/./text-grammar.rl"
            {
                if (!intb.same(uuidb))
                    op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr88 :
#line 13 "ragel/././uuid-grammar.rl"
        {
            value.end(p);
        }
#line 17 "ragel/././uuid-grammar.rl"
            {
                if (value.size() > Word::MAX_BASE64_SIZE ||
                    origin.size() > Word::MAX_BASE64_SIZE) {
                    cs = 0;
                    {
                        p++;
                        cs = 57;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 34 "ragel/./text-grammar.rl"
            {
                if (!intb.same(uuidb))
                    op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr97 :
#line 26 "ragel/./text-grammar.rl"
        {
            floatb.end(p);
            if (floatb.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            op_.AddAtom(
                Atom::Float(parse_float(floatb), body.range_of(floatb)));
        }
#line 16 "ragel/././uuid-grammar.rl"
            { origin.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
            {
                if (value.size() > Word::MAX_BASE64_SIZE ||
                    origin.size() > Word::MAX_BASE64_SIZE) {
                    cs = 0;
                    {
                        p++;
                        cs = 57;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 34 "ragel/./text-grammar.rl"
            {
                if (!intb.same(uuidb))
                    op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr104 :
#line 26 "ragel/./text-grammar.rl"
        {
            floatb.end(p);
            if (floatb.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            op_.AddAtom(
                Atom::Float(parse_float(floatb), body.range_of(floatb)));
        }
#line 13 "ragel/././uuid-grammar.rl"
            { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
            {
                if (value.size() > Word::MAX_BASE64_SIZE ||
                    origin.size() > Word::MAX_BASE64_SIZE) {
                    cs = 0;
                    {
                        p++;
                        cs = 57;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 34 "ragel/./text-grammar.rl"
            {
                if (!intb.same(uuidb))
                    op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr117 :
#line 13 "ragel/././uuid-grammar.rl"
        {
            value.end(p);
        }
#line 17 "ragel/././uuid-grammar.rl"
            {
                if (value.size() > Word::MAX_BASE64_SIZE ||
                    origin.size() > Word::MAX_BASE64_SIZE) {
                    cs = 0;
                    {
                        p++;
                        cs = 57;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 7 "ragel/./text-grammar.rl"
            { op_.SetId(Uuid{variety, value, version, origin}); }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr131 :
#line 13 "ragel/././uuid-grammar.rl"
        {
            value.end(p);
        }
#line 17 "ragel/././uuid-grammar.rl"
            {
                if (value.size() > Word::MAX_BASE64_SIZE ||
                    origin.size() > Word::MAX_BASE64_SIZE) {
                    cs = 0;
                    {
                        p++;
                        cs = 57;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 10 "ragel/./text-grammar.rl"
            { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr141 :
#line 16 "ragel/././uuid-grammar.rl"
        {
            origin.end(p);
        }
#line 17 "ragel/././uuid-grammar.rl"
            {
                if (value.size() > Word::MAX_BASE64_SIZE ||
                    origin.size() > Word::MAX_BASE64_SIZE) {
                    cs = 0;
                    {
                        p++;
                        cs = 57;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 10 "ragel/./text-grammar.rl"
            { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        tr150 :
#line 16 "ragel/././uuid-grammar.rl"
        {
            origin.end(p);
        }
#line 17 "ragel/././uuid-grammar.rl"
            {
                if (value.size() > Word::MAX_BASE64_SIZE ||
                    origin.size() > Word::MAX_BASE64_SIZE) {
                    cs = 0;
                    {
                        p++;
                        cs = 57;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 7 "ragel/./text-grammar.rl"
            { op_.SetId(Uuid{variety, value, version, origin}); }
#line 38 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 57;
                    goto _out;
                }
            }
            goto st57;
        st57:
            if (++p == pe) goto _test_eof57;
            case 57:
#line 392 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st57;
                    case 33u:
                        goto tr14;
                    case 39u:
                        goto st1;
                    case 44u:
                        goto tr14;
                    case 46u:
                        goto st43;
                    case 59u:
                        goto tr14;
                    case 61u:
                        goto st9;
                    case 62u:
                        goto st12;
                    case 63u:
                        goto tr14;
                    case 64u:
                        goto st44;
                    case 94u:
                        goto st16;
                    case 95u:
                        goto tr22;
                    case 126u:
                        goto tr22;
                }
                if ((*p) < 48u) {
                    if ((*p) > 13u) {
                        if (43u <= (*p) && (*p) <= 45u) goto tr19;
                    } else if ((*p) >= 9u)
                        goto st57;
                } else if ((*p) > 57u) {
                    if ((*p) < 71u) {
                        if (65u <= (*p) && (*p) <= 70u) goto tr21;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr22;
                    } else
                        goto tr22;
                } else
                    goto tr20;
                goto st0;
            st0:
                cs = 0;
                goto _out;
            tr26 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st1;
            tr35 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
                goto st1;
            tr47 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st1;
            tr56 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st1;
            tr70 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr81 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr89 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr98 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
#line 16 "ragel/././uuid-grammar.rl"
                { origin.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr105 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
#line 13 "ragel/././uuid-grammar.rl"
                { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr119 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st1;
            tr133 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st1;
            tr142 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st1;
            tr151 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st1;
            st1:
                if (++p == pe) goto _test_eof1;
            case 1:
#line 650 "ron/text-parser.cc"
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
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st2;
            st2:
                if (++p == pe) goto _test_eof2;
            case 2:
#line 680 "ron/text-parser.cc"
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr8;
                    case 92u:
                        goto st35;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto st40;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto st42;
                } else
                    goto st41;
                goto st2;
            tr2 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
#line 21 "ragel/./text-grammar.rl"
                {
                    strb.end(p);
                    op_.AddAtom(Atom::String(body.range_of(strb)));
                }
                goto st3;
            tr8 :
#line 21 "ragel/./text-grammar.rl"
            {
                strb.end(p);
                op_.AddAtom(Atom::String(body.range_of(strb)));
            }
                goto st3;
            st3:
                if (++p == pe) goto _test_eof3;
            case 3:
#line 722 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st4;
                    case 33u:
                        goto tr14;
                    case 39u:
                        goto st1;
                    case 44u:
                        goto tr14;
                    case 59u:
                        goto tr14;
                    case 61u:
                        goto st9;
                    case 62u:
                        goto st12;
                    case 63u:
                        goto tr14;
                    case 94u:
                        goto st16;
                }
                if (9u <= (*p) && (*p) <= 13u) goto st4;
                goto st0;
            tr24 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st4;
            tr33 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
                goto st4;
            tr44 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 4;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st4;
            tr54 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 4;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st4;
            tr67 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 4;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st4;
            tr79 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 4;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st4;
            tr87 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 4;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st4;
            tr96 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
#line 16 "ragel/././uuid-grammar.rl"
                { origin.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 4;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st4;
            tr103 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
#line 13 "ragel/././uuid-grammar.rl"
                { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 4;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st4;
            tr130 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 4;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st4;
            tr140 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 4;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st4;
            st4:
                if (++p == pe) goto _test_eof4;
            case 4:
#line 926 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st4;
                    case 33u:
                        goto tr14;
                    case 39u:
                        goto st1;
                    case 44u:
                        goto tr14;
                    case 59u:
                        goto tr14;
                    case 61u:
                        goto st9;
                    case 62u:
                        goto st12;
                    case 63u:
                        goto tr14;
                    case 94u:
                        goto st16;
                    case 95u:
                        goto tr22;
                    case 126u:
                        goto tr22;
                }
                if ((*p) < 48u) {
                    if ((*p) > 13u) {
                        if (43u <= (*p) && (*p) <= 45u) goto tr19;
                    } else if ((*p) >= 9u)
                        goto st4;
                } else if ((*p) > 57u) {
                    if ((*p) < 71u) {
                        if (65u <= (*p) && (*p) <= 70u) goto tr21;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr22;
                    } else
                        goto tr22;
                } else
                    goto tr20;
                goto st0;
            tr19 :
#line 13 "ragel/./text-grammar.rl"
            {
                intb.begin(p);
            }
#line 25 "ragel/./text-grammar.rl"
                { floatb.begin(p); }
                goto st5;
            st5:
                if (++p == pe) goto _test_eof5;
            case 5:
#line 968 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st6;
                goto st0;
            st6:
                if (++p == pe) goto _test_eof6;
            case 6:
                switch ((*p)) {
                    case 32u:
                        goto tr24;
                    case 33u:
                        goto tr25;
                    case 39u:
                        goto tr26;
                    case 44u:
                        goto tr25;
                    case 46u:
                        goto st7;
                    case 59u:
                        goto tr25;
                    case 61u:
                        goto tr28;
                    case 62u:
                        goto tr29;
                    case 63u:
                        goto tr25;
                    case 69u:
                        goto st19;
                    case 94u:
                        goto tr31;
                    case 101u:
                        goto st19;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st6;
                } else if ((*p) >= 9u)
                    goto tr24;
                goto st0;
            st7:
                if (++p == pe) goto _test_eof7;
            case 7:
                if (48u <= (*p) && (*p) <= 57u) goto st8;
                goto st0;
            st8:
                if (++p == pe) goto _test_eof8;
            case 8:
                switch ((*p)) {
                    case 32u:
                        goto tr33;
                    case 33u:
                        goto tr34;
                    case 39u:
                        goto tr35;
                    case 44u:
                        goto tr34;
                    case 59u:
                        goto tr34;
                    case 61u:
                        goto tr36;
                    case 62u:
                        goto tr37;
                    case 63u:
                        goto tr34;
                    case 69u:
                        goto st19;
                    case 94u:
                        goto tr38;
                    case 101u:
                        goto st19;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st8;
                } else if ((*p) >= 9u)
                    goto tr33;
                goto st0;
            tr28 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 9;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st9;
            tr36 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 9;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
                goto st9;
            tr50 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st9;
            tr58 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st9;
            tr73 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 9;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st9;
            tr83 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st9;
            tr90 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st9;
            tr100 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 9;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
#line 16 "ragel/././uuid-grammar.rl"
                { origin.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st9;
            tr106 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 9;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
#line 13 "ragel/././uuid-grammar.rl"
                { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st9;
            tr123 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st9;
            tr136 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st9;
            tr144 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st9;
            tr154 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st9;
            st9:
                if (++p == pe) goto _test_eof9;
            case 9:
#line 1247 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st9;
                    case 43u:
                        goto tr39;
                    case 45u:
                        goto tr39;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr40;
                } else if ((*p) >= 9u)
                    goto st9;
                goto st0;
            tr39 :
#line 13 "ragel/./text-grammar.rl"
            {
                intb.begin(p);
            }
                goto st10;
            st10:
                if (++p == pe) goto _test_eof10;
            case 10:
#line 1267 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st11;
                goto st0;
            tr40 :
#line 13 "ragel/./text-grammar.rl"
            {
                intb.begin(p);
            }
                goto st11;
            st11:
                if (++p == pe) goto _test_eof11;
            case 11:
#line 1279 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr24;
                    case 33u:
                        goto tr25;
                    case 39u:
                        goto tr26;
                    case 44u:
                        goto tr25;
                    case 59u:
                        goto tr25;
                    case 61u:
                        goto tr28;
                    case 62u:
                        goto tr29;
                    case 63u:
                        goto tr25;
                    case 94u:
                        goto tr31;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st11;
                } else if ((*p) >= 9u)
                    goto tr24;
                goto st0;
            tr29 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 12;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st12;
            tr37 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 12;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
                goto st12;
            tr51 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st12;
            tr59 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st12;
            tr74 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 12;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st12;
            tr84 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st12;
            tr91 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st12;
            tr101 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 12;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
#line 16 "ragel/././uuid-grammar.rl"
                { origin.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st12;
            tr107 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 12;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
#line 13 "ragel/././uuid-grammar.rl"
                { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st12;
            tr124 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st12;
            tr137 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st12;
            tr145 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st12;
            tr155 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st12;
            st12:
                if (++p == pe) goto _test_eof12;
            case 12:
#line 1518 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st12;
                    case 95u:
                        goto tr43;
                    case 126u:
                        goto tr43;
                }
                if ((*p) < 65u) {
                    if ((*p) > 13u) {
                        if (48u <= (*p) && (*p) <= 57u) goto tr42;
                    } else if ((*p) >= 9u)
                        goto st12;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr43;
                    } else if ((*p) >= 71u)
                        goto tr43;
                } else
                    goto tr42;
                goto st0;
            tr42 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st13;
            st13:
                if (++p == pe) goto _test_eof13;
            case 13:
#line 1554 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr44;
                    case 33u:
                        goto tr45;
                    case 39u:
                        goto tr47;
                    case 44u:
                        goto tr45;
                    case 47u:
                        goto tr48;
                    case 59u:
                        goto tr45;
                    case 61u:
                        goto tr50;
                    case 62u:
                        goto tr51;
                    case 63u:
                        goto tr45;
                    case 94u:
                        goto tr52;
                    case 95u:
                        goto st23;
                    case 126u:
                        goto st23;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr46;
                    } else if ((*p) >= 9u)
                        goto tr44;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st23;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st23;
                    } else
                        goto st23;
                } else
                    goto tr46;
                goto st0;
            tr46 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st14;
            st14:
                if (++p == pe) goto _test_eof14;
            case 14:
#line 1597 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr53;
                    case 126u:
                        goto tr53;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr53;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr53;
                } else
                    goto tr53;
                goto st0;
            tr53 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st15;
            st15:
                if (++p == pe) goto _test_eof15;
            case 15:
#line 1619 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr54;
                    case 33u:
                        goto tr55;
                    case 39u:
                        goto tr56;
                    case 44u:
                        goto tr55;
                    case 59u:
                        goto tr55;
                    case 61u:
                        goto tr58;
                    case 62u:
                        goto tr59;
                    case 63u:
                        goto tr55;
                    case 94u:
                        goto tr60;
                    case 95u:
                        goto st15;
                    case 126u:
                        goto st15;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 13u) goto tr54;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st15;
                    } else if ((*p) >= 65u)
                        goto st15;
                } else
                    goto st15;
                goto st0;
            tr31 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 16;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st16;
            tr38 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 16;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
                goto st16;
            tr52 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st16;
            tr60 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st16;
            tr77 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 16;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
#line 13 "ragel/././uuid-grammar.rl"
                { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st16;
            tr85 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st16;
            tr92 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st16;
            tr102 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 16;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
#line 16 "ragel/././uuid-grammar.rl"
                { origin.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st16;
            tr108 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 16;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
#line 13 "ragel/././uuid-grammar.rl"
                { value.end(p); }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 34 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st16;
            tr125 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st16;
            tr138 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st16;
            tr146 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st16;
            tr156 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st16;
            st16:
                if (++p == pe) goto _test_eof16;
            case 16:
#line 1866 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st16;
                    case 43u:
                        goto tr61;
                    case 45u:
                        goto tr61;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr62;
                } else if ((*p) >= 9u)
                    goto st16;
                goto st0;
            tr61 :
#line 25 "ragel/./text-grammar.rl"
            {
                floatb.begin(p);
            }
                goto st17;
            st17:
                if (++p == pe) goto _test_eof17;
            case 17:
#line 1886 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st18;
                goto st0;
            tr62 :
#line 25 "ragel/./text-grammar.rl"
            {
                floatb.begin(p);
            }
                goto st18;
            st18:
                if (++p == pe) goto _test_eof18;
            case 18:
#line 1898 "ron/text-parser.cc"
                switch ((*p)) {
                    case 46u:
                        goto st7;
                    case 69u:
                        goto st19;
                    case 101u:
                        goto st19;
                }
                if (48u <= (*p) && (*p) <= 57u) goto st18;
                goto st0;
            st19:
                if (++p == pe) goto _test_eof19;
            case 19:
                switch ((*p)) {
                    case 43u:
                        goto st20;
                    case 45u:
                        goto st20;
                }
                if (48u <= (*p) && (*p) <= 57u) goto st21;
                goto st0;
            st20:
                if (++p == pe) goto _test_eof20;
            case 20:
                if (48u <= (*p) && (*p) <= 57u) goto st21;
                goto st0;
            st21:
                if (++p == pe) goto _test_eof21;
            case 21:
                switch ((*p)) {
                    case 32u:
                        goto tr33;
                    case 33u:
                        goto tr34;
                    case 39u:
                        goto tr35;
                    case 44u:
                        goto tr34;
                    case 59u:
                        goto tr34;
                    case 61u:
                        goto tr36;
                    case 62u:
                        goto tr37;
                    case 63u:
                        goto tr34;
                    case 94u:
                        goto tr38;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st21;
                } else if ((*p) >= 9u)
                    goto tr33;
                goto st0;
            tr48 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st22;
            st22:
                if (++p == pe) goto _test_eof22;
            case 22:
#line 1954 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr66;
                    case 126u:
                        goto tr66;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr66;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr66;
                } else
                    goto tr66;
                goto st0;
            tr43 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st23;
            tr66 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st23;
            st23:
                if (++p == pe) goto _test_eof23;
            case 23:
#line 1987 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr44;
                    case 33u:
                        goto tr45;
                    case 39u:
                        goto tr47;
                    case 44u:
                        goto tr45;
                    case 59u:
                        goto tr45;
                    case 61u:
                        goto tr50;
                    case 62u:
                        goto tr51;
                    case 63u:
                        goto tr45;
                    case 94u:
                        goto tr52;
                    case 95u:
                        goto st23;
                    case 126u:
                        goto st23;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr46;
                    } else if ((*p) >= 9u)
                        goto tr44;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st23;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st23;
                    } else
                        goto st23;
                } else
                    goto tr46;
                goto st0;
            tr20 :
#line 13 "ragel/./text-grammar.rl"
            {
                intb.begin(p);
            }
#line 25 "ragel/./text-grammar.rl"
                { floatb.begin(p); }
#line 5 "ragel/././uuid-grammar.rl"
                {
                    variety = '0';
                    version = '$';
                    origin = slice_t{};
                    uuidb.begin(p);
                }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st24;
            st24:
                if (++p == pe) goto _test_eof24;
            case 24:
#line 2038 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr67;
                    case 33u:
                        goto tr68;
                    case 39u:
                        goto tr70;
                    case 44u:
                        goto tr68;
                    case 46u:
                        goto st7;
                    case 47u:
                        goto tr71;
                    case 59u:
                        goto tr68;
                    case 61u:
                        goto tr73;
                    case 62u:
                        goto tr74;
                    case 63u:
                        goto tr68;
                    case 69u:
                        goto st30;
                    case 94u:
                        goto tr77;
                    case 95u:
                        goto st28;
                    case 101u:
                        goto st30;
                    case 126u:
                        goto st28;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr69;
                    } else if ((*p) >= 9u)
                        goto tr67;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st29;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr69;
                goto st0;
            tr69 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st25;
            st25:
                if (++p == pe) goto _test_eof25;
            case 25:
#line 2084 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr78;
                    case 126u:
                        goto tr78;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr78;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr78;
                } else
                    goto tr78;
                goto st0;
            tr78 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st26;
            st26:
                if (++p == pe) goto _test_eof26;
            case 26:
#line 2106 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr79;
                    case 33u:
                        goto tr80;
                    case 39u:
                        goto tr81;
                    case 44u:
                        goto tr80;
                    case 59u:
                        goto tr80;
                    case 61u:
                        goto tr83;
                    case 62u:
                        goto tr84;
                    case 63u:
                        goto tr80;
                    case 94u:
                        goto tr85;
                    case 95u:
                        goto st26;
                    case 126u:
                        goto st26;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 13u) goto tr79;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st26;
                    } else if ((*p) >= 65u)
                        goto st26;
                } else
                    goto st26;
                goto st0;
            tr71 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st27;
            st27:
                if (++p == pe) goto _test_eof27;
            case 27:
#line 2140 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr86;
                    case 126u:
                        goto tr86;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr86;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr86;
                } else
                    goto tr86;
                goto st0;
            tr22 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st28;
            tr86 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st28;
            st28:
                if (++p == pe) goto _test_eof28;
            case 28:
#line 2173 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr87;
                    case 33u:
                        goto tr88;
                    case 39u:
                        goto tr89;
                    case 44u:
                        goto tr88;
                    case 59u:
                        goto tr88;
                    case 61u:
                        goto tr90;
                    case 62u:
                        goto tr91;
                    case 63u:
                        goto tr88;
                    case 94u:
                        goto tr92;
                    case 95u:
                        goto st28;
                    case 126u:
                        goto st28;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr69;
                    } else if ((*p) >= 9u)
                        goto tr87;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st28;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr69;
                goto st0;
            st29:
                if (++p == pe) goto _test_eof29;
            case 29:
                switch ((*p)) {
                    case 32u:
                        goto tr67;
                    case 33u:
                        goto tr68;
                    case 39u:
                        goto tr70;
                    case 44u:
                        goto tr68;
                    case 46u:
                        goto st7;
                    case 59u:
                        goto tr68;
                    case 61u:
                        goto tr73;
                    case 62u:
                        goto tr74;
                    case 63u:
                        goto tr68;
                    case 69u:
                        goto st30;
                    case 94u:
                        goto tr77;
                    case 95u:
                        goto st28;
                    case 101u:
                        goto st30;
                    case 126u:
                        goto st28;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr69;
                    } else if ((*p) >= 9u)
                        goto tr67;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st29;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr69;
                goto st0;
            st30:
                if (++p == pe) goto _test_eof30;
            case 30:
                switch ((*p)) {
                    case 32u:
                        goto tr87;
                    case 33u:
                        goto tr88;
                    case 39u:
                        goto tr89;
                    case 44u:
                        goto tr88;
                    case 59u:
                        goto tr88;
                    case 61u:
                        goto tr90;
                    case 62u:
                        goto tr91;
                    case 63u:
                        goto tr88;
                    case 94u:
                        goto tr92;
                    case 95u:
                        goto st28;
                    case 126u:
                        goto st28;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr69;
                    } else if ((*p) >= 9u)
                        goto tr87;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st33;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr93;
                goto st0;
            tr93 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st31;
            st31:
                if (++p == pe) goto _test_eof31;
            case 31:
#line 2288 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr78;
                    case 126u:
                        goto tr78;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr95;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr78;
                } else
                    goto tr78;
                goto st0;
            tr95 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st32;
            st32:
                if (++p == pe) goto _test_eof32;
            case 32:
#line 2310 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr96;
                    case 33u:
                        goto tr97;
                    case 39u:
                        goto tr98;
                    case 44u:
                        goto tr97;
                    case 59u:
                        goto tr97;
                    case 61u:
                        goto tr100;
                    case 62u:
                        goto tr101;
                    case 63u:
                        goto tr97;
                    case 94u:
                        goto tr102;
                    case 95u:
                        goto st26;
                    case 126u:
                        goto st26;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 13u) goto tr96;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st26;
                    } else if ((*p) >= 65u)
                        goto st26;
                } else
                    goto st32;
                goto st0;
            st33:
                if (++p == pe) goto _test_eof33;
            case 33:
                switch ((*p)) {
                    case 32u:
                        goto tr103;
                    case 33u:
                        goto tr104;
                    case 39u:
                        goto tr105;
                    case 44u:
                        goto tr104;
                    case 59u:
                        goto tr104;
                    case 61u:
                        goto tr106;
                    case 62u:
                        goto tr107;
                    case 63u:
                        goto tr104;
                    case 94u:
                        goto tr108;
                    case 95u:
                        goto st28;
                    case 126u:
                        goto st28;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr69;
                    } else if ((*p) >= 9u)
                        goto tr103;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st33;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr69;
                goto st0;
            tr21 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st34;
            st34:
                if (++p == pe) goto _test_eof34;
            case 34:
#line 2386 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr87;
                    case 33u:
                        goto tr88;
                    case 39u:
                        goto tr89;
                    case 44u:
                        goto tr88;
                    case 47u:
                        goto tr71;
                    case 59u:
                        goto tr88;
                    case 61u:
                        goto tr90;
                    case 62u:
                        goto tr91;
                    case 63u:
                        goto tr88;
                    case 94u:
                        goto tr92;
                    case 95u:
                        goto st28;
                    case 126u:
                        goto st28;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr69;
                    } else if ((*p) >= 9u)
                        goto tr87;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st28;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr69;
                goto st0;
            tr3 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st35;
            st35:
                if (++p == pe) goto _test_eof35;
            case 35:
#line 2427 "ron/text-parser.cc"
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
                        goto st36;
                }
                goto st0;
            st36:
                if (++p == pe) goto _test_eof36;
            case 36:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st37;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st37;
                } else
                    goto st37;
                goto st0;
            st37:
                if (++p == pe) goto _test_eof37;
            case 37:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st38;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st38;
                } else
                    goto st38;
                goto st0;
            st38:
                if (++p == pe) goto _test_eof38;
            case 38:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st39;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st39;
                } else
                    goto st39;
                goto st0;
            st39:
                if (++p == pe) goto _test_eof39;
            case 39:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st2;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st2;
                } else
                    goto st2;
                goto st0;
            tr4 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st40;
            st40:
                if (++p == pe) goto _test_eof40;
            case 40:
#line 2500 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st2;
                goto st0;
            tr5 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st41;
            st41:
                if (++p == pe) goto _test_eof41;
            case 41:
#line 2512 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st40;
                goto st0;
            tr6 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st42;
            st42:
                if (++p == pe) goto _test_eof42;
            case 42:
#line 2524 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st41;
                goto st0;
            st43:
                if (++p == pe) goto _test_eof43;
            case 43:
                if ((*p) == 10u) goto st58;
                goto st0;
            st58:
                if (++p == pe) goto _test_eof58;
            case 58:
                goto st0;
            st44:
                if (++p == pe) goto _test_eof44;
            case 44:
                switch ((*p)) {
                    case 95u:
                        goto tr115;
                    case 126u:
                        goto tr115;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr114;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr115;
                    } else if ((*p) >= 71u)
                        goto tr115;
                } else
                    goto tr114;
                goto st0;
            tr114 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st45;
            st45:
                if (++p == pe) goto _test_eof45;
            case 45:
#line 2575 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr116;
                    case 33u:
                        goto tr117;
                    case 39u:
                        goto tr119;
                    case 44u:
                        goto tr117;
                    case 47u:
                        goto tr120;
                    case 58u:
                        goto tr122;
                    case 59u:
                        goto tr117;
                    case 61u:
                        goto tr123;
                    case 62u:
                        goto tr124;
                    case 63u:
                        goto tr117;
                    case 94u:
                        goto tr125;
                    case 95u:
                        goto st56;
                    case 126u:
                        goto st56;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr118;
                    } else if ((*p) >= 9u)
                        goto tr116;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st56;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st56;
                    } else
                        goto st56;
                } else
                    goto tr118;
                goto st0;
            tr116 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 46;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st46;
            tr149 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 46;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st46;
            st46:
                if (++p == pe) goto _test_eof46;
            case 46:
#line 2645 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st46;
                    case 33u:
                        goto tr14;
                    case 39u:
                        goto st1;
                    case 44u:
                        goto tr14;
                    case 58u:
                        goto st47;
                    case 59u:
                        goto tr14;
                    case 61u:
                        goto st9;
                    case 62u:
                        goto st12;
                    case 63u:
                        goto tr14;
                    case 94u:
                        goto st16;
                    case 95u:
                        goto tr22;
                    case 126u:
                        goto tr22;
                }
                if ((*p) < 48u) {
                    if ((*p) > 13u) {
                        if (43u <= (*p) && (*p) <= 45u) goto tr19;
                    } else if ((*p) >= 9u)
                        goto st46;
                } else if ((*p) > 57u) {
                    if ((*p) < 71u) {
                        if (65u <= (*p) && (*p) <= 70u) goto tr21;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr22;
                    } else
                        goto tr22;
                } else
                    goto tr20;
                goto st0;
            tr122 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 47;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st47;
            tr153 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                origin.end(p);
            }
#line 17 "ragel/././uuid-grammar.rl"
                {
                    if (value.size() > Word::MAX_BASE64_SIZE ||
                        origin.size() > Word::MAX_BASE64_SIZE) {
                        cs = 0;
                        {
                            p++;
                            cs = 47;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st47;
            st47:
                if (++p == pe) goto _test_eof47;
            case 47:
#line 2714 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr129;
                    case 126u:
                        goto tr129;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr128;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr129;
                    } else if ((*p) >= 71u)
                        goto tr129;
                } else
                    goto tr128;
                goto st0;
            tr128 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st48;
            st48:
                if (++p == pe) goto _test_eof48;
            case 48:
#line 2746 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr130;
                    case 33u:
                        goto tr131;
                    case 39u:
                        goto tr133;
                    case 44u:
                        goto tr131;
                    case 47u:
                        goto tr134;
                    case 59u:
                        goto tr131;
                    case 61u:
                        goto tr136;
                    case 62u:
                        goto tr137;
                    case 63u:
                        goto tr131;
                    case 94u:
                        goto tr138;
                    case 95u:
                        goto st52;
                    case 126u:
                        goto st52;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr132;
                    } else if ((*p) >= 9u)
                        goto tr130;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st52;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st52;
                    } else
                        goto st52;
                } else
                    goto tr132;
                goto st0;
            tr132 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st49;
            st49:
                if (++p == pe) goto _test_eof49;
            case 49:
#line 2789 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr139;
                    case 126u:
                        goto tr139;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr139;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr139;
                } else
                    goto tr139;
                goto st0;
            tr139 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st50;
            st50:
                if (++p == pe) goto _test_eof50;
            case 50:
#line 2811 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr140;
                    case 33u:
                        goto tr141;
                    case 39u:
                        goto tr142;
                    case 44u:
                        goto tr141;
                    case 59u:
                        goto tr141;
                    case 61u:
                        goto tr144;
                    case 62u:
                        goto tr145;
                    case 63u:
                        goto tr141;
                    case 94u:
                        goto tr146;
                    case 95u:
                        goto st50;
                    case 126u:
                        goto st50;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 13u) goto tr140;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st50;
                    } else if ((*p) >= 65u)
                        goto st50;
                } else
                    goto st50;
                goto st0;
            tr134 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st51;
            st51:
                if (++p == pe) goto _test_eof51;
            case 51:
#line 2845 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr147;
                    case 126u:
                        goto tr147;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr147;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr147;
                } else
                    goto tr147;
                goto st0;
            tr129 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st52;
            tr147 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st52;
            st52:
                if (++p == pe) goto _test_eof52;
            case 52:
#line 2878 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr130;
                    case 33u:
                        goto tr131;
                    case 39u:
                        goto tr133;
                    case 44u:
                        goto tr131;
                    case 59u:
                        goto tr131;
                    case 61u:
                        goto tr136;
                    case 62u:
                        goto tr137;
                    case 63u:
                        goto tr131;
                    case 94u:
                        goto tr138;
                    case 95u:
                        goto st52;
                    case 126u:
                        goto st52;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr132;
                    } else if ((*p) >= 9u)
                        goto tr130;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st52;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st52;
                    } else
                        goto st52;
                } else
                    goto tr132;
                goto st0;
            tr118 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st53;
            st53:
                if (++p == pe) goto _test_eof53;
            case 53:
#line 2920 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr148;
                    case 126u:
                        goto tr148;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr148;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr148;
                } else
                    goto tr148;
                goto st0;
            tr148 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st54;
            st54:
                if (++p == pe) goto _test_eof54;
            case 54:
#line 2942 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr149;
                    case 33u:
                        goto tr150;
                    case 39u:
                        goto tr151;
                    case 44u:
                        goto tr150;
                    case 58u:
                        goto tr153;
                    case 59u:
                        goto tr150;
                    case 61u:
                        goto tr154;
                    case 62u:
                        goto tr155;
                    case 63u:
                        goto tr150;
                    case 94u:
                        goto tr156;
                    case 95u:
                        goto st54;
                    case 126u:
                        goto st54;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 13u) goto tr149;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st54;
                    } else if ((*p) >= 65u)
                        goto st54;
                } else
                    goto st54;
                goto st0;
            tr120 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st55;
            st55:
                if (++p == pe) goto _test_eof55;
            case 55:
#line 2977 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr157;
                    case 126u:
                        goto tr157;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr157;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr157;
                } else
                    goto tr157;
                goto st0;
            tr115 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st56;
            tr157 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st56;
            st56:
                if (++p == pe) goto _test_eof56;
            case 56:
#line 3010 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr116;
                    case 33u:
                        goto tr117;
                    case 39u:
                        goto tr119;
                    case 44u:
                        goto tr117;
                    case 58u:
                        goto tr122;
                    case 59u:
                        goto tr117;
                    case 61u:
                        goto tr123;
                    case 62u:
                        goto tr124;
                    case 63u:
                        goto tr117;
                    case 94u:
                        goto tr125;
                    case 95u:
                        goto st56;
                    case 126u:
                        goto st56;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr118;
                    } else if ((*p) >= 9u)
                        goto tr116;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st56;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st56;
                    } else
                        goto st56;
                } else
                    goto tr118;
                goto st0;
        }
    _test_eof57:
        cs = 57;
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
    _test_eof58:
        cs = 58;
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

    _test_eof : {}
    _out : {}
    }

#line 63 "ragel/text-parser.rl"

    off_ = p - pb;

    if (op_.size()) prev_id_ = op_.id();

    // std::cerr << "ending with [" <<p<<"] state "<<cs<<" "<<op_.size()<<"
    // atoms "<<(pe-p)<<" bytes left, prev_id_ "<<prev_id_.str()<<'\n';

    if (term && cs != RON_error) {
        return true;
    } else {
        cs = RON_error;
        return false;
    }
}

}  // namespace ron
