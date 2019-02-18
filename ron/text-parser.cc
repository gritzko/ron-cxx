
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

    atoms.clear();
    op_.AddAtom(prev_id_.inc());
    op_.AddAtom(prev_id_);

    // std::cerr<<"starting with "<<cs<<" ["<<p<<"]\n";

#line 79 "ron/text-parser.cc"
    {
        if (p == pe) goto _test_eof;
        switch (cs) {
        tr14 :
#line 40 "ragel/./text-grammar.rl"
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
        tr47 :
#line 14 "ragel/./text-grammar.rl"
        {
            intb.end(p);
            if (intb.size() > 21) {
                cs = 0;
                {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            op_.AddAtom(Atom::Integer(parse_int(intb), body.range_of(intb)));
            lastintb = intb.buf_;
        }
#line 40 "ragel/./text-grammar.rl"
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
        tr57 :
#line 26 "ragel/./text-grammar.rl"
        {
            floatb.end(p);
            if (floatb.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 72;
                    goto _out;
                }
            }
            op_.AddAtom(
                Atom::Float(parse_float(floatb), body.range_of(floatb)));
        }
#line 40 "ragel/./text-grammar.rl"
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
        tr69 :
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
                        cs = 72;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 31 "ragel/./text-grammar.rl"
            { op_.AddAtom(Uuid{variety, value, version, origin}); }
#line 40 "ragel/./text-grammar.rl"
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
                        cs = 72;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 31 "ragel/./text-grammar.rl"
            { op_.AddAtom(Uuid{variety, value, version, origin}); }
#line 40 "ragel/./text-grammar.rl"
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
        tr94 :
#line 14 "ragel/./text-grammar.rl"
        {
            intb.end(p);
            if (intb.size() > 21) {
                cs = 0;
                {
                    p++;
                    cs = 72;
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
                        cs = 72;
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
#line 40 "ragel/./text-grammar.rl"
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
        tr107 :
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
                        cs = 72;
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
#line 40 "ragel/./text-grammar.rl"
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
                        cs = 72;
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
#line 40 "ragel/./text-grammar.rl"
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
        tr126 :
#line 26 "ragel/./text-grammar.rl"
        {
            floatb.end(p);
            if (floatb.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 72;
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
                        cs = 72;
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
#line 40 "ragel/./text-grammar.rl"
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
        tr134 :
#line 26 "ragel/./text-grammar.rl"
        {
            floatb.end(p);
            if (floatb.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 72;
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
                        cs = 72;
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
#line 40 "ragel/./text-grammar.rl"
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
        tr148 :
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
                        cs = 72;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 7 "ragel/./text-grammar.rl"
            { op_.SetId(Uuid{variety, value, version, origin}); }
#line 40 "ragel/./text-grammar.rl"
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
        tr163 :
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
                        cs = 72;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 10 "ragel/./text-grammar.rl"
            { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 40 "ragel/./text-grammar.rl"
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
        tr174 :
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
                        cs = 72;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 10 "ragel/./text-grammar.rl"
            { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 40 "ragel/./text-grammar.rl"
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
        tr184 :
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
                        cs = 72;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 7 "ragel/./text-grammar.rl"
            { op_.SetId(Uuid{variety, value, version, origin}); }
#line 40 "ragel/./text-grammar.rl"
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
#line 392 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st72;
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
                    if ((*p) > 13u) {
                        if (43u <= (*p) && (*p) <= 45u) goto tr20;
                    } else if ((*p) >= 9u)
                        goto st72;
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
            tr48 :
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
            tr58 :
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
            tr71 :
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
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st1;
            tr96 :
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
            tr108 :
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
            tr127 :
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
            tr135 :
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
            tr150 :
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
            tr165 :
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
            tr175 :
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
            tr185 :
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
#line 651 "ron/text-parser.cc"
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
#line 681 "ron/text-parser.cc"
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
#line 723 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st4;
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
                if (9u <= (*p) && (*p) <= 13u) goto st4;
                goto st0;
            tr46 :
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
            tr56 :
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
            tr68 :
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
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st4;
            tr93 :
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
            tr106 :
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
            tr115 :
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
            tr125 :
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
            tr133 :
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
            tr162 :
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
            tr173 :
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
#line 928 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st4;
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
                    if ((*p) > 13u) {
                        if (43u <= (*p) && (*p) <= 45u) goto tr20;
                    } else if ((*p) >= 9u)
                        goto st4;
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
            tr49 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st5;
            tr59 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
                goto st5;
            tr72 :
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
                            cs = 5;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st5;
            tr82 :
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
                            cs = 5;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st5;
            tr97 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
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
                            cs = 5;
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
                goto st5;
            tr109 :
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
                            cs = 5;
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
                goto st5;
            tr118 :
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
                            cs = 5;
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
                goto st5;
            tr128 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
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
                            cs = 5;
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
                goto st5;
            tr136 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
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
                            cs = 5;
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
                goto st5;
            tr151 :
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
                            cs = 5;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st5;
            tr166 :
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
                            cs = 5;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st5;
            tr176 :
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
                            cs = 5;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st5;
            tr186 :
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
                            cs = 5;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st5;
            st5:
                if (++p == pe) goto _test_eof5;
            case 5:
#line 1182 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st5;
                    case 39u:
                        goto st6;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st9;
                } else if ((*p) >= 9u)
                    goto st5;
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
                        goto tr27;
                    case 92u:
                        goto tr28;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr29;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr31;
                } else
                    goto tr30;
                goto tr26;
            tr26 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st7;
            st7:
                if (++p == pe) goto _test_eof7;
            case 7:
#line 1226 "ron/text-parser.cc"
                switch ((*p)) {
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr33;
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
            tr27 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
#line 21 "ragel/./text-grammar.rl"
                {
                    strb.end(p);
                    op_.AddAtom(Atom::String(body.range_of(strb)));
                }
                goto st8;
            tr33 :
#line 21 "ragel/./text-grammar.rl"
            {
                strb.end(p);
                op_.AddAtom(Atom::String(body.range_of(strb)));
            }
                goto st8;
            st8:
                if (++p == pe) goto _test_eof8;
            case 8:
#line 1268 "ron/text-parser.cc"
                if ((*p) == 32u) goto st8;
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st9;
                } else if ((*p) >= 9u)
                    goto st8;
                goto st0;
            st9:
                if (++p == pe) goto _test_eof9;
            case 9:
                switch ((*p)) {
                    case 32u:
                        goto st10;
                    case 41u:
                        goto st11;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st9;
                } else if ((*p) >= 9u)
                    goto st10;
                goto st0;
            st10:
                if (++p == pe) goto _test_eof10;
            case 10:
                switch ((*p)) {
                    case 32u:
                        goto st10;
                    case 41u:
                        goto st11;
                }
                if (9u <= (*p) && (*p) <= 13u) goto st10;
                goto st0;
            st11:
                if (++p == pe) goto _test_eof11;
            case 11:
                switch ((*p)) {
                    case 32u:
                        goto st11;
                    case 33u:
                        goto tr14;
                    case 44u:
                        goto tr14;
                    case 59u:
                        goto tr14;
                    case 63u:
                        goto tr14;
                }
                if (9u <= (*p) && (*p) <= 13u) goto st11;
                goto st0;
            tr28 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st12;
            st12:
                if (++p == pe) goto _test_eof12;
            case 12:
#line 1324 "ron/text-parser.cc"
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
            tr29 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st17;
            st17:
                if (++p == pe) goto _test_eof17;
            case 17:
#line 1397 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st7;
                goto st0;
            tr30 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st18;
            st18:
                if (++p == pe) goto _test_eof18;
            case 18:
#line 1409 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st17;
                goto st0;
            tr31 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st19;
            st19:
                if (++p == pe) goto _test_eof19;
            case 19:
#line 1421 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st18;
                goto st0;
            tr20 :
#line 13 "ragel/./text-grammar.rl"
            {
                intb.begin(p);
            }
#line 25 "ragel/./text-grammar.rl"
                { floatb.begin(p); }
                goto st20;
            st20:
                if (++p == pe) goto _test_eof20;
            case 20:
#line 1435 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st21;
                goto st0;
            st21:
                if (++p == pe) goto _test_eof21;
            case 21:
                switch ((*p)) {
                    case 32u:
                        goto tr46;
                    case 33u:
                        goto tr47;
                    case 39u:
                        goto tr48;
                    case 40u:
                        goto tr49;
                    case 44u:
                        goto tr47;
                    case 46u:
                        goto st22;
                    case 59u:
                        goto tr47;
                    case 61u:
                        goto tr51;
                    case 62u:
                        goto tr52;
                    case 63u:
                        goto tr47;
                    case 69u:
                        goto st34;
                    case 94u:
                        goto tr54;
                    case 101u:
                        goto st34;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st21;
                } else if ((*p) >= 9u)
                    goto tr46;
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
                    case 32u:
                        goto tr56;
                    case 33u:
                        goto tr57;
                    case 39u:
                        goto tr58;
                    case 40u:
                        goto tr59;
                    case 44u:
                        goto tr57;
                    case 59u:
                        goto tr57;
                    case 61u:
                        goto tr60;
                    case 62u:
                        goto tr61;
                    case 63u:
                        goto tr57;
                    case 69u:
                        goto st34;
                    case 94u:
                        goto tr62;
                    case 101u:
                        goto st34;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st23;
                } else if ((*p) >= 9u)
                    goto tr56;
                goto st0;
            tr51 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 24;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st24;
            tr60 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 24;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
                goto st24;
            tr75 :
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
                            cs = 24;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st24;
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
                            cs = 24;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st24;
            tr100 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 24;
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
                            cs = 24;
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
                goto st24;
            tr111 :
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
                            cs = 24;
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
                goto st24;
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
                            cs = 24;
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
                goto st24;
            tr130 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 24;
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
                            cs = 24;
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
                goto st24;
            tr137 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 24;
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
                            cs = 24;
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
                goto st24;
            tr155 :
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
                            cs = 24;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st24;
            tr169 :
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
                            cs = 24;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st24;
            tr178 :
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
                            cs = 24;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st24;
            tr189 :
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
                            cs = 24;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st24;
            st24:
                if (++p == pe) goto _test_eof24;
            case 24:
#line 1716 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st24;
                    case 43u:
                        goto tr63;
                    case 45u:
                        goto tr63;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr64;
                } else if ((*p) >= 9u)
                    goto st24;
                goto st0;
            tr63 :
#line 13 "ragel/./text-grammar.rl"
            {
                intb.begin(p);
            }
                goto st25;
            st25:
                if (++p == pe) goto _test_eof25;
            case 25:
#line 1736 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st26;
                goto st0;
            tr64 :
#line 13 "ragel/./text-grammar.rl"
            {
                intb.begin(p);
            }
                goto st26;
            st26:
                if (++p == pe) goto _test_eof26;
            case 26:
#line 1748 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr46;
                    case 33u:
                        goto tr47;
                    case 39u:
                        goto tr48;
                    case 40u:
                        goto tr49;
                    case 44u:
                        goto tr47;
                    case 59u:
                        goto tr47;
                    case 61u:
                        goto tr51;
                    case 62u:
                        goto tr52;
                    case 63u:
                        goto tr47;
                    case 94u:
                        goto tr54;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st26;
                } else if ((*p) >= 9u)
                    goto tr46;
                goto st0;
            tr52 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 27;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st27;
            tr61 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 27;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
                goto st27;
            tr76 :
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
                            cs = 27;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st27;
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
                            cs = 27;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st27;
            tr101 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 27;
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
                            cs = 27;
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
                goto st27;
            tr112 :
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
                            cs = 27;
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
                goto st27;
            tr120 :
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
                            cs = 27;
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
                goto st27;
            tr131 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 27;
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
                            cs = 27;
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
                goto st27;
            tr138 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 27;
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
                            cs = 27;
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
                goto st27;
            tr156 :
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
                            cs = 27;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st27;
            tr170 :
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
                            cs = 27;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st27;
            tr179 :
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
                            cs = 27;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st27;
            tr190 :
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
                            cs = 27;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st27;
            st27:
                if (++p == pe) goto _test_eof27;
            case 27:
#line 1988 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st27;
                    case 95u:
                        goto tr67;
                    case 126u:
                        goto tr67;
                }
                if ((*p) < 65u) {
                    if ((*p) > 13u) {
                        if (48u <= (*p) && (*p) <= 57u) goto tr66;
                    } else if ((*p) >= 9u)
                        goto st27;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr67;
                    } else if ((*p) >= 71u)
                        goto tr67;
                } else
                    goto tr66;
                goto st0;
            tr66 :
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
            st28:
                if (++p == pe) goto _test_eof28;
            case 28:
#line 2024 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr68;
                    case 33u:
                        goto tr69;
                    case 39u:
                        goto tr71;
                    case 40u:
                        goto tr72;
                    case 44u:
                        goto tr69;
                    case 47u:
                        goto tr73;
                    case 59u:
                        goto tr69;
                    case 61u:
                        goto tr75;
                    case 62u:
                        goto tr76;
                    case 63u:
                        goto tr69;
                    case 94u:
                        goto tr77;
                    case 95u:
                        goto st38;
                    case 126u:
                        goto st38;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr70;
                    } else if ((*p) >= 9u)
                        goto tr68;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st38;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st38;
                    } else
                        goto st38;
                } else
                    goto tr70;
                goto st0;
            tr70 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st29;
            st29:
                if (++p == pe) goto _test_eof29;
            case 29:
#line 2068 "ron/text-parser.cc"
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
                goto st30;
            st30:
                if (++p == pe) goto _test_eof30;
            case 30:
#line 2090 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr79;
                    case 33u:
                        goto tr80;
                    case 39u:
                        goto tr81;
                    case 40u:
                        goto tr82;
                    case 44u:
                        goto tr80;
                    case 59u:
                        goto tr80;
                    case 61u:
                        goto tr84;
                    case 62u:
                        goto tr85;
                    case 63u:
                        goto tr80;
                    case 94u:
                        goto tr86;
                    case 95u:
                        goto st30;
                    case 126u:
                        goto st30;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 13u) goto tr79;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st30;
                    } else if ((*p) >= 65u)
                        goto st30;
                } else
                    goto st30;
                goto st0;
            tr54 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 31;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Integer(parse_int(intb), body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st31;
            tr62 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 31;
                        goto _out;
                    }
                }
                op_.AddAtom(
                    Atom::Float(parse_float(floatb), body.range_of(floatb)));
            }
                goto st31;
            tr77 :
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
                            cs = 31;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st31;
            tr86 :
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
                            cs = 31;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 31 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st31;
            tr104 :
#line 14 "ragel/./text-grammar.rl"
            {
                intb.end(p);
                if (intb.size() > 21) {
                    cs = 0;
                    {
                        p++;
                        cs = 31;
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
                            cs = 31;
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
                goto st31;
            tr113 :
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
                            cs = 31;
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
                goto st31;
            tr121 :
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
                            cs = 31;
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
                goto st31;
            tr132 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 31;
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
                            cs = 31;
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
                goto st31;
            tr139 :
#line 26 "ragel/./text-grammar.rl"
            {
                floatb.end(p);
                if (floatb.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 31;
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
                            cs = 31;
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
                goto st31;
            tr157 :
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
                            cs = 31;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st31;
            tr171 :
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
                            cs = 31;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st31;
            tr180 :
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
                            cs = 31;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 10 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st31;
            tr191 :
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
                            cs = 31;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st31;
            st31:
                if (++p == pe) goto _test_eof31;
            case 31:
#line 2338 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st31;
                    case 43u:
                        goto tr87;
                    case 45u:
                        goto tr87;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr88;
                } else if ((*p) >= 9u)
                    goto st31;
                goto st0;
            tr87 :
#line 25 "ragel/./text-grammar.rl"
            {
                floatb.begin(p);
            }
                goto st32;
            st32:
                if (++p == pe) goto _test_eof32;
            case 32:
#line 2358 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st33;
                goto st0;
            tr88 :
#line 25 "ragel/./text-grammar.rl"
            {
                floatb.begin(p);
            }
                goto st33;
            st33:
                if (++p == pe) goto _test_eof33;
            case 33:
#line 2370 "ron/text-parser.cc"
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
                    case 32u:
                        goto tr56;
                    case 33u:
                        goto tr57;
                    case 39u:
                        goto tr58;
                    case 40u:
                        goto tr59;
                    case 44u:
                        goto tr57;
                    case 59u:
                        goto tr57;
                    case 61u:
                        goto tr60;
                    case 62u:
                        goto tr61;
                    case 63u:
                        goto tr57;
                    case 94u:
                        goto tr62;
                }
                if ((*p) > 13u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st36;
                } else if ((*p) >= 9u)
                    goto tr56;
                goto st0;
            tr73 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st37;
            st37:
                if (++p == pe) goto _test_eof37;
            case 37:
#line 2427 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr92;
                    case 126u:
                        goto tr92;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr92;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr92;
                } else
                    goto tr92;
                goto st0;
            tr67 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st38;
            tr92 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st38;
            st38:
                if (++p == pe) goto _test_eof38;
            case 38:
#line 2460 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr68;
                    case 33u:
                        goto tr69;
                    case 39u:
                        goto tr71;
                    case 40u:
                        goto tr72;
                    case 44u:
                        goto tr69;
                    case 59u:
                        goto tr69;
                    case 61u:
                        goto tr75;
                    case 62u:
                        goto tr76;
                    case 63u:
                        goto tr69;
                    case 94u:
                        goto tr77;
                    case 95u:
                        goto st38;
                    case 126u:
                        goto st38;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr70;
                    } else if ((*p) >= 9u)
                        goto tr68;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st38;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st38;
                    } else
                        goto st38;
                } else
                    goto tr70;
                goto st0;
            tr21 :
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
                goto st39;
            st39:
                if (++p == pe) goto _test_eof39;
            case 39:
#line 2512 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr93;
                    case 33u:
                        goto tr94;
                    case 39u:
                        goto tr96;
                    case 40u:
                        goto tr97;
                    case 44u:
                        goto tr94;
                    case 46u:
                        goto st22;
                    case 47u:
                        goto tr98;
                    case 59u:
                        goto tr94;
                    case 61u:
                        goto tr100;
                    case 62u:
                        goto tr101;
                    case 63u:
                        goto tr94;
                    case 69u:
                        goto st45;
                    case 94u:
                        goto tr104;
                    case 95u:
                        goto st43;
                    case 101u:
                        goto st45;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr95;
                    } else if ((*p) >= 9u)
                        goto tr93;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st44;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr95;
                goto st0;
            tr95 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st40;
            st40:
                if (++p == pe) goto _test_eof40;
            case 40:
#line 2559 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr105;
                    case 126u:
                        goto tr105;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr105;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr105;
                } else
                    goto tr105;
                goto st0;
            tr105 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st41;
            st41:
                if (++p == pe) goto _test_eof41;
            case 41:
#line 2581 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr106;
                    case 33u:
                        goto tr107;
                    case 39u:
                        goto tr108;
                    case 40u:
                        goto tr109;
                    case 44u:
                        goto tr107;
                    case 59u:
                        goto tr107;
                    case 61u:
                        goto tr111;
                    case 62u:
                        goto tr112;
                    case 63u:
                        goto tr107;
                    case 94u:
                        goto tr113;
                    case 95u:
                        goto st41;
                    case 126u:
                        goto st41;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 13u) goto tr106;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st41;
                    } else if ((*p) >= 65u)
                        goto st41;
                } else
                    goto st41;
                goto st0;
            tr98 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st42;
            st42:
                if (++p == pe) goto _test_eof42;
            case 42:
#line 2616 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr114;
                    case 126u:
                        goto tr114;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr114;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr114;
                } else
                    goto tr114;
                goto st0;
            tr23 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st43;
            tr114 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st43;
            st43:
                if (++p == pe) goto _test_eof43;
            case 43:
#line 2649 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr115;
                    case 33u:
                        goto tr116;
                    case 39u:
                        goto tr117;
                    case 40u:
                        goto tr118;
                    case 44u:
                        goto tr116;
                    case 59u:
                        goto tr116;
                    case 61u:
                        goto tr119;
                    case 62u:
                        goto tr120;
                    case 63u:
                        goto tr116;
                    case 94u:
                        goto tr121;
                    case 95u:
                        goto st43;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr95;
                    } else if ((*p) >= 9u)
                        goto tr115;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st43;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr95;
                goto st0;
            st44:
                if (++p == pe) goto _test_eof44;
            case 44:
                switch ((*p)) {
                    case 32u:
                        goto tr93;
                    case 33u:
                        goto tr94;
                    case 39u:
                        goto tr96;
                    case 40u:
                        goto tr97;
                    case 44u:
                        goto tr94;
                    case 46u:
                        goto st22;
                    case 59u:
                        goto tr94;
                    case 61u:
                        goto tr100;
                    case 62u:
                        goto tr101;
                    case 63u:
                        goto tr94;
                    case 69u:
                        goto st45;
                    case 94u:
                        goto tr104;
                    case 95u:
                        goto st43;
                    case 101u:
                        goto st45;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr95;
                    } else if ((*p) >= 9u)
                        goto tr93;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st44;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr95;
                goto st0;
            st45:
                if (++p == pe) goto _test_eof45;
            case 45:
                switch ((*p)) {
                    case 32u:
                        goto tr115;
                    case 33u:
                        goto tr116;
                    case 39u:
                        goto tr117;
                    case 40u:
                        goto tr118;
                    case 44u:
                        goto tr116;
                    case 59u:
                        goto tr116;
                    case 61u:
                        goto tr119;
                    case 62u:
                        goto tr120;
                    case 63u:
                        goto tr116;
                    case 94u:
                        goto tr121;
                    case 95u:
                        goto st43;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr95;
                    } else if ((*p) >= 9u)
                        goto tr115;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st48;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr122;
                goto st0;
            tr122 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st46;
            st46:
                if (++p == pe) goto _test_eof46;
            case 46:
#line 2767 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr105;
                    case 126u:
                        goto tr105;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr124;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr105;
                } else
                    goto tr105;
                goto st0;
            tr124 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st47;
            st47:
                if (++p == pe) goto _test_eof47;
            case 47:
#line 2789 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr125;
                    case 33u:
                        goto tr126;
                    case 39u:
                        goto tr127;
                    case 40u:
                        goto tr128;
                    case 44u:
                        goto tr126;
                    case 59u:
                        goto tr126;
                    case 61u:
                        goto tr130;
                    case 62u:
                        goto tr131;
                    case 63u:
                        goto tr126;
                    case 94u:
                        goto tr132;
                    case 95u:
                        goto st41;
                    case 126u:
                        goto st41;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 13u) goto tr125;
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
                    case 32u:
                        goto tr133;
                    case 33u:
                        goto tr134;
                    case 39u:
                        goto tr135;
                    case 40u:
                        goto tr136;
                    case 44u:
                        goto tr134;
                    case 59u:
                        goto tr134;
                    case 61u:
                        goto tr137;
                    case 62u:
                        goto tr138;
                    case 63u:
                        goto tr134;
                    case 94u:
                        goto tr139;
                    case 95u:
                        goto st43;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr95;
                    } else if ((*p) >= 9u)
                        goto tr133;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st48;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr95;
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
                goto st49;
            st49:
                if (++p == pe) goto _test_eof49;
            case 49:
#line 2867 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr115;
                    case 33u:
                        goto tr116;
                    case 39u:
                        goto tr117;
                    case 40u:
                        goto tr118;
                    case 44u:
                        goto tr116;
                    case 47u:
                        goto tr98;
                    case 59u:
                        goto tr116;
                    case 61u:
                        goto tr119;
                    case 62u:
                        goto tr120;
                    case 63u:
                        goto tr116;
                    case 94u:
                        goto tr121;
                    case 95u:
                        goto st43;
                    case 126u:
                        goto st43;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr95;
                    } else if ((*p) >= 9u)
                        goto tr115;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st43;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st43;
                    } else
                        goto st43;
                } else
                    goto tr95;
                goto st0;
            tr3 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st50;
            st50:
                if (++p == pe) goto _test_eof50;
            case 50:
#line 2909 "ron/text-parser.cc"
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
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st55;
            st55:
                if (++p == pe) goto _test_eof55;
            case 55:
#line 2982 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st2;
                goto st0;
            tr5 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st56;
            st56:
                if (++p == pe) goto _test_eof56;
            case 56:
#line 2994 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto st55;
                goto st0;
            tr6 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st57;
            st57:
                if (++p == pe) goto _test_eof57;
            case 57:
#line 3006 "ron/text-parser.cc"
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
                        goto tr146;
                    case 126u:
                        goto tr146;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr145;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr146;
                    } else if ((*p) >= 71u)
                        goto tr146;
                } else
                    goto tr145;
                goto st0;
            tr145 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st60;
            st60:
                if (++p == pe) goto _test_eof60;
            case 60:
#line 3057 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr147;
                    case 33u:
                        goto tr148;
                    case 39u:
                        goto tr150;
                    case 40u:
                        goto tr151;
                    case 44u:
                        goto tr148;
                    case 47u:
                        goto tr152;
                    case 58u:
                        goto tr154;
                    case 59u:
                        goto tr148;
                    case 61u:
                        goto tr155;
                    case 62u:
                        goto tr156;
                    case 63u:
                        goto tr148;
                    case 94u:
                        goto tr157;
                    case 95u:
                        goto st71;
                    case 126u:
                        goto st71;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr149;
                    } else if ((*p) >= 9u)
                        goto tr147;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st71;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st71;
                    } else
                        goto st71;
                } else
                    goto tr149;
                goto st0;
            tr147 :
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
                            cs = 61;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st61;
            tr183 :
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
                            cs = 61;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st61;
            st61:
                if (++p == pe) goto _test_eof61;
            case 61:
#line 3128 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto st61;
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
                    if ((*p) > 13u) {
                        if (43u <= (*p) && (*p) <= 45u) goto tr20;
                    } else if ((*p) >= 9u)
                        goto st61;
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
            tr154 :
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
                            cs = 62;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st62;
            tr188 :
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
                            cs = 62;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 7 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st62;
            st62:
                if (++p == pe) goto _test_eof62;
            case 62:
#line 3198 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr161;
                    case 126u:
                        goto tr161;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr160;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr161;
                    } else if ((*p) >= 71u)
                        goto tr161;
                } else
                    goto tr160;
                goto st0;
            tr160 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st63;
            st63:
                if (++p == pe) goto _test_eof63;
            case 63:
#line 3230 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr162;
                    case 33u:
                        goto tr163;
                    case 39u:
                        goto tr165;
                    case 40u:
                        goto tr166;
                    case 44u:
                        goto tr163;
                    case 47u:
                        goto tr167;
                    case 59u:
                        goto tr163;
                    case 61u:
                        goto tr169;
                    case 62u:
                        goto tr170;
                    case 63u:
                        goto tr163;
                    case 94u:
                        goto tr171;
                    case 95u:
                        goto st67;
                    case 126u:
                        goto st67;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr164;
                    } else if ((*p) >= 9u)
                        goto tr162;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st67;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st67;
                    } else
                        goto st67;
                } else
                    goto tr164;
                goto st0;
            tr164 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st64;
            st64:
                if (++p == pe) goto _test_eof64;
            case 64:
#line 3274 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr172;
                    case 126u:
                        goto tr172;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr172;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr172;
                } else
                    goto tr172;
                goto st0;
            tr172 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st65;
            st65:
                if (++p == pe) goto _test_eof65;
            case 65:
#line 3296 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr173;
                    case 33u:
                        goto tr174;
                    case 39u:
                        goto tr175;
                    case 40u:
                        goto tr176;
                    case 44u:
                        goto tr174;
                    case 59u:
                        goto tr174;
                    case 61u:
                        goto tr178;
                    case 62u:
                        goto tr179;
                    case 63u:
                        goto tr174;
                    case 94u:
                        goto tr180;
                    case 95u:
                        goto st65;
                    case 126u:
                        goto st65;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 13u) goto tr173;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st65;
                    } else if ((*p) >= 65u)
                        goto st65;
                } else
                    goto st65;
                goto st0;
            tr167 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st66;
            st66:
                if (++p == pe) goto _test_eof66;
            case 66:
#line 3331 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr181;
                    case 126u:
                        goto tr181;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr181;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr181;
                } else
                    goto tr181;
                goto st0;
            tr161 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st67;
            tr181 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st67;
            st67:
                if (++p == pe) goto _test_eof67;
            case 67:
#line 3364 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr162;
                    case 33u:
                        goto tr163;
                    case 39u:
                        goto tr165;
                    case 40u:
                        goto tr166;
                    case 44u:
                        goto tr163;
                    case 59u:
                        goto tr163;
                    case 61u:
                        goto tr169;
                    case 62u:
                        goto tr170;
                    case 63u:
                        goto tr163;
                    case 94u:
                        goto tr171;
                    case 95u:
                        goto st67;
                    case 126u:
                        goto st67;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr164;
                    } else if ((*p) >= 9u)
                        goto tr162;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st67;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st67;
                    } else
                        goto st67;
                } else
                    goto tr164;
                goto st0;
            tr149 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st68;
            st68:
                if (++p == pe) goto _test_eof68;
            case 68:
#line 3407 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr182;
                    case 126u:
                        goto tr182;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr182;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr182;
                } else
                    goto tr182;
                goto st0;
            tr182 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st69;
            st69:
                if (++p == pe) goto _test_eof69;
            case 69:
#line 3429 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr183;
                    case 33u:
                        goto tr184;
                    case 39u:
                        goto tr185;
                    case 40u:
                        goto tr186;
                    case 44u:
                        goto tr184;
                    case 58u:
                        goto tr188;
                    case 59u:
                        goto tr184;
                    case 61u:
                        goto tr189;
                    case 62u:
                        goto tr190;
                    case 63u:
                        goto tr184;
                    case 94u:
                        goto tr191;
                    case 95u:
                        goto st69;
                    case 126u:
                        goto st69;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 13u) goto tr183;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st69;
                    } else if ((*p) >= 65u)
                        goto st69;
                } else
                    goto st69;
                goto st0;
            tr152 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st70;
            st70:
                if (++p == pe) goto _test_eof70;
            case 70:
#line 3465 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr192;
                    case 126u:
                        goto tr192;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr192;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr192;
                } else
                    goto tr192;
                goto st0;
            tr146 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st71;
            tr192 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st71;
            st71:
                if (++p == pe) goto _test_eof71;
            case 71:
#line 3498 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32u:
                        goto tr147;
                    case 33u:
                        goto tr148;
                    case 39u:
                        goto tr150;
                    case 40u:
                        goto tr151;
                    case 44u:
                        goto tr148;
                    case 58u:
                        goto tr154;
                    case 59u:
                        goto tr148;
                    case 61u:
                        goto tr155;
                    case 62u:
                        goto tr156;
                    case 63u:
                        goto tr148;
                    case 94u:
                        goto tr157;
                    case 95u:
                        goto st71;
                    case 126u:
                        goto st71;
                }
                if ((*p) < 43u) {
                    if ((*p) > 13u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr149;
                    } else if ((*p) >= 9u)
                        goto tr147;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st71;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st71;
                    } else
                        goto st71;
                } else
                    goto tr149;
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

    off_ = p - pb;

    if (op_.size()) prev_id_ = op_.id();

    // std::cerr << "ending with [" <<p<<"] state "<<cs<<" "<<op_.size()<<"
    // atoms "<<(pe-p)<<" bytes left, prev_id_ "<<prev_id_.str()<<'\n';

    if (term && cs != RON_error) {
        op_.term_ =
            TERM(strchr(TERM_PUNCT, term) - TERM_PUNCT);  // FIXME gen a fn
        return Status::OK;
    } else {
        cs = RON_error;
        return Status::BAD_STATE;
    }
}

}  // namespace ron
