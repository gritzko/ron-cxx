
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
    Atoms &atoms = op_;
    fsize_t &line = line_;
    uint8_t &cs = ragel_state_;
    static_assert(RON_first_final < UINT8_MAX,
                  "this grammar should not change much");

    if (span_size_) {
        --span_size_;
        atoms[1] = atoms[0];
        ++atoms[0];
        if (atoms.back().type() == STRING && atoms.back().value.cp) {
            NextCodepoint(atoms.back());
        }
        return Status::OK;
    }

    switch (cs) {
        case RON_error:
            if (data().range().begin() != 0) {
                return Status::BAD_STATE;
            }

#line 46 "ron/text-parser.cc"
            { cs = RON_start; }

#line 33 "ragel/text-parser.rl"
            break;

        case RON_FULL_STOP:
            cs = RON_error;
            return Status::ENDOFFRAME;

        default:
            break;
    }

    if (data().empty()) {
        cs = RON_error;
        return Status::ENDOFFRAME;
    }

    CharRef buffer = data().buffer();
    CharRef pb = data().begin();
    CharRef p = pb;
    CharRef pe = data().end();
    CharRef eof = pe;
    CharRef lineb = pb;
    CharRef intb{p};
    CharRef floatb{p};
    CharRef strb{p};
    CharRef uuidb{p};
    CharRef wordb{p};
    Codepoint cp{0};
    fsize_t cp_size{0};
    span_size_ = 0;
    char term{0};
    Slice value, origin;
    char variety{0}, version{0};

    Uuid prev = atoms.empty() ? Uuid::NIL : A2U(atoms[0]);
    atoms.clear();
    op_.push_back(prev.inc());
    op_.push_back(prev);

#line 92 "ron/text-parser.cc"
    {
        if (p == pe) goto _test_eof;
        switch (cs) {
        tr223 :
#line 52 "ragel/./text-grammar.rl"
        {
            if ((*p) == '\n') {
                line++;
                lineb = p;
            }
        }
            goto st74;
        tr14 :
#line 48 "ragel/./text-grammar.rl"
        {
            term = (*p);
            if (p < pe - 1) {
                p++;
                cs = 74;
                goto _out;
            }
        }
            goto st74;
        tr63 :
#line 16 "ragel/./text-grammar.rl"
        {
            Range range{buffer, intb, p};
            if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                cs = 0;
                {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            op_.emplace_back(INT, range);
            uuidb = nullptr;  // sabotage uuid
        }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr73 :
#line 28 "ragel/./text-grammar.rl"
        {
            Range range{buffer, floatb, p};
            if (range.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            op_.emplace_back(FLOAT, range);
        }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr87 :
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
                        cs = 74;
                        goto _out;
                    }
                }
                op_.push_back(Uuid{variety, value, version, origin});
            }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr98 :
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
                        cs = 74;
                        goto _out;
                    }
                }
                op_.push_back(Uuid{variety, value, version, origin});
            }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr113 :
#line 16 "ragel/./text-grammar.rl"
        {
            Range range{buffer, intb, p};
            if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                cs = 0;
                {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            op_.emplace_back(INT, range);
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
                            cs = 74;
                            goto _out;
                        }
                    }
                    op_.push_back(Uuid{variety, value, version, origin});
                }
            }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr126 :
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
                            cs = 74;
                            goto _out;
                        }
                    }
                    op_.push_back(Uuid{variety, value, version, origin});
                }
            }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr135 :
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
                            cs = 74;
                            goto _out;
                        }
                    }
                    op_.push_back(Uuid{variety, value, version, origin});
                }
            }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr145 :
#line 28 "ragel/./text-grammar.rl"
        {
            Range range{buffer, floatb, p};
            if (range.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            op_.emplace_back(FLOAT, range);
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
                            cs = 74;
                            goto _out;
                        }
                    }
                    op_.push_back(Uuid{variety, value, version, origin});
                }
            }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr153 :
#line 28 "ragel/./text-grammar.rl"
        {
            Range range{buffer, floatb, p};
            if (range.size() > 24) {
                cs = 0;
                {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            op_.emplace_back(FLOAT, range);
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
                            cs = 74;
                            goto _out;
                        }
                    }
                    op_.push_back(Uuid{variety, value, version, origin});
                }
            }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr178 :
#line 14 "ragel/././uuid-grammar.rl"
        {
            value = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 9 "ragel/./text-grammar.rl"
            { op_[0] = Uuid{variety, value, version, origin}; }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr193 :
#line 14 "ragel/././uuid-grammar.rl"
        {
            value = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 12 "ragel/./text-grammar.rl"
            { op_[1] = Uuid{variety, value, version, origin}; }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr204 :
#line 17 "ragel/././uuid-grammar.rl"
        {
            origin = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 12 "ragel/./text-grammar.rl"
            { op_[1] = Uuid{variety, value, version, origin}; }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
                if (p < pe - 1) {
                    p++;
                    cs = 74;
                    goto _out;
                }
            }
            goto st74;
        tr214 :
#line 17 "ragel/././uuid-grammar.rl"
        {
            origin = Slice{wordb, p};
        }
#line 18 "ragel/././uuid-grammar.rl"
            {}
#line 9 "ragel/./text-grammar.rl"
            { op_[0] = Uuid{variety, value, version, origin}; }
#line 48 "ragel/./text-grammar.rl"
            {
                term = (*p);
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
#line 368 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr223;
                    case 32u:
                        goto tr223;
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
                        goto tr223;
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
            tr64 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
                uuidb = nullptr;  // sabotage uuid
            }
                goto st1;
            tr74 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
            }
                goto st1;
            tr89 :
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
                    op_.push_back(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr99 :
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
                    op_.push_back(Uuid{variety, value, version, origin});
                }
                goto st1;
            tr115 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st1;
            tr127 :
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st1;
            tr136 :
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st1;
            tr146 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st1;
            tr154 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 1;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st1;
            tr180 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st1;
            tr195 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
                goto st1;
            tr205 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
                goto st1;
            tr215 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st1;
            st1:
                if (++p == pe) goto _test_eof1;
            case 1:
#line 596 "ron/text-parser.cc"
                switch ((*p)) {
                    case 0u:
                        goto st0;
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
                cp_size = 0;
            }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st2;
            tr7 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st2;
            tr159 :
#line 58 "ragel/./text-grammar.rl"
            {
                cp = decode_esc((*p));
            }
                goto st2;
            tr165 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st2;
            tr171 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st2;
            st2:
                if (++p == pe) goto _test_eof2;
            case 2:
#line 655 "ron/text-parser.cc"
                switch ((*p)) {
                    case 0u:
                        goto st0;
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr8;
                    case 92u:
                        goto tr9;
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
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
                cp_size = 0;
            }
#line 23 "ragel/./text-grammar.rl"
                {
                    op_.emplace_back(STRING, Range{buffer, strb, p});
                    op_.back().value.cp_size = cp_size;
                }
                goto st3;
            tr8 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 23 "ragel/./text-grammar.rl"
                {
                    op_.emplace_back(STRING, Range{buffer, strb, p});
                    op_.back().value.cp_size = cp_size;
                }
                goto st3;
            tr166 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 23 "ragel/./text-grammar.rl"
                {
                    op_.emplace_back(STRING, Range{buffer, strb, p});
                    op_.back().value.cp_size = cp_size;
                }
                goto st3;
            st3:
                if (++p == pe) goto _test_eof3;
            case 3:
#line 715 "ron/text-parser.cc"
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
#line 52 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st4;
            tr62 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
                uuidb = nullptr;  // sabotage uuid
            }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr72 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
            }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr86 :
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
                    op_.push_back(Uuid{variety, value, version, origin});
                }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr97 :
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
                    op_.push_back(Uuid{variety, value, version, origin});
                }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr112 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr125 :
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr134 :
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr144 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr152 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 4;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr192 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st4;
            tr203 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
#line 52 "ragel/./text-grammar.rl"
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
#line 983 "ron/text-parser.cc"
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
#line 52 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st5;
            tr65 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
                uuidb = nullptr;  // sabotage uuid
            }
                goto st5;
            tr75 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
            }
                goto st5;
            tr90 :
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
                    op_.push_back(Uuid{variety, value, version, origin});
                }
                goto st5;
            tr100 :
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
                    op_.push_back(Uuid{variety, value, version, origin});
                }
                goto st5;
            tr116 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st5;
            tr128 :
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st5;
            tr137 :
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st5;
            tr147 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st5;
            tr155 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 5;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st5;
            tr181 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st5;
            tr196 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
                goto st5;
            tr206 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
                goto st5;
            tr216 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st5;
            st5:
                if (++p == pe) goto _test_eof5;
            case 5:
#line 1215 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr24;
                    case 32u:
                        goto tr24;
                    case 39u:
                        goto st6;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr26;
                } else if ((*p) >= 9u)
                    goto tr24;
                goto st0;
            st6:
                if (++p == pe) goto _test_eof6;
            case 6:
                switch ((*p)) {
                    case 0u:
                        goto st0;
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
                cp_size = 0;
            }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st7;
            tr33 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st7;
            tr44 :
#line 58 "ragel/./text-grammar.rl"
            {
                cp = decode_esc((*p));
            }
                goto st7;
            tr50 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 8 "ragel/././utf8-grammar.rl"
                { cp = (*p); }
                goto st7;
            tr56 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st7;
            st7:
                if (++p == pe) goto _test_eof7;
            case 7:
#line 1289 "ron/text-parser.cc"
                switch ((*p)) {
                    case 0u:
                        goto st0;
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr34;
                    case 92u:
                        goto tr35;
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
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
                cp_size = 0;
            }
#line 23 "ragel/./text-grammar.rl"
                {
                    op_.emplace_back(STRING, Range{buffer, strb, p});
                    op_.back().value.cp_size = cp_size;
                }
                goto st8;
            tr34 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 23 "ragel/./text-grammar.rl"
                {
                    op_.emplace_back(STRING, Range{buffer, strb, p});
                    op_.back().value.cp_size = cp_size;
                }
                goto st8;
            tr51 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 23 "ragel/./text-grammar.rl"
                {
                    op_.emplace_back(STRING, Range{buffer, strb, p});
                    op_.back().value.cp_size = cp_size;
                }
                goto st8;
            st8:
                if (++p == pe) goto _test_eof8;
            case 8:
#line 1349 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr39;
                    case 32u:
                        goto tr39;
                    case 41u:
                        goto tr40;
                }
                if (9u <= (*p) && (*p) <= 10u) goto tr39;
                goto st0;
            tr41 :
#line 52 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st9;
            tr39 :
#line 68 "ragel/./text-grammar.rl"
            {
                if (op_.back().value.cp_size) {
                    NextCodepoint(op_.back());
                }
                span_size_ = cp_size - 1;
            }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st9;
            tr59 :
#line 65 "ragel/./text-grammar.rl"
            {
                --span_size_;
            }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st9;
            st9:
                if (++p == pe) goto _test_eof9;
            case 9:
#line 1398 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr41;
                    case 32u:
                        goto tr41;
                    case 41u:
                        goto st10;
                }
                if (9u <= (*p) && (*p) <= 10u) goto tr41;
                goto st0;
            tr43 :
#line 52 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st10;
            tr40 :
#line 68 "ragel/./text-grammar.rl"
            {
                if (op_.back().value.cp_size) {
                    NextCodepoint(op_.back());
                }
                span_size_ = cp_size - 1;
            }
                goto st10;
            tr60 :
#line 65 "ragel/./text-grammar.rl"
            {
                --span_size_;
            }
                goto st10;
            st10:
                if (++p == pe) goto _test_eof10;
            case 10:
#line 1433 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr43;
                    case 32u:
                        goto tr43;
                    case 33u:
                        goto tr14;
                    case 44u:
                        goto tr14;
                    case 59u:
                        goto tr14;
                    case 63u:
                        goto tr14;
                }
                if (9u <= (*p) && (*p) <= 10u) goto tr43;
                goto st0;
            tr29 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
                cp_size = 0;
            }
                goto st11;
            tr35 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
                goto st11;
            tr52 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
                goto st11;
            st11:
                if (++p == pe) goto _test_eof11;
            case 11:
#line 1467 "ron/text-parser.cc"
                switch ((*p)) {
                    case 34u:
                        goto tr44;
                    case 39u:
                        goto tr44;
                    case 47u:
                        goto tr44;
                    case 92u:
                        goto tr44;
                    case 98u:
                        goto tr44;
                    case 110u:
                        goto tr44;
                    case 114u:
                        goto tr44;
                    case 116u:
                        goto tr44;
                    case 117u:
                        goto st12;
                }
                goto st0;
            st12:
                if (++p == pe) goto _test_eof12;
            case 12:
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st13;
                } else if ((*p) > 70u) {
                    if (97u <= (*p) && (*p) <= 102u) goto st13;
                } else
                    goto st13;
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
                switch ((*p)) {
                    case 0u:
                        goto st0;
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr51;
                    case 92u:
                        goto tr52;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr53;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr55;
                } else
                    goto tr54;
                goto tr50;
            tr30 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
                cp_size = 0;
            }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st17;
            tr36 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st17;
            tr53 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st17;
            tr57 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st17;
            st17:
                if (++p == pe) goto _test_eof17;
            case 17:
#line 1590 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr56;
                goto st0;
            tr31 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
                cp_size = 0;
            }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st18;
            tr37 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st18;
            tr54 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st18;
            tr58 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st18;
            st18:
                if (++p == pe) goto _test_eof18;
            case 18:
#line 1626 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr57;
                goto st0;
            tr32 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
                cp_size = 0;
            }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st19;
            tr38 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st19;
            tr55 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st19;
            st19:
                if (++p == pe) goto _test_eof19;
            case 19:
#line 1658 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr58;
                goto st0;
            tr26 :
#line 61 "ragel/./text-grammar.rl"
            {
                span_size_ *= 10;
                span_size_ += (*p) - '0';
            }
                goto st20;
            st20:
                if (++p == pe) goto _test_eof20;
            case 20:
#line 1673 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr59;
                    case 32u:
                        goto tr59;
                    case 41u:
                        goto tr60;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr26;
                } else if ((*p) >= 9u)
                    goto tr59;
                goto st0;
            tr20 :
#line 15 "ragel/./text-grammar.rl"
            {
                intb = p;
            }
#line 27 "ragel/./text-grammar.rl"
                { floatb = p; }
                goto st21;
            st21:
                if (++p == pe) goto _test_eof21;
            case 21:
#line 1695 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st22;
                goto st0;
            st22:
                if (++p == pe) goto _test_eof22;
            case 22:
                switch ((*p)) {
                    case 13u:
                        goto tr62;
                    case 32u:
                        goto tr62;
                    case 33u:
                        goto tr63;
                    case 39u:
                        goto tr64;
                    case 40u:
                        goto tr65;
                    case 44u:
                        goto tr63;
                    case 46u:
                        goto st23;
                    case 59u:
                        goto tr63;
                    case 61u:
                        goto tr67;
                    case 62u:
                        goto tr68;
                    case 63u:
                        goto tr63;
                    case 69u:
                        goto st35;
                    case 94u:
                        goto tr70;
                    case 101u:
                        goto st35;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st22;
                } else if ((*p) >= 9u)
                    goto tr62;
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
                        goto tr72;
                    case 32u:
                        goto tr72;
                    case 33u:
                        goto tr73;
                    case 39u:
                        goto tr74;
                    case 40u:
                        goto tr75;
                    case 44u:
                        goto tr73;
                    case 59u:
                        goto tr73;
                    case 61u:
                        goto tr76;
                    case 62u:
                        goto tr77;
                    case 63u:
                        goto tr73;
                    case 69u:
                        goto st35;
                    case 94u:
                        goto tr78;
                    case 101u:
                        goto st35;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st24;
                } else if ((*p) >= 9u)
                    goto tr72;
                goto st0;
            tr79 :
#line 52 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st25;
            tr67 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 25;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
                uuidb = nullptr;  // sabotage uuid
            }
                goto st25;
            tr76 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 25;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
            }
                goto st25;
            tr93 :
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
                            cs = 25;
                            goto _out;
                        }
                    }
                    op_.push_back(Uuid{variety, value, version, origin});
                }
                goto st25;
            tr102 :
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
                            cs = 25;
                            goto _out;
                        }
                    }
                    op_.push_back(Uuid{variety, value, version, origin});
                }
                goto st25;
            tr119 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 25;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
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
                                cs = 25;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st25;
            tr130 :
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
                                cs = 25;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st25;
            tr138 :
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
                                cs = 25;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st25;
            tr149 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 25;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                                cs = 25;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st25;
            tr156 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 25;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                                cs = 25;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st25;
            tr185 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st25;
            tr199 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
                goto st25;
            tr208 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
                goto st25;
            tr219 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st25;
            st25:
                if (++p == pe) goto _test_eof25;
            case 25:
#line 1955 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr79;
                    case 32u:
                        goto tr79;
                    case 43u:
                        goto tr80;
                    case 45u:
                        goto tr80;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr81;
                } else if ((*p) >= 9u)
                    goto tr79;
                goto st0;
            tr80 :
#line 15 "ragel/./text-grammar.rl"
            {
                intb = p;
            }
                goto st26;
            st26:
                if (++p == pe) goto _test_eof26;
            case 26:
#line 1976 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st27;
                goto st0;
            tr81 :
#line 15 "ragel/./text-grammar.rl"
            {
                intb = p;
            }
                goto st27;
            st27:
                if (++p == pe) goto _test_eof27;
            case 27:
#line 1988 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr62;
                    case 32u:
                        goto tr62;
                    case 33u:
                        goto tr63;
                    case 39u:
                        goto tr64;
                    case 40u:
                        goto tr65;
                    case 44u:
                        goto tr63;
                    case 59u:
                        goto tr63;
                    case 61u:
                        goto tr67;
                    case 62u:
                        goto tr68;
                    case 63u:
                        goto tr63;
                    case 94u:
                        goto tr70;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st27;
                } else if ((*p) >= 9u)
                    goto tr62;
                goto st0;
            tr83 :
#line 52 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st28;
            tr68 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 28;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
                uuidb = nullptr;  // sabotage uuid
            }
                goto st28;
            tr77 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 28;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
            }
                goto st28;
            tr94 :
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
                            cs = 28;
                            goto _out;
                        }
                    }
                    op_.push_back(Uuid{variety, value, version, origin});
                }
                goto st28;
            tr103 :
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
                            cs = 28;
                            goto _out;
                        }
                    }
                    op_.push_back(Uuid{variety, value, version, origin});
                }
                goto st28;
            tr120 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 28;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
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
                                cs = 28;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st28;
            tr131 :
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
                                cs = 28;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st28;
            tr139 :
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
                                cs = 28;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st28;
            tr150 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 28;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                                cs = 28;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st28;
            tr157 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 28;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                                cs = 28;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st28;
            tr186 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st28;
            tr200 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
                goto st28;
            tr209 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
                goto st28;
            tr220 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st28;
            st28:
                if (++p == pe) goto _test_eof28;
            case 28:
#line 2206 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr83;
                    case 32u:
                        goto tr83;
                    case 95u:
                        goto tr85;
                    case 126u:
                        goto tr85;
                }
                if ((*p) < 65u) {
                    if ((*p) > 10u) {
                        if (48u <= (*p) && (*p) <= 57u) goto tr84;
                    } else if ((*p) >= 9u)
                        goto tr83;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr85;
                    } else if ((*p) >= 71u)
                        goto tr85;
                } else
                    goto tr84;
                goto st0;
            tr84 :
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
#line 2243 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr86;
                    case 32u:
                        goto tr86;
                    case 33u:
                        goto tr87;
                    case 39u:
                        goto tr89;
                    case 40u:
                        goto tr90;
                    case 44u:
                        goto tr87;
                    case 47u:
                        goto tr91;
                    case 59u:
                        goto tr87;
                    case 61u:
                        goto tr93;
                    case 62u:
                        goto tr94;
                    case 63u:
                        goto tr87;
                    case 94u:
                        goto tr95;
                    case 95u:
                        goto st39;
                    case 126u:
                        goto st39;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr88;
                    } else if ((*p) >= 9u)
                        goto tr86;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st39;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st39;
                    } else
                        goto st39;
                } else
                    goto tr88;
                goto st0;
            tr88 :
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
#line 2288 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr96;
                    case 126u:
                        goto tr96;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr96;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr96;
                } else
                    goto tr96;
                goto st0;
            tr96 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st31;
            st31:
                if (++p == pe) goto _test_eof31;
            case 31:
#line 2310 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr97;
                    case 32u:
                        goto tr97;
                    case 33u:
                        goto tr98;
                    case 39u:
                        goto tr99;
                    case 40u:
                        goto tr100;
                    case 44u:
                        goto tr98;
                    case 59u:
                        goto tr98;
                    case 61u:
                        goto tr102;
                    case 62u:
                        goto tr103;
                    case 63u:
                        goto tr98;
                    case 94u:
                        goto tr104;
                    case 95u:
                        goto st31;
                    case 126u:
                        goto st31;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr97;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st31;
                    } else if ((*p) >= 65u)
                        goto st31;
                } else
                    goto st31;
                goto st0;
            tr105 :
#line 52 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st32;
            tr70 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 32;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
                uuidb = nullptr;  // sabotage uuid
            }
                goto st32;
            tr78 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 32;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
            }
                goto st32;
            tr95 :
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
                            cs = 32;
                            goto _out;
                        }
                    }
                    op_.push_back(Uuid{variety, value, version, origin});
                }
                goto st32;
            tr104 :
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
                            cs = 32;
                            goto _out;
                        }
                    }
                    op_.push_back(Uuid{variety, value, version, origin});
                }
                goto st32;
            tr123 :
#line 16 "ragel/./text-grammar.rl"
            {
                Range range{buffer, intb, p};
                if (range.size() >= 19 && int_too_big(Slice{intb, p})) {
                    cs = 0;
                    {
                        p++;
                        cs = 32;
                        goto _out;
                    }
                }
                op_.emplace_back(INT, range);
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
                                cs = 32;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st32;
            tr132 :
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
                                cs = 32;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st32;
            tr140 :
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
                                cs = 32;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st32;
            tr151 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 32;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                                cs = 32;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st32;
            tr158 :
#line 28 "ragel/./text-grammar.rl"
            {
                Range range{buffer, floatb, p};
                if (range.size() > 24) {
                    cs = 0;
                    {
                        p++;
                        cs = 32;
                        goto _out;
                    }
                }
                op_.emplace_back(FLOAT, range);
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
                                cs = 32;
                                goto _out;
                            }
                        }
                        op_.push_back(Uuid{variety, value, version, origin});
                    }
                }
                goto st32;
            tr187 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st32;
            tr201 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
                goto st32;
            tr210 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 12 "ragel/./text-grammar.rl"
                { op_[1] = Uuid{variety, value, version, origin}; }
                goto st32;
            tr221 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st32;
            st32:
                if (++p == pe) goto _test_eof32;
            case 32:
#line 2536 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr105;
                    case 32u:
                        goto tr105;
                    case 43u:
                        goto tr106;
                    case 45u:
                        goto tr106;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr107;
                } else if ((*p) >= 9u)
                    goto tr105;
                goto st0;
            tr106 :
#line 27 "ragel/./text-grammar.rl"
            {
                floatb = p;
            }
                goto st33;
            st33:
                if (++p == pe) goto _test_eof33;
            case 33:
#line 2557 "ron/text-parser.cc"
                if (48u <= (*p) && (*p) <= 57u) goto st34;
                goto st0;
            tr107 :
#line 27 "ragel/./text-grammar.rl"
            {
                floatb = p;
            }
                goto st34;
            st34:
                if (++p == pe) goto _test_eof34;
            case 34:
#line 2569 "ron/text-parser.cc"
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
                        goto tr72;
                    case 32u:
                        goto tr72;
                    case 33u:
                        goto tr73;
                    case 39u:
                        goto tr74;
                    case 40u:
                        goto tr75;
                    case 44u:
                        goto tr73;
                    case 59u:
                        goto tr73;
                    case 61u:
                        goto tr76;
                    case 62u:
                        goto tr77;
                    case 63u:
                        goto tr73;
                    case 94u:
                        goto tr78;
                }
                if ((*p) > 10u) {
                    if (48u <= (*p) && (*p) <= 57u) goto st37;
                } else if ((*p) >= 9u)
                    goto tr72;
                goto st0;
            tr91 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st38;
            st38:
                if (++p == pe) goto _test_eof38;
            case 38:
#line 2627 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr111;
                    case 126u:
                        goto tr111;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr111;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr111;
                } else
                    goto tr111;
                goto st0;
            tr85 :
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
            tr111 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st39;
            st39:
                if (++p == pe) goto _test_eof39;
            case 39:
#line 2660 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr86;
                    case 32u:
                        goto tr86;
                    case 33u:
                        goto tr87;
                    case 39u:
                        goto tr89;
                    case 40u:
                        goto tr90;
                    case 44u:
                        goto tr87;
                    case 59u:
                        goto tr87;
                    case 61u:
                        goto tr93;
                    case 62u:
                        goto tr94;
                    case 63u:
                        goto tr87;
                    case 94u:
                        goto tr95;
                    case 95u:
                        goto st39;
                    case 126u:
                        goto st39;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr88;
                    } else if ((*p) >= 9u)
                        goto tr86;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st39;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st39;
                    } else
                        goto st39;
                } else
                    goto tr88;
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
                goto st40;
            st40:
                if (++p == pe) goto _test_eof40;
            case 40:
#line 2713 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr112;
                    case 32u:
                        goto tr112;
                    case 33u:
                        goto tr113;
                    case 39u:
                        goto tr115;
                    case 40u:
                        goto tr116;
                    case 44u:
                        goto tr113;
                    case 46u:
                        goto st23;
                    case 47u:
                        goto tr117;
                    case 59u:
                        goto tr113;
                    case 61u:
                        goto tr119;
                    case 62u:
                        goto tr120;
                    case 63u:
                        goto tr113;
                    case 69u:
                        goto st46;
                    case 94u:
                        goto tr123;
                    case 95u:
                        goto st44;
                    case 101u:
                        goto st46;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr114;
                    } else if ((*p) >= 9u)
                        goto tr112;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st45;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr114;
                goto st0;
            tr114 :
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
#line 2761 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr124;
                    case 126u:
                        goto tr124;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr124;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr124;
                } else
                    goto tr124;
                goto st0;
            tr124 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st42;
            st42:
                if (++p == pe) goto _test_eof42;
            case 42:
#line 2783 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr125;
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
                        goto st42;
                    case 126u:
                        goto st42;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr125;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st42;
                    } else if ((*p) >= 65u)
                        goto st42;
                } else
                    goto st42;
                goto st0;
            tr117 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st43;
            st43:
                if (++p == pe) goto _test_eof43;
            case 43:
#line 2819 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr133;
                    case 126u:
                        goto tr133;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr133;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr133;
                } else
                    goto tr133;
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
            tr133 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st44;
            st44:
                if (++p == pe) goto _test_eof44;
            case 44:
#line 2852 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr134;
                    case 32u:
                        goto tr134;
                    case 33u:
                        goto tr135;
                    case 39u:
                        goto tr136;
                    case 40u:
                        goto tr137;
                    case 44u:
                        goto tr135;
                    case 59u:
                        goto tr135;
                    case 61u:
                        goto tr138;
                    case 62u:
                        goto tr139;
                    case 63u:
                        goto tr135;
                    case 94u:
                        goto tr140;
                    case 95u:
                        goto st44;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr114;
                    } else if ((*p) >= 9u)
                        goto tr134;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st44;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr114;
                goto st0;
            st45:
                if (++p == pe) goto _test_eof45;
            case 45:
                switch ((*p)) {
                    case 13u:
                        goto tr112;
                    case 32u:
                        goto tr112;
                    case 33u:
                        goto tr113;
                    case 39u:
                        goto tr115;
                    case 40u:
                        goto tr116;
                    case 44u:
                        goto tr113;
                    case 46u:
                        goto st23;
                    case 59u:
                        goto tr113;
                    case 61u:
                        goto tr119;
                    case 62u:
                        goto tr120;
                    case 63u:
                        goto tr113;
                    case 69u:
                        goto st46;
                    case 94u:
                        goto tr123;
                    case 95u:
                        goto st44;
                    case 101u:
                        goto st46;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr114;
                    } else if ((*p) >= 9u)
                        goto tr112;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st45;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr114;
                goto st0;
            st46:
                if (++p == pe) goto _test_eof46;
            case 46:
                switch ((*p)) {
                    case 13u:
                        goto tr134;
                    case 32u:
                        goto tr134;
                    case 33u:
                        goto tr135;
                    case 39u:
                        goto tr136;
                    case 40u:
                        goto tr137;
                    case 44u:
                        goto tr135;
                    case 59u:
                        goto tr135;
                    case 61u:
                        goto tr138;
                    case 62u:
                        goto tr139;
                    case 63u:
                        goto tr135;
                    case 94u:
                        goto tr140;
                    case 95u:
                        goto st44;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr114;
                    } else if ((*p) >= 9u)
                        goto tr134;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st49;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr141;
                goto st0;
            tr141 :
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
#line 2973 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr124;
                    case 126u:
                        goto tr124;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr143;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr124;
                } else
                    goto tr124;
                goto st0;
            tr143 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st48;
            st48:
                if (++p == pe) goto _test_eof48;
            case 48:
#line 2995 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr144;
                    case 32u:
                        goto tr144;
                    case 33u:
                        goto tr145;
                    case 39u:
                        goto tr146;
                    case 40u:
                        goto tr147;
                    case 44u:
                        goto tr145;
                    case 59u:
                        goto tr145;
                    case 61u:
                        goto tr149;
                    case 62u:
                        goto tr150;
                    case 63u:
                        goto tr145;
                    case 94u:
                        goto tr151;
                    case 95u:
                        goto st42;
                    case 126u:
                        goto st42;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr144;
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
                        goto tr152;
                    case 32u:
                        goto tr152;
                    case 33u:
                        goto tr153;
                    case 39u:
                        goto tr154;
                    case 40u:
                        goto tr155;
                    case 44u:
                        goto tr153;
                    case 59u:
                        goto tr153;
                    case 61u:
                        goto tr156;
                    case 62u:
                        goto tr157;
                    case 63u:
                        goto tr153;
                    case 94u:
                        goto tr158;
                    case 95u:
                        goto st44;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr114;
                    } else if ((*p) >= 9u)
                        goto tr152;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st49;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr114;
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
#line 3075 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr134;
                    case 32u:
                        goto tr134;
                    case 33u:
                        goto tr135;
                    case 39u:
                        goto tr136;
                    case 40u:
                        goto tr137;
                    case 44u:
                        goto tr135;
                    case 47u:
                        goto tr117;
                    case 59u:
                        goto tr135;
                    case 61u:
                        goto tr138;
                    case 62u:
                        goto tr139;
                    case 63u:
                        goto tr135;
                    case 94u:
                        goto tr140;
                    case 95u:
                        goto st44;
                    case 126u:
                        goto st44;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr114;
                    } else if ((*p) >= 9u)
                        goto tr134;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st44;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st44;
                    } else
                        goto st44;
                } else
                    goto tr114;
                goto st0;
            tr3 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
                cp_size = 0;
            }
                goto st51;
            tr9 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
                goto st51;
            tr167 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
                goto st51;
            st51:
                if (++p == pe) goto _test_eof51;
            case 51:
#line 3132 "ron/text-parser.cc"
                switch ((*p)) {
                    case 34u:
                        goto tr159;
                    case 39u:
                        goto tr159;
                    case 47u:
                        goto tr159;
                    case 92u:
                        goto tr159;
                    case 98u:
                        goto tr159;
                    case 110u:
                        goto tr159;
                    case 114u:
                        goto tr159;
                    case 116u:
                        goto tr159;
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
                    case 0u:
                        goto st0;
                    case 10u:
                        goto st0;
                    case 13u:
                        goto st0;
                    case 39u:
                        goto tr166;
                    case 92u:
                        goto tr167;
                }
                if ((*p) < 224u) {
                    if ((*p) > 191u) {
                        if (192u <= (*p) && (*p) <= 223u) goto tr168;
                    } else if ((*p) >= 128u)
                        goto st0;
                } else if ((*p) > 239u) {
                    if ((*p) > 247u) {
                        if (248u <= (*p)) goto st0;
                    } else if ((*p) >= 240u)
                        goto tr170;
                } else
                    goto tr169;
                goto tr165;
            tr4 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
                cp_size = 0;
            }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st57;
            tr10 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st57;
            tr168 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 9 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0x1f; }
                goto st57;
            tr172 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st57;
            st57:
                if (++p == pe) goto _test_eof57;
            case 57:
#line 3255 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr171;
                goto st0;
            tr5 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
                cp_size = 0;
            }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st58;
            tr11 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st58;
            tr169 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 10 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 0xf; }
                goto st58;
            tr173 :
#line 7 "ragel/././utf8-grammar.rl"
            {
                cp = (cp << 6) | ((*p) & 0x3f);
            }
                goto st58;
            st58:
                if (++p == pe) goto _test_eof58;
            case 58:
#line 3291 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr172;
                goto st0;
            tr6 :
#line 22 "ragel/./text-grammar.rl"
            {
                strb = p;
                cp_size = 0;
            }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st59;
            tr12 :
#line 43 "ragel/./text-grammar.rl"
            {
                cp_size++;
            }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st59;
            tr170 :
#line 59 "ragel/./text-grammar.rl"
            {
                cp = decode_hex_cp(Slice{p - 4, 4});
            }
#line 43 "ragel/./text-grammar.rl"
                { cp_size++; }
#line 11 "ragel/././utf8-grammar.rl"
                { cp = (*p) & 7; }
                goto st59;
            st59:
                if (++p == pe) goto _test_eof59;
            case 59:
#line 3323 "ron/text-parser.cc"
                if (128u <= (*p) && (*p) <= 191u) goto tr173;
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
                        goto tr176;
                    case 126u:
                        goto tr176;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr175;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr176;
                    } else if ((*p) >= 71u)
                        goto tr176;
                } else
                    goto tr175;
                goto st0;
            tr175 :
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
#line 3374 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr177;
                    case 32u:
                        goto tr177;
                    case 33u:
                        goto tr178;
                    case 39u:
                        goto tr180;
                    case 40u:
                        goto tr181;
                    case 44u:
                        goto tr178;
                    case 47u:
                        goto tr182;
                    case 58u:
                        goto tr184;
                    case 59u:
                        goto tr178;
                    case 61u:
                        goto tr185;
                    case 62u:
                        goto tr186;
                    case 63u:
                        goto tr178;
                    case 94u:
                        goto tr187;
                    case 95u:
                        goto st73;
                    case 126u:
                        goto st73;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr179;
                    } else if ((*p) >= 9u)
                        goto tr177;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st73;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st73;
                    } else
                        goto st73;
                } else
                    goto tr179;
                goto st0;
            tr188 :
#line 52 "ragel/./text-grammar.rl"
            {
                if ((*p) == '\n') {
                    line++;
                    lineb = p;
                }
            }
                goto st63;
            tr177 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
#line 52 "ragel/./text-grammar.rl"
                {
                    if ((*p) == '\n') {
                        line++;
                        lineb = p;
                    }
                }
                goto st63;
            tr213 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
#line 52 "ragel/./text-grammar.rl"
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
#line 3461 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr188;
                    case 32u:
                        goto tr188;
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
                        goto tr188;
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
            tr184 :
#line 14 "ragel/././uuid-grammar.rl"
            {
                value = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st64;
            tr218 :
#line 17 "ragel/././uuid-grammar.rl"
            {
                origin = Slice{wordb, p};
            }
#line 18 "ragel/././uuid-grammar.rl"
                {}
#line 9 "ragel/./text-grammar.rl"
                { op_[0] = Uuid{variety, value, version, origin}; }
                goto st64;
            st64:
                if (++p == pe) goto _test_eof64;
            case 64:
#line 3524 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr191;
                    case 126u:
                        goto tr191;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr190;
                } else if ((*p) > 70u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto tr191;
                    } else if ((*p) >= 71u)
                        goto tr191;
                } else
                    goto tr190;
                goto st0;
            tr190 :
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
#line 3556 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr192;
                    case 32u:
                        goto tr192;
                    case 33u:
                        goto tr193;
                    case 39u:
                        goto tr195;
                    case 40u:
                        goto tr196;
                    case 44u:
                        goto tr193;
                    case 47u:
                        goto tr197;
                    case 59u:
                        goto tr193;
                    case 61u:
                        goto tr199;
                    case 62u:
                        goto tr200;
                    case 63u:
                        goto tr193;
                    case 94u:
                        goto tr201;
                    case 95u:
                        goto st69;
                    case 126u:
                        goto st69;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr194;
                    } else if ((*p) >= 9u)
                        goto tr192;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st69;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st69;
                    } else
                        goto st69;
                } else
                    goto tr194;
                goto st0;
            tr194 :
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
#line 3601 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr202;
                    case 126u:
                        goto tr202;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr202;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr202;
                } else
                    goto tr202;
                goto st0;
            tr202 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st67;
            st67:
                if (++p == pe) goto _test_eof67;
            case 67:
#line 3623 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr203;
                    case 32u:
                        goto tr203;
                    case 33u:
                        goto tr204;
                    case 39u:
                        goto tr205;
                    case 40u:
                        goto tr206;
                    case 44u:
                        goto tr204;
                    case 59u:
                        goto tr204;
                    case 61u:
                        goto tr208;
                    case 62u:
                        goto tr209;
                    case 63u:
                        goto tr204;
                    case 94u:
                        goto tr210;
                    case 95u:
                        goto st67;
                    case 126u:
                        goto st67;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr203;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st67;
                    } else if ((*p) >= 65u)
                        goto st67;
                } else
                    goto st67;
                goto st0;
            tr197 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st68;
            st68:
                if (++p == pe) goto _test_eof68;
            case 68:
#line 3659 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr211;
                    case 126u:
                        goto tr211;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr211;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr211;
                } else
                    goto tr211;
                goto st0;
            tr191 :
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
            tr211 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st69;
            st69:
                if (++p == pe) goto _test_eof69;
            case 69:
#line 3692 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr192;
                    case 32u:
                        goto tr192;
                    case 33u:
                        goto tr193;
                    case 39u:
                        goto tr195;
                    case 40u:
                        goto tr196;
                    case 44u:
                        goto tr193;
                    case 59u:
                        goto tr193;
                    case 61u:
                        goto tr199;
                    case 62u:
                        goto tr200;
                    case 63u:
                        goto tr193;
                    case 94u:
                        goto tr201;
                    case 95u:
                        goto st69;
                    case 126u:
                        goto st69;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr194;
                    } else if ((*p) >= 9u)
                        goto tr192;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st69;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st69;
                    } else
                        goto st69;
                } else
                    goto tr194;
                goto st0;
            tr179 :
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
#line 3736 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr212;
                    case 126u:
                        goto tr212;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr212;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr212;
                } else
                    goto tr212;
                goto st0;
            tr212 :
#line 16 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st71;
            st71:
                if (++p == pe) goto _test_eof71;
            case 71:
#line 3758 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr213;
                    case 32u:
                        goto tr213;
                    case 33u:
                        goto tr214;
                    case 39u:
                        goto tr215;
                    case 40u:
                        goto tr216;
                    case 44u:
                        goto tr214;
                    case 58u:
                        goto tr218;
                    case 59u:
                        goto tr214;
                    case 61u:
                        goto tr219;
                    case 62u:
                        goto tr220;
                    case 63u:
                        goto tr214;
                    case 94u:
                        goto tr221;
                    case 95u:
                        goto st71;
                    case 126u:
                        goto st71;
                }
                if ((*p) < 48u) {
                    if (9u <= (*p) && (*p) <= 10u) goto tr213;
                } else if ((*p) > 57u) {
                    if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st71;
                    } else if ((*p) >= 65u)
                        goto st71;
                } else
                    goto st71;
                goto st0;
            tr182 :
#line 12 "ragel/././uuid-grammar.rl"
            {
                variety = *(p - 1);
            }
                goto st72;
            st72:
                if (++p == pe) goto _test_eof72;
            case 72:
#line 3795 "ron/text-parser.cc"
                switch ((*p)) {
                    case 95u:
                        goto tr222;
                    case 126u:
                        goto tr222;
                }
                if ((*p) < 65u) {
                    if (48u <= (*p) && (*p) <= 57u) goto tr222;
                } else if ((*p) > 90u) {
                    if (97u <= (*p) && (*p) <= 122u) goto tr222;
                } else
                    goto tr222;
                goto st0;
            tr176 :
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
            tr222 :
#line 13 "ragel/././uuid-grammar.rl"
            {
                wordb = p;
            }
                goto st73;
            st73:
                if (++p == pe) goto _test_eof73;
            case 73:
#line 3828 "ron/text-parser.cc"
                switch ((*p)) {
                    case 13u:
                        goto tr177;
                    case 32u:
                        goto tr177;
                    case 33u:
                        goto tr178;
                    case 39u:
                        goto tr180;
                    case 40u:
                        goto tr181;
                    case 44u:
                        goto tr178;
                    case 58u:
                        goto tr184;
                    case 59u:
                        goto tr178;
                    case 61u:
                        goto tr185;
                    case 62u:
                        goto tr186;
                    case 63u:
                        goto tr178;
                    case 94u:
                        goto tr187;
                    case 95u:
                        goto st73;
                    case 126u:
                        goto st73;
                }
                if ((*p) < 43u) {
                    if ((*p) > 10u) {
                        if (36u <= (*p) && (*p) <= 37u) goto tr179;
                    } else if ((*p) >= 9u)
                        goto tr177;
                } else if ((*p) > 45u) {
                    if ((*p) < 65u) {
                        if (48u <= (*p) && (*p) <= 57u) goto st73;
                    } else if ((*p) > 90u) {
                        if (97u <= (*p) && (*p) <= 122u) goto st73;
                    } else
                        goto st73;
                } else
                    goto tr179;
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

#line 76 "ragel/text-parser.rl"

    data().Consume(p - pb);

    // std::cerr << "ending with [" <<p<<"] state "<<cs<<" "<<op_.size()<<"
    // atoms "<<(pe-p)<<" bytes left, prev_id_ "<<prev_id_.str()<<'\n';

    if (term && cs != RON_error) {
        term_ = chr2term(term);  // FIXME gen a fn
        return Status::OK;
    } else if (cs >= RON_first_final) {
        cs = RON_error;
        return Status::ENDOFFRAME;
    } else {
        cs = RON_error;
        char msg[64];
        size_t msglen =
            sprintf((char *)msg, "syntax error at line %d col %d (offset %d)",
                    line, (int)(p - lineb), (int)(p - pb));
        return Status::BAD_STATE.comment(msg);
    }
}

}  // namespace ron
