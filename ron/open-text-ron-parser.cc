
#line 1 "ragel/open-parser.rl"
#include "open-text-ron.hpp"

namespace ron {


#line 6 "ragel/open-parser.rl"

#line 11 "ron/open-text-ron-parser.cc"
static const int RON_start = 33;
static const int RON_first_final = 33;
static const int RON_error = 0;

static const int RON_en_main = 33;


#line 7 "ragel/open-parser.rl"

bool OpenTextFrame::Cursor::_parse_int (iter from, iter till) {
}

bool OpenTextFrame::Cursor::_parse_float (iter from, iter till) {
}

bool OpenTextFrame::Cursor::_parse_uuid (iter from, iter varsion, iter till) {
}

bool OpenTextFrame::Cursor::_parse_string (iter from, iter till) {
}

bool OpenTextFrame::Cursor::_set_term (char term) {
    op_.term_ = (enum TERM) (ABC[term]);
}

bool OpenTextFrame::Cursor::_default_spec () {
}

bool OpenTextFrame::Cursor::Next () {

    Atoms& atoms = op_.atoms_;

    // hint: the machine must start parsing in the "space" state
    // to allow for frames like `1, 2, 3 or 'string'`

    switch (cs) {
        case RON_error:
            if (off_!=0) {
                return false;
            }
            
#line 53 "ron/open-text-ron-parser.cc"
	{
	cs = RON_start;
	}

#line 40 "ragel/open-parser.rl"
            atoms.resize(0);
            break;

        case RON_FULL_STOP:
            cs = RON_error;
            return false;

        default:
            atoms.resize(0);
            break;
    }

    if (data().size()<=off_) {
        cs = RON_error;
        return false;
    }

    iter pb = data().begin();
    iter p = pb + off_;
    iter pe = data().end();
    iter eof = pe;

    iter int_start{pb};
    iter uuid_start{pb};
    iter float_start{pb};
    iter string_start{pb};
    iter version_at{pb};

    
#line 88 "ron/open-text-ron-parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 33:
	switch( (*p) ) {
		case 32: goto tr84;
		case 33: goto tr85;
		case 39: goto tr86;
		case 44: goto tr85;
		case 46: goto st35;
		case 59: goto tr85;
		case 61: goto tr88;
		case 62: goto tr89;
		case 63: goto tr85;
		case 64: goto st36;
		case 94: goto tr90;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr84;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st1;
tr84:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st1;
tr91:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 1; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 136 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr0;
		case 33: goto tr2;
		case 39: goto tr4;
		case 44: goto tr2;
		case 59: goto tr2;
		case 61: goto tr7;
		case 62: goto tr8;
		case 63: goto tr2;
		case 64: goto st36;
		case 94: goto tr11;
		case 95: goto tr10;
		case 126: goto tr10;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr3;
		} else if ( (*p) >= 9 )
			goto tr0;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr6;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr10;
		} else
			goto tr10;
	} else
		goto tr5;
	goto st0;
tr2:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
	goto st34;
tr17:
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
	goto st34;
tr23:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
	goto st34;
tr29:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
	goto st34;
tr39:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
	goto st34;
tr52:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
	goto st34;
tr62:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
	goto st34;
tr85:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
	goto st34;
tr92:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 34; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
	goto st34;
tr100:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 34; goto _out;} }
	goto st34;
tr110:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 34; goto _out;} }
	goto st34;
tr118:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 34; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st34;
tr127:
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 34; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st34;
tr133:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 34; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st34;
tr141:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 34; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st34;
tr149:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 34; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st34;
tr159:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 34; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st34;
tr168:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 13 "ragel/./open-grammar.rl"
	{ _set_term((*p)); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 34; goto _out;} }
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 331 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr91;
		case 33: goto tr92;
		case 39: goto tr93;
		case 44: goto tr92;
		case 46: goto tr94;
		case 59: goto tr92;
		case 61: goto tr95;
		case 62: goto tr96;
		case 63: goto tr92;
		case 64: goto tr97;
		case 94: goto tr98;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr91;
	goto st0;
tr4:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st2;
tr24:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st2;
tr30:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st2;
tr40:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
	goto st2;
tr53:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
	goto st2;
tr63:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st2;
tr86:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st2;
tr93:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 2; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 394 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto st0;
		case 34: goto st0;
		case 39: goto tr13;
		case 92: goto st0;
	}
	goto tr12;
tr12:
#line 11 "ragel/./open-grammar.rl"
	{ string_start = p; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 411 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto st0;
		case 34: goto st0;
		case 39: goto tr15;
		case 92: goto st0;
	}
	goto st3;
tr13:
#line 11 "ragel/./open-grammar.rl"
	{ string_start = p; }
#line 12 "ragel/./open-grammar.rl"
	{ _parse_string(string_start, p); }
	goto st4;
tr15:
#line 12 "ragel/./open-grammar.rl"
	{ _parse_string(string_start, p); }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 434 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto st5;
		case 33: goto tr17;
		case 39: goto st2;
		case 44: goto tr17;
		case 59: goto tr17;
		case 61: goto st7;
		case 62: goto st10;
		case 63: goto tr17;
		case 94: goto st12;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st5;
	goto st0;
tr22:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st5;
tr28:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st5;
tr38:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
	goto st5;
tr51:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
	goto st5;
tr61:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 477 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr22;
		case 33: goto tr23;
		case 39: goto tr24;
		case 44: goto tr23;
		case 59: goto tr23;
		case 61: goto tr25;
		case 62: goto tr26;
		case 63: goto tr23;
		case 94: goto tr27;
		case 95: goto tr10;
		case 126: goto tr10;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr3;
		} else if ( (*p) >= 9 )
			goto tr22;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr6;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr10;
		} else
			goto tr10;
	} else
		goto tr5;
	goto st0;
tr3:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 9 "ragel/./open-grammar.rl"
	{ version_at = p; }
	goto st6;
tr44:
#line 9 "ragel/./open-grammar.rl"
	{ version_at = p; }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 523 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr28;
		case 33: goto tr29;
		case 39: goto tr30;
		case 44: goto tr29;
		case 59: goto tr29;
		case 61: goto tr32;
		case 62: goto tr33;
		case 63: goto tr29;
		case 94: goto tr34;
		case 95: goto st6;
		case 126: goto st6;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr28;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st6;
		} else if ( (*p) >= 65 )
			goto st6;
	} else
		goto st6;
	goto st0;
tr7:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st7;
tr25:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st7;
tr32:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st7;
tr41:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
	goto st7;
tr54:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
	goto st7;
tr64:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st7;
tr88:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st7;
tr95:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 7; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 595 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto st7;
		case 43: goto tr35;
		case 45: goto tr35;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr36;
	} else if ( (*p) >= 9 )
		goto st7;
	goto st0;
tr35:
#line 6 "ragel/./open-grammar.rl"
	{ int_start = p; }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 615 "ron/open-text-ron-parser.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st9;
	goto st0;
tr36:
#line 6 "ragel/./open-grammar.rl"
	{ int_start = p; }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 627 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr38;
		case 33: goto tr39;
		case 39: goto tr40;
		case 44: goto tr39;
		case 59: goto tr39;
		case 61: goto tr41;
		case 62: goto tr42;
		case 63: goto tr39;
		case 94: goto tr43;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st9;
	} else if ( (*p) >= 9 )
		goto tr38;
	goto st0;
tr8:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st10;
tr26:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st10;
tr33:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st10;
tr42:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
	goto st10;
tr55:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
	goto st10;
tr65:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st10;
tr89:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st10;
tr96:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 10; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 691 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr22;
		case 33: goto tr23;
		case 39: goto tr24;
		case 44: goto tr23;
		case 59: goto tr23;
		case 61: goto tr25;
		case 62: goto tr26;
		case 63: goto tr23;
		case 94: goto tr27;
		case 95: goto tr10;
		case 126: goto tr10;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr3;
		} else if ( (*p) >= 9 )
			goto tr22;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr10;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr10;
		} else
			goto tr10;
	} else
		goto tr3;
	goto st0;
tr10:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 731 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr28;
		case 33: goto tr29;
		case 39: goto tr30;
		case 44: goto tr29;
		case 59: goto tr29;
		case 61: goto tr32;
		case 62: goto tr33;
		case 63: goto tr29;
		case 94: goto tr34;
		case 95: goto st11;
		case 126: goto st11;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr44;
		} else if ( (*p) >= 9 )
			goto tr28;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st11;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st11;
		} else
			goto st11;
	} else
		goto tr44;
	goto st0;
tr11:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st12;
tr27:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st12;
tr34:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st12;
tr43:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
	goto st12;
tr57:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
	goto st12;
tr66:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st12;
tr90:
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st12;
tr98:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 12; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 809 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto st12;
		case 43: goto tr46;
		case 45: goto tr46;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr47;
	} else if ( (*p) >= 9 )
		goto st12;
	goto st0;
tr46:
#line 14 "ragel/./open-grammar.rl"
	{ float_start = p; }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 829 "ron/open-text-ron-parser.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st14;
	goto st0;
tr47:
#line 14 "ragel/./open-grammar.rl"
	{ float_start = p; }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 841 "ron/open-text-ron-parser.cc"
	if ( (*p) == 46 )
		goto st15;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st14;
	goto st0;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st16;
	goto st0;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	switch( (*p) ) {
		case 32: goto tr51;
		case 33: goto tr52;
		case 39: goto tr53;
		case 44: goto tr52;
		case 59: goto tr52;
		case 61: goto tr54;
		case 62: goto tr55;
		case 63: goto tr52;
		case 69: goto st17;
		case 94: goto tr57;
		case 101: goto st17;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st16;
	} else if ( (*p) >= 9 )
		goto tr51;
	goto st0;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	switch( (*p) ) {
		case 43: goto st18;
		case 45: goto st18;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st19;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st19;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	switch( (*p) ) {
		case 32: goto tr51;
		case 33: goto tr52;
		case 39: goto tr53;
		case 44: goto tr52;
		case 59: goto tr52;
		case 61: goto tr54;
		case 62: goto tr55;
		case 63: goto tr52;
		case 94: goto tr57;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st19;
	} else if ( (*p) >= 9 )
		goto tr51;
	goto st0;
tr5:
#line 6 "ragel/./open-grammar.rl"
	{ int_start = p; }
#line 14 "ragel/./open-grammar.rl"
	{ float_start = p; }
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 9 "ragel/./open-grammar.rl"
	{ version_at = p; }
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 930 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr28;
		case 33: goto tr29;
		case 39: goto tr30;
		case 44: goto tr29;
		case 59: goto tr29;
		case 61: goto tr32;
		case 62: goto tr33;
		case 63: goto tr29;
		case 94: goto tr34;
		case 95: goto st6;
		case 126: goto st6;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr28;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st6;
		} else if ( (*p) >= 65 )
			goto st6;
	} else
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 32: goto tr61;
		case 33: goto tr62;
		case 39: goto tr63;
		case 44: goto tr62;
		case 46: goto st15;
		case 59: goto tr62;
		case 61: goto tr64;
		case 62: goto tr65;
		case 63: goto tr62;
		case 94: goto tr66;
		case 95: goto st6;
		case 126: goto st6;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr61;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st6;
		} else if ( (*p) >= 65 )
			goto st6;
	} else
		goto st21;
	goto st0;
tr6:
#line 6 "ragel/./open-grammar.rl"
	{ int_start = p; }
#line 14 "ragel/./open-grammar.rl"
	{ float_start = p; }
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 998 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr61;
		case 33: goto tr62;
		case 39: goto tr63;
		case 44: goto tr62;
		case 46: goto st15;
		case 59: goto tr62;
		case 61: goto tr64;
		case 62: goto tr65;
		case 63: goto tr62;
		case 94: goto tr66;
		case 95: goto st11;
		case 126: goto st11;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr44;
		} else if ( (*p) >= 9 )
			goto tr61;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st22;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st11;
		} else
			goto st11;
	} else
		goto tr44;
	goto st0;
tr94:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 35; goto _out;} }
	goto st35;
tr103:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 35; goto _out;} }
	goto st35;
tr113:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 35; goto _out;} }
	goto st35;
tr120:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 35; goto _out;} }
	goto st35;
tr135:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 35; goto _out;} }
	goto st35;
tr151:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 35; goto _out;} }
	goto st35;
tr170:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 35; goto _out;} }
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 1083 "ron/open-text-ron-parser.cc"
	goto st0;
tr97:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 36; goto _out;} }
	goto st36;
tr108:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 36; goto _out;} }
	goto st36;
tr115:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 36; goto _out;} }
	goto st36;
tr124:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 36; goto _out;} }
	goto st36;
tr138:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 36; goto _out;} }
	goto st36;
tr154:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 36; goto _out;} }
	goto st36;
tr164:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 36; goto _out;} }
	goto st36;
tr175:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 36; goto _out;} }
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 1145 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr99;
		case 33: goto tr100;
		case 39: goto tr102;
		case 44: goto tr100;
		case 46: goto tr103;
		case 58: goto tr105;
		case 59: goto tr100;
		case 61: goto tr106;
		case 62: goto tr107;
		case 63: goto tr100;
		case 64: goto tr108;
		case 94: goto tr109;
		case 95: goto tr104;
		case 126: goto tr104;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr101;
		} else if ( (*p) >= 9 )
			goto tr99;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr104;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr104;
		} else
			goto tr104;
	} else
		goto tr101;
	goto st0;
tr126:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 37; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st37;
tr99:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 37; goto _out;} }
	goto st37;
tr117:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 37; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st37;
tr132:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 37; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st37;
tr140:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 37; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st37;
tr148:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 37; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st37;
tr158:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 37; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st37;
tr167:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 37; goto _out;} }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 1252 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr99;
		case 33: goto tr110;
		case 39: goto tr102;
		case 44: goto tr110;
		case 46: goto tr113;
		case 59: goto tr110;
		case 61: goto tr106;
		case 62: goto tr107;
		case 63: goto tr110;
		case 64: goto tr115;
		case 94: goto tr109;
		case 95: goto tr116;
		case 126: goto tr116;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr111;
		} else if ( (*p) >= 9 )
			goto tr99;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr114;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr116;
		} else
			goto tr116;
	} else
		goto tr112;
	goto st0;
tr111:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 9 "ragel/./open-grammar.rl"
	{ version_at = p; }
	goto st38;
tr146:
#line 9 "ragel/./open-grammar.rl"
	{ version_at = p; }
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 1300 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr117;
		case 33: goto tr118;
		case 39: goto tr119;
		case 44: goto tr118;
		case 46: goto tr120;
		case 59: goto tr118;
		case 61: goto tr122;
		case 62: goto tr123;
		case 63: goto tr118;
		case 64: goto tr124;
		case 94: goto tr125;
		case 95: goto st38;
		case 126: goto st38;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr117;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st38;
		} else if ( (*p) >= 65 )
			goto st38;
	} else
		goto st38;
	goto st0;
tr128:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 23; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st23;
tr102:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 23; goto _out;} }
	goto st23;
tr119:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 23; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st23;
tr134:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 23; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st23;
tr142:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 23; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st23;
tr150:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 23; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st23;
tr160:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 23; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st23;
tr169:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 23; goto _out;} }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 1400 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto st0;
		case 34: goto st0;
		case 39: goto tr69;
		case 92: goto st0;
	}
	goto tr68;
tr68:
#line 11 "ragel/./open-grammar.rl"
	{ string_start = p; }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 1417 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto st0;
		case 34: goto st0;
		case 39: goto tr71;
		case 92: goto st0;
	}
	goto st24;
tr69:
#line 11 "ragel/./open-grammar.rl"
	{ string_start = p; }
#line 12 "ragel/./open-grammar.rl"
	{ _parse_string(string_start, p); }
	goto st39;
tr71:
#line 12 "ragel/./open-grammar.rl"
	{ _parse_string(string_start, p); }
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 1440 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr126;
		case 33: goto tr127;
		case 39: goto tr128;
		case 44: goto tr127;
		case 46: goto tr94;
		case 59: goto tr127;
		case 61: goto tr129;
		case 62: goto tr130;
		case 63: goto tr127;
		case 64: goto tr97;
		case 94: goto tr131;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr126;
	goto st0;
tr129:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 25; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st25;
tr106:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 25; goto _out;} }
	goto st25;
tr122:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 25; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st25;
tr136:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 25; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st25;
tr143:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 25; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st25;
tr152:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 25; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st25;
tr162:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 25; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st25;
tr173:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 25; goto _out;} }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 1529 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto st25;
		case 43: goto tr73;
		case 45: goto tr73;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr74;
	} else if ( (*p) >= 9 )
		goto st25;
	goto st0;
tr73:
#line 6 "ragel/./open-grammar.rl"
	{ int_start = p; }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 1549 "ron/open-text-ron-parser.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st40;
	goto st0;
tr74:
#line 6 "ragel/./open-grammar.rl"
	{ int_start = p; }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 1561 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr132;
		case 33: goto tr133;
		case 39: goto tr134;
		case 44: goto tr133;
		case 46: goto tr135;
		case 59: goto tr133;
		case 61: goto tr136;
		case 62: goto tr137;
		case 63: goto tr133;
		case 64: goto tr138;
		case 94: goto tr139;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st40;
	} else if ( (*p) >= 9 )
		goto tr132;
	goto st0;
tr130:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 41; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st41;
tr107:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 41; goto _out;} }
	goto st41;
tr123:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 41; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st41;
tr137:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 41; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st41;
tr144:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 41; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st41;
tr153:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 41; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st41;
tr163:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 41; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st41;
tr174:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 41; goto _out;} }
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
#line 1653 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr140;
		case 33: goto tr141;
		case 39: goto tr142;
		case 44: goto tr141;
		case 46: goto tr113;
		case 59: goto tr141;
		case 61: goto tr143;
		case 62: goto tr144;
		case 63: goto tr141;
		case 64: goto tr115;
		case 94: goto tr145;
		case 95: goto tr116;
		case 126: goto tr116;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr111;
		} else if ( (*p) >= 9 )
			goto tr140;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr116;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr116;
		} else
			goto tr116;
	} else
		goto tr111;
	goto st0;
tr116:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
	goto st42;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
#line 1695 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr117;
		case 33: goto tr118;
		case 39: goto tr119;
		case 44: goto tr118;
		case 46: goto tr120;
		case 59: goto tr118;
		case 61: goto tr122;
		case 62: goto tr123;
		case 63: goto tr118;
		case 64: goto tr124;
		case 94: goto tr125;
		case 95: goto st42;
		case 126: goto st42;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr146;
		} else if ( (*p) >= 9 )
			goto tr117;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st42;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st42;
		} else
			goto st42;
	} else
		goto tr146;
	goto st0;
tr131:
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 27; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st27;
tr109:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 27; goto _out;} }
	goto st27;
tr125:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 27; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st27;
tr139:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 27; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st27;
tr145:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 27; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st27;
tr156:
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 27; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st27;
tr165:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 27; goto _out;} }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
	goto st27;
tr176:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 27; goto _out;} }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 1801 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto st27;
		case 43: goto tr77;
		case 45: goto tr77;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr78;
	} else if ( (*p) >= 9 )
		goto st27;
	goto st0;
tr77:
#line 14 "ragel/./open-grammar.rl"
	{ float_start = p; }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 1821 "ron/open-text-ron-parser.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st29;
	goto st0;
tr78:
#line 14 "ragel/./open-grammar.rl"
	{ float_start = p; }
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 1833 "ron/open-text-ron-parser.cc"
	if ( (*p) == 46 )
		goto st30;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st29;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st43;
	goto st0;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
	switch( (*p) ) {
		case 32: goto tr148;
		case 33: goto tr149;
		case 39: goto tr150;
		case 44: goto tr149;
		case 46: goto tr151;
		case 59: goto tr149;
		case 61: goto tr152;
		case 62: goto tr153;
		case 63: goto tr149;
		case 64: goto tr154;
		case 69: goto st31;
		case 94: goto tr156;
		case 101: goto st31;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st43;
	} else if ( (*p) >= 9 )
		goto tr148;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 43: goto st32;
		case 45: goto st32;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st44;
	goto st0;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st44;
	goto st0;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
	switch( (*p) ) {
		case 32: goto tr148;
		case 33: goto tr149;
		case 39: goto tr150;
		case 44: goto tr149;
		case 46: goto tr151;
		case 59: goto tr149;
		case 61: goto tr152;
		case 62: goto tr153;
		case 63: goto tr149;
		case 64: goto tr154;
		case 94: goto tr156;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st44;
	} else if ( (*p) >= 9 )
		goto tr148;
	goto st0;
tr112:
#line 6 "ragel/./open-grammar.rl"
	{ int_start = p; }
#line 14 "ragel/./open-grammar.rl"
	{ float_start = p; }
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 9 "ragel/./open-grammar.rl"
	{ version_at = p; }
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
#line 1926 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr117;
		case 33: goto tr118;
		case 39: goto tr119;
		case 44: goto tr118;
		case 46: goto tr120;
		case 59: goto tr118;
		case 61: goto tr122;
		case 62: goto tr123;
		case 63: goto tr118;
		case 64: goto tr124;
		case 94: goto tr125;
		case 95: goto st38;
		case 126: goto st38;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr117;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st38;
		} else if ( (*p) >= 65 )
			goto st38;
	} else
		goto st46;
	goto st0;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
	switch( (*p) ) {
		case 32: goto tr158;
		case 33: goto tr159;
		case 39: goto tr160;
		case 44: goto tr159;
		case 46: goto tr161;
		case 59: goto tr159;
		case 61: goto tr162;
		case 62: goto tr163;
		case 63: goto tr159;
		case 64: goto tr164;
		case 94: goto tr165;
		case 95: goto st38;
		case 126: goto st38;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr158;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st38;
		} else if ( (*p) >= 65 )
			goto st38;
	} else
		goto st46;
	goto st0;
tr161:
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 47; goto _out;} }
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 1997 "ron/open-text-ron-parser.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st43;
	goto st0;
tr114:
#line 6 "ragel/./open-grammar.rl"
	{ int_start = p; }
#line 14 "ragel/./open-grammar.rl"
	{ float_start = p; }
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 2013 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr158;
		case 33: goto tr159;
		case 39: goto tr160;
		case 44: goto tr159;
		case 46: goto tr161;
		case 59: goto tr159;
		case 61: goto tr162;
		case 62: goto tr163;
		case 63: goto tr159;
		case 64: goto tr164;
		case 94: goto tr165;
		case 95: goto st42;
		case 126: goto st42;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr146;
		} else if ( (*p) >= 9 )
			goto tr158;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st48;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st42;
		} else
			goto st42;
	} else
		goto tr146;
	goto st0;
tr101:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 9 "ragel/./open-grammar.rl"
	{ version_at = p; }
	goto st49;
tr182:
#line 9 "ragel/./open-grammar.rl"
	{ version_at = p; }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 2061 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr167;
		case 33: goto tr168;
		case 39: goto tr169;
		case 44: goto tr168;
		case 46: goto tr170;
		case 58: goto tr172;
		case 59: goto tr168;
		case 61: goto tr173;
		case 62: goto tr174;
		case 63: goto tr168;
		case 64: goto tr175;
		case 94: goto tr176;
		case 95: goto st49;
		case 126: goto st49;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr167;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st49;
		} else if ( (*p) >= 65 )
			goto st49;
	} else
		goto st49;
	goto st0;
tr105:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st50;
tr172:
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 2104 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr99;
		case 33: goto tr100;
		case 39: goto tr102;
		case 44: goto tr100;
		case 46: goto tr103;
		case 59: goto tr100;
		case 61: goto tr106;
		case 62: goto tr107;
		case 63: goto tr100;
		case 64: goto tr108;
		case 94: goto tr109;
		case 95: goto tr178;
		case 126: goto tr178;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr177;
		} else if ( (*p) >= 9 )
			goto tr99;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr178;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr178;
		} else
			goto tr178;
	} else
		goto tr177;
	goto st0;
tr177:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 9 "ragel/./open-grammar.rl"
	{ version_at = p; }
	goto st51;
tr180:
#line 9 "ragel/./open-grammar.rl"
	{ version_at = p; }
	goto st51;
st51:
	if ( ++p == pe )
		goto _test_eof51;
case 51:
#line 2152 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr167;
		case 33: goto tr168;
		case 39: goto tr169;
		case 44: goto tr168;
		case 46: goto tr170;
		case 59: goto tr168;
		case 61: goto tr173;
		case 62: goto tr174;
		case 63: goto tr168;
		case 64: goto tr175;
		case 94: goto tr176;
		case 95: goto st51;
		case 126: goto st51;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr167;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st51;
		} else if ( (*p) >= 65 )
			goto st51;
	} else
		goto st51;
	goto st0;
tr178:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
	goto st52;
st52:
	if ( ++p == pe )
		goto _test_eof52;
case 52:
#line 2188 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr167;
		case 33: goto tr168;
		case 39: goto tr169;
		case 44: goto tr168;
		case 46: goto tr170;
		case 59: goto tr168;
		case 61: goto tr173;
		case 62: goto tr174;
		case 63: goto tr168;
		case 64: goto tr175;
		case 94: goto tr176;
		case 95: goto st52;
		case 126: goto st52;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr180;
		} else if ( (*p) >= 9 )
			goto tr167;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st52;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st52;
		} else
			goto st52;
	} else
		goto tr180;
	goto st0;
tr104:
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
	goto st53;
st53:
	if ( ++p == pe )
		goto _test_eof53;
case 53:
#line 2230 "ron/open-text-ron-parser.cc"
	switch( (*p) ) {
		case 32: goto tr167;
		case 33: goto tr168;
		case 39: goto tr169;
		case 44: goto tr168;
		case 46: goto tr170;
		case 58: goto tr172;
		case 59: goto tr168;
		case 61: goto tr173;
		case 62: goto tr174;
		case 63: goto tr168;
		case 64: goto tr175;
		case 94: goto tr176;
		case 95: goto st53;
		case 126: goto st53;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr182;
		} else if ( (*p) >= 9 )
			goto tr167;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st53;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st53;
		} else
			goto st53;
	} else
		goto tr182;
	goto st0;
	}
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof51: cs = 51; goto _test_eof; 
	_test_eof52: cs = 52; goto _test_eof; 
	_test_eof53: cs = 53; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 34: 
	case 39: 
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 0; goto _out;} }
	break;
	case 40: 
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 0; goto _out;} }
	break;
	case 38: 
	case 42: 
	case 45: 
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 0; goto _out;} }
	break;
	case 43: 
	case 44: 
#line 15 "ragel/./open-grammar.rl"
	{ _parse_float(float_start, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 0; goto _out;} }
	break;
	case 46: 
	case 48: 
#line 7 "ragel/./open-grammar.rl"
	{ _parse_int(int_start, p); }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 0; goto _out;} }
	break;
	case 37: 
	case 41: 
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 0; goto _out;} }
	break;
	case 49: 
	case 51: 
	case 52: 
	case 53: 
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 0; goto _out;} }
	break;
	case 36: 
	case 50: 
#line 8 "ragel/./open-grammar.rl"
	{ uuid_start = p; version_at = pb; }
#line 10 "ragel/./open-grammar.rl"
	{ _parse_uuid(uuid_start, version_at, p); }
#line 5 "ragel/./open-grammar.rl"
	{ _default_spec(); }
#line 16 "ragel/./open-grammar.rl"
	{ {p++; cs = 0; goto _out;} }
	break;
#line 2389 "ron/open-text-ron-parser.cc"
	}
	}

	_out: {}
	}

#line 72 "ragel/open-parser.rl"


    off_ = p-pb;
    pos_++;

    if (cs==RON_error) {
	    return false;
    } else if (cs>=33) { // one of end states
        if (p>=eof) {
            // in the block mode, the final dot is optional/implied
            cs = RON_FULL_STOP;
        }
        return true;
    } else if (cs==RON_FULL_STOP) {
        return true; // explicit dot
    } else if (p>=eof) {
        cs = RON_error;
	    return false;
    }
}

}
