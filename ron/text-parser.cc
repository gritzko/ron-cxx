
#line 1 "ragel/text-parser.rl"
#include <iostream>
#include "text.hpp"

namespace ron {

#line 7 "ragel/text-parser.rl"

#line 12 "ron/text-parser.cc"
static const int RON_start = 54;
static const int RON_first_final = 54;
static const int RON_error = 0;

static const int RON_en_main = 54;

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

    slice_t body{frame_.data()};
    iter pb = data().data();
    iter p = pb + off_;
    iter pe = pb + data().size();
    iter eof = pe;

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
        tr8 :
#line 37 "ragel/./text-grammar.rl"
        {
            term = (*p);
            pos_++;
            if (p < pe - 1) {
                p++;
                cs = 54;
                goto _out;
            }
        }
            goto st54;
        tr19 :
#line 13 "ragel/./text-grammar.rl"
        {
            intb.end(p);
            if (intb.size() > 21) {
                cs = 0;
                {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            op_.AddAtom(Atom::Integer(body.range_of(intb)));
            lastintb = intb.buf_;
        }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
        tr28 :
#line 25 "ragel/./text-grammar.rl"
        {
            floatb.end(p);
            if (floatb.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            op_.AddAtom(Atom::Float(body.range_of(floatb)));
        }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
        tr39 :
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
                        cs = 54;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 30 "ragel/./text-grammar.rl"
            { op_.AddAtom(Uuid{variety, value, version, origin}); }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
        tr49 :
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
                        cs = 54;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 30 "ragel/./text-grammar.rl"
            { op_.AddAtom(Uuid{variety, value, version, origin}); }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
        tr62 :
#line 13 "ragel/./text-grammar.rl"
        {
            intb.end(p);
            if (intb.size() > 21) {
                cs = 0;
                {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            op_.AddAtom(Atom::Integer(body.range_of(intb)));
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
                        cs = 54;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 33 "ragel/./text-grammar.rl"
            {
                if (!intb.same(uuidb))
                    op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
        tr74 :
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
                        cs = 54;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 33 "ragel/./text-grammar.rl"
            {
                if (!intb.same(uuidb))
                    op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
        tr82 :
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
                        cs = 54;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 33 "ragel/./text-grammar.rl"
            {
                if (!intb.same(uuidb))
                    op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
        tr91 :
#line 25 "ragel/./text-grammar.rl"
        {
            floatb.end(p);
            if (floatb.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
                        cs = 54;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 33 "ragel/./text-grammar.rl"
            {
                if (!intb.same(uuidb))
                    op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
        tr98 :
#line 25 "ragel/./text-grammar.rl"
        {
            floatb.end(p);
            if (floatb.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
                        cs = 54;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 33 "ragel/./text-grammar.rl"
            {
                if (!intb.same(uuidb))
                    op_.AddAtom(Uuid{variety, value, version, origin});
            }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
        tr111 :
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
                        cs = 54;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 6 "ragel/./text-grammar.rl"
            { op_.SetId(Uuid{variety, value, version, origin}); }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
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
                        cs = 54;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 9 "ragel/./text-grammar.rl"
            { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
        tr135 :
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
                        cs = 54;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 9 "ragel/./text-grammar.rl"
            { op_.SetRef(Uuid{variety, value, version, origin}); }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
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
                        cs = 54;
                        goto _out;
                    }
                }
                uuidb.end(p);
            }
#line 6 "ragel/./text-grammar.rl"
            { op_.SetId(Uuid{variety, value, version, origin}); }
#line 37 "ragel/./text-grammar.rl"
            {
                term = (*p);
                pos_++;
                if (p < pe - 1) {
                    p++;
                    cs = 54;
                    goto _out;
                }
            }
            goto st54;
        st54:
            if (++p == pe) goto _test_eof54;
            case 54:
#line 392 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto st54;
                    case 33:
                        goto tr8;
                    case 39:
                        goto st1;
                    case 44:
                        goto tr8;
                    case 46:
                        goto st40;
                    case 59:
                        goto tr8;
                    case 61:
                        goto st9;
                    case 62:
                        goto st12;
                    case 63:
                        goto tr8;
                    case 64:
                        goto st41;
                    case 94:
                        goto st16;
                    case 95:
                        goto tr16;
                    case 126:
                        goto tr16;
                }
                if ((*p) < 48) {
                    if ((*p) > 13) {
                        if (43 <= (*p) && (*p) <= 45) goto tr13;
                    } else if ((*p) >= 9)
                        goto st54;
                } else if ((*p) > 57) {
                    if ((*p) < 71) {
                        if (65 <= (*p) && (*p) <= 70) goto tr15;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto tr16;
                    } else
                        goto tr16;
                } else
                    goto tr14;
                goto st0;
            st0:
                cs = 0;
                goto _out;
            tr20 :
#line 13 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Integer(body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st1;
            tr29 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
            }
                goto st1;
            tr41 :
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
#line 30 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st1;
            tr50 :
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
#line 30 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st1;
            tr64 :
#line 13 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Integer(body.range_of(intb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr75 :
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr83 :
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr92 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr99 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr113 :
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
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st1;
            tr127 :
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
#line 9 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st1;
            tr136 :
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
#line 9 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st1;
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
                            cs = 1;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st1;
            st1:
                if (++p == pe) goto _test_eof1;
            case 1:
#line 650 "ron/text-parser.cc"
                switch ((*p)) {
                    case 10:
                        goto st0;
                    case 13:
                        goto st0;
                    case 39:
                        goto tr2;
                    case 92:
                        goto tr3;
                }
                goto tr0;
            tr0 :
#line 19 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st2;
            st2:
                if (++p == pe) goto _test_eof2;
            case 2:
#line 666 "ron/text-parser.cc"
                switch ((*p)) {
                    case 10:
                        goto st0;
                    case 13:
                        goto st0;
                    case 39:
                        goto tr5;
                    case 92:
                        goto st35;
                }
                goto st2;
            tr2 :
#line 19 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
#line 20 "ragel/./text-grammar.rl"
                {
                    strb.end(p);
                    op_.AddAtom(Atom::String(body.range_of(strb)));
                }
                goto st3;
            tr5 :
#line 20 "ragel/./text-grammar.rl"
            {
                strb.end(p);
                op_.AddAtom(Atom::String(body.range_of(strb)));
            }
                goto st3;
            st3:
                if (++p == pe) goto _test_eof3;
            case 3:
#line 694 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto st4;
                    case 33:
                        goto tr8;
                    case 39:
                        goto st1;
                    case 44:
                        goto tr8;
                    case 59:
                        goto tr8;
                    case 61:
                        goto st9;
                    case 62:
                        goto st12;
                    case 63:
                        goto tr8;
                    case 94:
                        goto st16;
                }
                if (9 <= (*p) && (*p) <= 13) goto st4;
                goto st0;
            tr18 :
#line 13 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Integer(body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st4;
            tr27 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
            }
                goto st4;
            tr38 :
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
#line 30 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st4;
            tr48 :
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
#line 30 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st4;
            tr61 :
#line 13 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Integer(body.range_of(intb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st4;
            tr73 :
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st4;
            tr81 :
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st4;
            tr90 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st4;
            tr97 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st4;
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
                            cs = 4;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 9 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st4;
            tr134 :
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
#line 9 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st4;
            st4:
                if (++p == pe) goto _test_eof4;
            case 4:
#line 898 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto st4;
                    case 33:
                        goto tr8;
                    case 39:
                        goto st1;
                    case 44:
                        goto tr8;
                    case 59:
                        goto tr8;
                    case 61:
                        goto st9;
                    case 62:
                        goto st12;
                    case 63:
                        goto tr8;
                    case 94:
                        goto st16;
                    case 95:
                        goto tr16;
                    case 126:
                        goto tr16;
                }
                if ((*p) < 48) {
                    if ((*p) > 13) {
                        if (43 <= (*p) && (*p) <= 45) goto tr13;
                    } else if ((*p) >= 9)
                        goto st4;
                } else if ((*p) > 57) {
                    if ((*p) < 71) {
                        if (65 <= (*p) && (*p) <= 70) goto tr15;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto tr16;
                    } else
                        goto tr16;
                } else
                    goto tr14;
                goto st0;
            tr13 :
#line 12 "ragel/./text-grammar.rl"
            {
                intb.begin(p);
            }
#line 24 "ragel/./text-grammar.rl"
                { floatb.begin(p); }
                goto st5;
            st5:
                if (++p == pe) goto _test_eof5;
            case 5:
#line 940 "ron/text-parser.cc"
                if (48 <= (*p) && (*p) <= 57) goto st6;
                goto st0;
            st6:
                if (++p == pe) goto _test_eof6;
            case 6:
                switch ((*p)) {
                    case 32:
                        goto tr18;
                    case 33:
                        goto tr19;
                    case 39:
                        goto tr20;
                    case 44:
                        goto tr19;
                    case 46:
                        goto st7;
                    case 59:
                        goto tr19;
                    case 61:
                        goto tr22;
                    case 62:
                        goto tr23;
                    case 63:
                        goto tr19;
                    case 69:
                        goto st19;
                    case 94:
                        goto tr25;
                    case 101:
                        goto st19;
                }
                if ((*p) > 13) {
                    if (48 <= (*p) && (*p) <= 57) goto st6;
                } else if ((*p) >= 9)
                    goto tr18;
                goto st0;
            st7:
                if (++p == pe) goto _test_eof7;
            case 7:
                if (48 <= (*p) && (*p) <= 57) goto st8;
                goto st0;
            st8:
                if (++p == pe) goto _test_eof8;
            case 8:
                switch ((*p)) {
                    case 32:
                        goto tr27;
                    case 33:
                        goto tr28;
                    case 39:
                        goto tr29;
                    case 44:
                        goto tr28;
                    case 59:
                        goto tr28;
                    case 61:
                        goto tr30;
                    case 62:
                        goto tr31;
                    case 63:
                        goto tr28;
                    case 69:
                        goto st19;
                    case 94:
                        goto tr32;
                    case 101:
                        goto st19;
                }
                if ((*p) > 13) {
                    if (48 <= (*p) && (*p) <= 57) goto st8;
                } else if ((*p) >= 9)
                    goto tr27;
                goto st0;
            tr22 :
#line 13 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Integer(body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st9;
            tr30 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
            }
                goto st9;
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
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 30 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st9;
            tr52 :
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
#line 30 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st9;
            tr67 :
#line 13 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Integer(body.range_of(intb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st9;
            tr77 :
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st9;
            tr84 :
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st9;
            tr94 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st9;
            tr100 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st9;
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
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st9;
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
                            cs = 9;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 9 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st9;
            tr138 :
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
#line 9 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st9;
            tr148 :
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
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st9;
            st9:
                if (++p == pe) goto _test_eof9;
            case 9:
#line 1219 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto st9;
                    case 43:
                        goto tr33;
                    case 45:
                        goto tr33;
                }
                if ((*p) > 13) {
                    if (48 <= (*p) && (*p) <= 57) goto tr34;
                } else if ((*p) >= 9)
                    goto st9;
                goto st0;
            tr33 :
#line 12 "ragel/./text-grammar.rl"
            {
                intb.begin(p);
            }
                goto st10;
            st10:
                if (++p == pe) goto _test_eof10;
            case 10:
#line 1239 "ron/text-parser.cc"
                if (48 <= (*p) && (*p) <= 57) goto st11;
                goto st0;
            tr34 :
#line 12 "ragel/./text-grammar.rl"
            {
                intb.begin(p);
            }
                goto st11;
            st11:
                if (++p == pe) goto _test_eof11;
            case 11:
#line 1251 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr18;
                    case 33:
                        goto tr19;
                    case 39:
                        goto tr20;
                    case 44:
                        goto tr19;
                    case 59:
                        goto tr19;
                    case 61:
                        goto tr22;
                    case 62:
                        goto tr23;
                    case 63:
                        goto tr19;
                    case 94:
                        goto tr25;
                }
                if ((*p) > 13) {
                    if (48 <= (*p) && (*p) <= 57) goto st11;
                } else if ((*p) >= 9)
                    goto tr18;
                goto st0;
            tr23 :
#line 13 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Integer(body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st12;
            tr31 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
            }
                goto st12;
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
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 30 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st12;
            tr53 :
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
#line 30 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st12;
            tr68 :
#line 13 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Integer(body.range_of(intb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st12;
            tr78 :
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st12;
            tr85 :
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st12;
            tr95 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st12;
            tr101 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st12;
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
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st12;
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
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 9 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st12;
            tr139 :
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
#line 9 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st12;
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
                            cs = 12;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st12;
            st12:
                if (++p == pe) goto _test_eof12;
            case 12:
#line 1490 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto st12;
                    case 95:
                        goto tr37;
                    case 126:
                        goto tr37;
                }
                if ((*p) < 65) {
                    if ((*p) > 13) {
                        if (48 <= (*p) && (*p) <= 57) goto tr36;
                    } else if ((*p) >= 9)
                        goto st12;
                } else if ((*p) > 70) {
                    if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto tr37;
                    } else if ((*p) >= 71)
                        goto tr37;
                } else
                    goto tr36;
                goto st0;
            tr36 :
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
#line 1526 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr38;
                    case 33:
                        goto tr39;
                    case 39:
                        goto tr41;
                    case 44:
                        goto tr39;
                    case 47:
                        goto tr42;
                    case 59:
                        goto tr39;
                    case 61:
                        goto tr44;
                    case 62:
                        goto tr45;
                    case 63:
                        goto tr39;
                    case 94:
                        goto tr46;
                    case 95:
                        goto st23;
                    case 126:
                        goto st23;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr40;
                    } else if ((*p) >= 9)
                        goto tr38;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st23;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st23;
                    } else
                        goto st23;
                } else
                    goto tr40;
                goto st0;
            tr40 :
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
#line 1569 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr47;
                    case 126:
                        goto tr47;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr47;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto tr47;
                } else
                    goto tr47;
                goto st0;
            tr47 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st15;
            st15:
                if (++p == pe) goto _test_eof15;
            case 15:
#line 1591 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr48;
                    case 33:
                        goto tr49;
                    case 39:
                        goto tr50;
                    case 44:
                        goto tr49;
                    case 59:
                        goto tr49;
                    case 61:
                        goto tr52;
                    case 62:
                        goto tr53;
                    case 63:
                        goto tr49;
                    case 94:
                        goto tr54;
                    case 95:
                        goto st15;
                    case 126:
                        goto st15;
                }
                if ((*p) < 48) {
                    if (9 <= (*p) && (*p) <= 13) goto tr48;
                } else if ((*p) > 57) {
                    if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st15;
                    } else if ((*p) >= 65)
                        goto st15;
                } else
                    goto st15;
                goto st0;
            tr25 :
#line 13 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Integer(body.range_of(intb)));
                lastintb = intb.buf_;
            }
                goto st16;
            tr32 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
            }
                goto st16;
            tr46 :
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
#line 30 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st16;
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
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 30 "ragel/./text-grammar.rl"
                { op_.AddAtom(Uuid{variety, value, version, origin}); }
                goto st16;
            tr71 :
#line 13 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Integer(body.range_of(intb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st16;
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
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st16;
            tr86 :
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st16;
            tr96 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st16;
            tr102 :
#line 25 "ragel/./text-grammar.rl"
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
                op_.AddAtom(Atom::Float(body.range_of(floatb)));
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
#line 33 "ragel/./text-grammar.rl"
                {
                    if (!intb.same(uuidb))
                        op_.AddAtom(Uuid{variety, value, version, origin});
                }
                goto st16;
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
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st16;
            tr132 :
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
#line 9 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st16;
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
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 9 "ragel/./text-grammar.rl"
                { op_.SetRef(Uuid{variety, value, version, origin}); }
                goto st16;
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
                            cs = 16;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st16;
            st16:
                if (++p == pe) goto _test_eof16;
            case 16:
#line 1838 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto st16;
                    case 43:
                        goto tr55;
                    case 45:
                        goto tr55;
                }
                if ((*p) > 13) {
                    if (48 <= (*p) && (*p) <= 57) goto tr56;
                } else if ((*p) >= 9)
                    goto st16;
                goto st0;
            tr55 :
#line 24 "ragel/./text-grammar.rl"
            {
                floatb.begin(p);
            }
                goto st17;
            st17:
                if (++p == pe) goto _test_eof17;
            case 17:
#line 1858 "ron/text-parser.cc"
                if (48 <= (*p) && (*p) <= 57) goto st18;
                goto st0;
            tr56 :
#line 24 "ragel/./text-grammar.rl"
            {
                floatb.begin(p);
            }
                goto st18;
            st18:
                if (++p == pe) goto _test_eof18;
            case 18:
#line 1870 "ron/text-parser.cc"
                switch ((*p)) {
                    case 46:
                        goto st7;
                    case 69:
                        goto st19;
                    case 101:
                        goto st19;
                }
                if (48 <= (*p) && (*p) <= 57) goto st18;
                goto st0;
            st19:
                if (++p == pe) goto _test_eof19;
            case 19:
                switch ((*p)) {
                    case 43:
                        goto st20;
                    case 45:
                        goto st20;
                }
                if (48 <= (*p) && (*p) <= 57) goto st21;
                goto st0;
            st20:
                if (++p == pe) goto _test_eof20;
            case 20:
                if (48 <= (*p) && (*p) <= 57) goto st21;
                goto st0;
            st21:
                if (++p == pe) goto _test_eof21;
            case 21:
                switch ((*p)) {
                    case 32:
                        goto tr27;
                    case 33:
                        goto tr28;
                    case 39:
                        goto tr29;
                    case 44:
                        goto tr28;
                    case 59:
                        goto tr28;
                    case 61:
                        goto tr30;
                    case 62:
                        goto tr31;
                    case 63:
                        goto tr28;
                    case 94:
                        goto tr32;
                }
                if ((*p) > 13) {
                    if (48 <= (*p) && (*p) <= 57) goto st21;
                } else if ((*p) >= 9)
                    goto tr27;
                goto st0;
            tr42 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st22;
            st22:
                if (++p == pe) goto _test_eof22;
            case 22:
#line 1926 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr60;
                    case 126:
                        goto tr60;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr60;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto tr60;
                } else
                    goto tr60;
                goto st0;
            tr37 :
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
            tr60 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st23;
            st23:
                if (++p == pe) goto _test_eof23;
            case 23:
#line 1959 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr38;
                    case 33:
                        goto tr39;
                    case 39:
                        goto tr41;
                    case 44:
                        goto tr39;
                    case 59:
                        goto tr39;
                    case 61:
                        goto tr44;
                    case 62:
                        goto tr45;
                    case 63:
                        goto tr39;
                    case 94:
                        goto tr46;
                    case 95:
                        goto st23;
                    case 126:
                        goto st23;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr40;
                    } else if ((*p) >= 9)
                        goto tr38;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st23;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st23;
                    } else
                        goto st23;
                } else
                    goto tr40;
                goto st0;
            tr14 :
#line 12 "ragel/./text-grammar.rl"
            {
                intb.begin(p);
            }
#line 24 "ragel/./text-grammar.rl"
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
#line 2010 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr61;
                    case 33:
                        goto tr62;
                    case 39:
                        goto tr64;
                    case 44:
                        goto tr62;
                    case 46:
                        goto st7;
                    case 47:
                        goto tr65;
                    case 59:
                        goto tr62;
                    case 61:
                        goto tr67;
                    case 62:
                        goto tr68;
                    case 63:
                        goto tr62;
                    case 69:
                        goto st30;
                    case 94:
                        goto tr71;
                    case 95:
                        goto st28;
                    case 101:
                        goto st30;
                    case 126:
                        goto st28;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr63;
                    } else if ((*p) >= 9)
                        goto tr61;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st29;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr63;
                goto st0;
            tr63 :
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
#line 2056 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr72;
                    case 126:
                        goto tr72;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr72;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto tr72;
                } else
                    goto tr72;
                goto st0;
            tr72 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st26;
            st26:
                if (++p == pe) goto _test_eof26;
            case 26:
#line 2078 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr73;
                    case 33:
                        goto tr74;
                    case 39:
                        goto tr75;
                    case 44:
                        goto tr74;
                    case 59:
                        goto tr74;
                    case 61:
                        goto tr77;
                    case 62:
                        goto tr78;
                    case 63:
                        goto tr74;
                    case 94:
                        goto tr79;
                    case 95:
                        goto st26;
                    case 126:
                        goto st26;
                }
                if ((*p) < 48) {
                    if (9 <= (*p) && (*p) <= 13) goto tr73;
                } else if ((*p) > 57) {
                    if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st26;
                    } else if ((*p) >= 65)
                        goto st26;
                } else
                    goto st26;
                goto st0;
            tr65 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st27;
            st27:
                if (++p == pe) goto _test_eof27;
            case 27:
#line 2112 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr80;
                    case 126:
                        goto tr80;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr80;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto tr80;
                } else
                    goto tr80;
                goto st0;
            tr16 :
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
            tr80 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st28;
            st28:
                if (++p == pe) goto _test_eof28;
            case 28:
#line 2145 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr81;
                    case 33:
                        goto tr82;
                    case 39:
                        goto tr83;
                    case 44:
                        goto tr82;
                    case 59:
                        goto tr82;
                    case 61:
                        goto tr84;
                    case 62:
                        goto tr85;
                    case 63:
                        goto tr82;
                    case 94:
                        goto tr86;
                    case 95:
                        goto st28;
                    case 126:
                        goto st28;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr63;
                    } else if ((*p) >= 9)
                        goto tr81;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st28;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr63;
                goto st0;
            st29:
                if (++p == pe) goto _test_eof29;
            case 29:
                switch ((*p)) {
                    case 32:
                        goto tr61;
                    case 33:
                        goto tr62;
                    case 39:
                        goto tr64;
                    case 44:
                        goto tr62;
                    case 46:
                        goto st7;
                    case 59:
                        goto tr62;
                    case 61:
                        goto tr67;
                    case 62:
                        goto tr68;
                    case 63:
                        goto tr62;
                    case 69:
                        goto st30;
                    case 94:
                        goto tr71;
                    case 95:
                        goto st28;
                    case 101:
                        goto st30;
                    case 126:
                        goto st28;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr63;
                    } else if ((*p) >= 9)
                        goto tr61;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st29;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr63;
                goto st0;
            st30:
                if (++p == pe) goto _test_eof30;
            case 30:
                switch ((*p)) {
                    case 32:
                        goto tr81;
                    case 33:
                        goto tr82;
                    case 39:
                        goto tr83;
                    case 44:
                        goto tr82;
                    case 59:
                        goto tr82;
                    case 61:
                        goto tr84;
                    case 62:
                        goto tr85;
                    case 63:
                        goto tr82;
                    case 94:
                        goto tr86;
                    case 95:
                        goto st28;
                    case 126:
                        goto st28;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr63;
                    } else if ((*p) >= 9)
                        goto tr81;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st33;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr87;
                goto st0;
            tr87 :
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
#line 2260 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr72;
                    case 126:
                        goto tr72;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr89;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto tr72;
                } else
                    goto tr72;
                goto st0;
            tr89 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st32;
            st32:
                if (++p == pe) goto _test_eof32;
            case 32:
#line 2282 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr90;
                    case 33:
                        goto tr91;
                    case 39:
                        goto tr92;
                    case 44:
                        goto tr91;
                    case 59:
                        goto tr91;
                    case 61:
                        goto tr94;
                    case 62:
                        goto tr95;
                    case 63:
                        goto tr91;
                    case 94:
                        goto tr96;
                    case 95:
                        goto st26;
                    case 126:
                        goto st26;
                }
                if ((*p) < 48) {
                    if (9 <= (*p) && (*p) <= 13) goto tr90;
                } else if ((*p) > 57) {
                    if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st26;
                    } else if ((*p) >= 65)
                        goto st26;
                } else
                    goto st32;
                goto st0;
            st33:
                if (++p == pe) goto _test_eof33;
            case 33:
                switch ((*p)) {
                    case 32:
                        goto tr97;
                    case 33:
                        goto tr98;
                    case 39:
                        goto tr99;
                    case 44:
                        goto tr98;
                    case 59:
                        goto tr98;
                    case 61:
                        goto tr100;
                    case 62:
                        goto tr101;
                    case 63:
                        goto tr98;
                    case 94:
                        goto tr102;
                    case 95:
                        goto st28;
                    case 126:
                        goto st28;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr63;
                    } else if ((*p) >= 9)
                        goto tr97;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st33;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr63;
                goto st0;
            tr15 :
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
#line 2358 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr81;
                    case 33:
                        goto tr82;
                    case 39:
                        goto tr83;
                    case 44:
                        goto tr82;
                    case 47:
                        goto tr65;
                    case 59:
                        goto tr82;
                    case 61:
                        goto tr84;
                    case 62:
                        goto tr85;
                    case 63:
                        goto tr82;
                    case 94:
                        goto tr86;
                    case 95:
                        goto st28;
                    case 126:
                        goto st28;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr63;
                    } else if ((*p) >= 9)
                        goto tr81;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st28;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st28;
                    } else
                        goto st28;
                } else
                    goto tr63;
                goto st0;
            tr3 :
#line 19 "ragel/./text-grammar.rl"
            {
                strb.begin(p);
            }
                goto st35;
            st35:
                if (++p == pe) goto _test_eof35;
            case 35:
#line 2399 "ron/text-parser.cc"
                switch ((*p)) {
                    case 34:
                        goto st2;
                    case 39:
                        goto st2;
                    case 47:
                        goto st2;
                    case 92:
                        goto st2;
                    case 98:
                        goto st2;
                    case 110:
                        goto st2;
                    case 114:
                        goto st2;
                    case 116:
                        goto st2;
                    case 117:
                        goto st36;
                }
                goto st0;
            st36:
                if (++p == pe) goto _test_eof36;
            case 36:
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto st37;
                } else if ((*p) > 70) {
                    if (97 <= (*p) && (*p) <= 102) goto st37;
                } else
                    goto st37;
                goto st0;
            st37:
                if (++p == pe) goto _test_eof37;
            case 37:
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto st38;
                } else if ((*p) > 70) {
                    if (97 <= (*p) && (*p) <= 102) goto st38;
                } else
                    goto st38;
                goto st0;
            st38:
                if (++p == pe) goto _test_eof38;
            case 38:
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto st39;
                } else if ((*p) > 70) {
                    if (97 <= (*p) && (*p) <= 102) goto st39;
                } else
                    goto st39;
                goto st0;
            st39:
                if (++p == pe) goto _test_eof39;
            case 39:
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto st2;
                } else if ((*p) > 70) {
                    if (97 <= (*p) && (*p) <= 102) goto st2;
                } else
                    goto st2;
                goto st0;
            st40:
                if (++p == pe) goto _test_eof40;
            case 40:
                if ((*p) == 10) goto st55;
                goto st0;
            st55:
                if (++p == pe) goto _test_eof55;
            case 55:
                goto st0;
            st41:
                if (++p == pe) goto _test_eof41;
            case 41:
                switch ((*p)) {
                    case 95:
                        goto tr109;
                    case 126:
                        goto tr109;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr108;
                } else if ((*p) > 70) {
                    if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto tr109;
                    } else if ((*p) >= 71)
                        goto tr109;
                } else
                    goto tr108;
                goto st0;
            tr108 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st42;
            st42:
                if (++p == pe) goto _test_eof42;
            case 42:
#line 2511 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr110;
                    case 33:
                        goto tr111;
                    case 39:
                        goto tr113;
                    case 44:
                        goto tr111;
                    case 47:
                        goto tr114;
                    case 58:
                        goto tr116;
                    case 59:
                        goto tr111;
                    case 61:
                        goto tr117;
                    case 62:
                        goto tr118;
                    case 63:
                        goto tr111;
                    case 94:
                        goto tr119;
                    case 95:
                        goto st53;
                    case 126:
                        goto st53;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr112;
                    } else if ((*p) >= 9)
                        goto tr110;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st53;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st53;
                    } else
                        goto st53;
                } else
                    goto tr112;
                goto st0;
            tr110 :
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
                            cs = 43;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st43;
            tr143 :
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
                            cs = 43;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st43;
            st43:
                if (++p == pe) goto _test_eof43;
            case 43:
#line 2581 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto st43;
                    case 33:
                        goto tr8;
                    case 39:
                        goto st1;
                    case 44:
                        goto tr8;
                    case 58:
                        goto st44;
                    case 59:
                        goto tr8;
                    case 61:
                        goto st9;
                    case 62:
                        goto st12;
                    case 63:
                        goto tr8;
                    case 94:
                        goto st16;
                    case 95:
                        goto tr16;
                    case 126:
                        goto tr16;
                }
                if ((*p) < 48) {
                    if ((*p) > 13) {
                        if (43 <= (*p) && (*p) <= 45) goto tr13;
                    } else if ((*p) >= 9)
                        goto st43;
                } else if ((*p) > 57) {
                    if ((*p) < 71) {
                        if (65 <= (*p) && (*p) <= 70) goto tr15;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto tr16;
                    } else
                        goto tr16;
                } else
                    goto tr14;
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
                            cs = 44;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st44;
            tr147 :
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
                            cs = 44;
                            goto _out;
                        }
                    }
                    uuidb.end(p);
                }
#line 6 "ragel/./text-grammar.rl"
                { op_.SetId(Uuid{variety, value, version, origin}); }
                goto st44;
            st44:
                if (++p == pe) goto _test_eof44;
            case 44:
#line 2650 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr123;
                    case 126:
                        goto tr123;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr122;
                } else if ((*p) > 70) {
                    if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto tr123;
                    } else if ((*p) >= 71)
                        goto tr123;
                } else
                    goto tr122;
                goto st0;
            tr122 :
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
#line 2682 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr124;
                    case 33:
                        goto tr125;
                    case 39:
                        goto tr127;
                    case 44:
                        goto tr125;
                    case 47:
                        goto tr128;
                    case 59:
                        goto tr125;
                    case 61:
                        goto tr130;
                    case 62:
                        goto tr131;
                    case 63:
                        goto tr125;
                    case 94:
                        goto tr132;
                    case 95:
                        goto st49;
                    case 126:
                        goto st49;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr126;
                    } else if ((*p) >= 9)
                        goto tr124;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st49;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st49;
                    } else
                        goto st49;
                } else
                    goto tr126;
                goto st0;
            tr126 :
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
#line 2725 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr133;
                    case 126:
                        goto tr133;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr133;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto tr133;
                } else
                    goto tr133;
                goto st0;
            tr133 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st47;
            st47:
                if (++p == pe) goto _test_eof47;
            case 47:
#line 2747 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr134;
                    case 33:
                        goto tr135;
                    case 39:
                        goto tr136;
                    case 44:
                        goto tr135;
                    case 59:
                        goto tr135;
                    case 61:
                        goto tr138;
                    case 62:
                        goto tr139;
                    case 63:
                        goto tr135;
                    case 94:
                        goto tr140;
                    case 95:
                        goto st47;
                    case 126:
                        goto st47;
                }
                if ((*p) < 48) {
                    if (9 <= (*p) && (*p) <= 13) goto tr134;
                } else if ((*p) > 57) {
                    if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st47;
                    } else if ((*p) >= 65)
                        goto st47;
                } else
                    goto st47;
                goto st0;
            tr128 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st48;
            st48:
                if (++p == pe) goto _test_eof48;
            case 48:
#line 2781 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr141;
                    case 126:
                        goto tr141;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr141;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto tr141;
                } else
                    goto tr141;
                goto st0;
            tr123 :
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
            tr141 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st49;
            st49:
                if (++p == pe) goto _test_eof49;
            case 49:
#line 2814 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr124;
                    case 33:
                        goto tr125;
                    case 39:
                        goto tr127;
                    case 44:
                        goto tr125;
                    case 59:
                        goto tr125;
                    case 61:
                        goto tr130;
                    case 62:
                        goto tr131;
                    case 63:
                        goto tr125;
                    case 94:
                        goto tr132;
                    case 95:
                        goto st49;
                    case 126:
                        goto st49;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr126;
                    } else if ((*p) >= 9)
                        goto tr124;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st49;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st49;
                    } else
                        goto st49;
                } else
                    goto tr126;
                goto st0;
            tr112 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                value.end(p);
            }
#line 14 "ragel/././uuid-grammar.rl"
                { version = (*p); }
                goto st50;
            st50:
                if (++p == pe) goto _test_eof50;
            case 50:
#line 2856 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr142;
                    case 126:
                        goto tr142;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr142;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto tr142;
                } else
                    goto tr142;
                goto st0;
            tr142 :
#line 15 "ragel/././uuid-grammar.rl"
            {
                origin.begin(p);
            }
                goto st51;
            st51:
                if (++p == pe) goto _test_eof51;
            case 51:
#line 2878 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr143;
                    case 33:
                        goto tr144;
                    case 39:
                        goto tr145;
                    case 44:
                        goto tr144;
                    case 58:
                        goto tr147;
                    case 59:
                        goto tr144;
                    case 61:
                        goto tr148;
                    case 62:
                        goto tr149;
                    case 63:
                        goto tr144;
                    case 94:
                        goto tr150;
                    case 95:
                        goto st51;
                    case 126:
                        goto st51;
                }
                if ((*p) < 48) {
                    if (9 <= (*p) && (*p) <= 13) goto tr143;
                } else if ((*p) > 57) {
                    if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st51;
                    } else if ((*p) >= 65)
                        goto st51;
                } else
                    goto st51;
                goto st0;
            tr114 :
#line 11 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st52;
            st52:
                if (++p == pe) goto _test_eof52;
            case 52:
#line 2913 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95:
                        goto tr151;
                    case 126:
                        goto tr151;
                }
                if ((*p) < 65) {
                    if (48 <= (*p) && (*p) <= 57) goto tr151;
                } else if ((*p) > 90) {
                    if (97 <= (*p) && (*p) <= 122) goto tr151;
                } else
                    goto tr151;
                goto st0;
            tr109 :
#line 5 "ragel/././uuid-grammar.rl"
            {
                variety = '0';
                version = '$';
                origin = slice_t{};
                uuidb.begin(p);
            }
#line 12 "ragel/././uuid-grammar.rl"
                { value.begin(p); }
                goto st53;
            tr151 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                value.begin(p);
            }
                goto st53;
            st53:
                if (++p == pe) goto _test_eof53;
            case 53:
#line 2946 "ron/text-parser.cc"
                switch ((*p)) {
                    case 32:
                        goto tr110;
                    case 33:
                        goto tr111;
                    case 39:
                        goto tr113;
                    case 44:
                        goto tr111;
                    case 58:
                        goto tr116;
                    case 59:
                        goto tr111;
                    case 61:
                        goto tr117;
                    case 62:
                        goto tr118;
                    case 63:
                        goto tr111;
                    case 94:
                        goto tr119;
                    case 95:
                        goto st53;
                    case 126:
                        goto st53;
                }
                if ((*p) < 43) {
                    if ((*p) > 13) {
                        if (36 <= (*p) && (*p) <= 37) goto tr112;
                    } else if ((*p) >= 9)
                        goto tr110;
                } else if ((*p) > 45) {
                    if ((*p) < 65) {
                        if (48 <= (*p) && (*p) <= 57) goto st53;
                    } else if ((*p) > 90) {
                        if (97 <= (*p) && (*p) <= 122) goto st53;
                    } else
                        goto st53;
                } else
                    goto tr112;
                goto st0;
        }
    _test_eof54:
        cs = 54;
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
    _test_eof55:
        cs = 55;
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

    _test_eof : {}
    _out : {}
    }

#line 63 "ragel/text-parser.rl"

    off_ = p - pb;

    if (op_.size()) prev_id_ = op_.id();

    // std::cerr << "ending with [" <<p<<"] state "<<cs<<" "<<op_.size()<<"
    // atoms
    // "<<(pe-p)<<" bytes left, prev_id_ "<<prev_id_.str()<<'\n';

    if (term && cs != RON_error) {
        return true;
    } else {
        cs = RON_error;
        return false;
    }
}

}  // namespace ron
