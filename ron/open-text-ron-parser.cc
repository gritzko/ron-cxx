
#include "open-text-ron.hpp"
#include <iostream>

namespace ron {



static const int RON_start = 21;
static const int RON_first_final = 21;
static const int RON_error = 0;

static const int RON_en_main = 21;



bool OpenTextFrame::Cursor::_parse_int (iter from, iter till) {
    std::cerr<<"I see int '"<<std::string{from, till}<<"'\n"; 
}

bool OpenTextFrame::Cursor::_parse_float (iter from, iter till) {
    std::cerr<<"I see float '"<<std::string{from, till}<<"'\n"; 
}

bool OpenTextFrame::Cursor::_parse_uuid (iter from, iter varsion, iter till) {
    std::cerr<<"I see UUID '"<<std::string{from, till}<<"'\n"; 
}

bool OpenTextFrame::Cursor::_parse_string (iter from, iter till) {
    std::cerr<<"I see string '"<<std::string{from, till}<<"'\n"; 
}

bool OpenTextFrame::Cursor::_set_term (char term) {
    op_.term_ = (enum TERM) (ABC[term]);
    std::cerr<<"I see term "<<term<<"\n"; 
}

bool OpenTextFrame::Cursor::_parse_spec () {
    std::cerr<<"I saw spec\n"; 
}

bool OpenTextFrame::Cursor::_parse_op () {
    std::cerr<<"I saw op\n\n"; 
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
            
	{
	cs = RON_start;
	}

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

    iter int_start{p};
    iter uuid_start{p};
    iter float_start{p};
    iter string_start{p};
    iter version_at{p};

    std::cerr<<"starting with '"<<*p<<"'\n";

    
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 21:
	switch( (*p) ) {
		case 32: goto st1;
		case 39: goto st2;
		case 43: goto tr3;
		case 45: goto tr3;
		case 46: goto st6;
		case 61: goto st8;
		case 62: goto st10;
		case 64: goto st11;
		case 94: goto st15;
		case 95: goto tr8;
		case 126: goto tr8;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st1;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else if ( (*p) >= 65 )
			goto tr8;
	} else
		goto tr4;
	goto st0;
st0:
cs = 0;
	goto _out;
tr48:
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 1; goto _out;} } }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
	switch( (*p) ) {
		case 32: goto st1;
		case 39: goto st2;
		case 43: goto tr3;
		case 45: goto tr3;
		case 61: goto st8;
		case 62: goto st10;
		case 64: goto st11;
		case 94: goto st15;
		case 95: goto tr8;
		case 126: goto tr8;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st1;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else if ( (*p) >= 65 )
			goto tr8;
	} else
		goto tr4;
	goto st0;
tr49:
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 2; goto _out;} } }
	goto st2;
tr41:
	{ _parse_int(int_start, p); }
	goto st2;
tr56:
	{ _parse_int(int_start, p); }
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st2;
tr65:
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st2;
tr76:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_spec(); }
	goto st2;
tr89:
	{ _parse_float(float_start, p); }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto st0;
		case 34: goto st0;
		case 39: goto tr11;
		case 92: goto st0;
	}
	goto tr10;
tr10:
	{ string_start = p; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto st0;
		case 34: goto st0;
		case 39: goto tr13;
		case 92: goto st0;
	}
	goto st3;
tr11:
	{ string_start = p; }
	{ _parse_string(string_start, p); }
	goto st22;
tr13:
	{ _parse_string(string_start, p); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
	switch( (*p) ) {
		case 32: goto st4;
		case 33: goto tr33;
		case 39: goto st2;
		case 44: goto tr33;
		case 46: goto tr35;
		case 59: goto tr33;
		case 61: goto st8;
		case 62: goto st10;
		case 63: goto tr33;
		case 64: goto tr37;
		case 94: goto st15;
		case 95: goto tr38;
		case 126: goto tr38;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr34;
		} else if ( (*p) >= 9 )
			goto st4;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr38;
		} else if ( (*p) >= 65 )
			goto tr38;
	} else
		goto tr36;
	goto st0;
tr39:
	{ _parse_int(int_start, p); }
	goto st4;
tr53:
	{ _parse_int(int_start, p); }
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st4;
tr63:
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st4;
tr73:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_spec(); }
	goto st4;
tr87:
	{ _parse_float(float_start, p); }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
	switch( (*p) ) {
		case 32: goto st4;
		case 39: goto st2;
		case 43: goto tr3;
		case 45: goto tr3;
		case 61: goto st8;
		case 62: goto st10;
		case 94: goto st15;
		case 95: goto tr8;
		case 126: goto tr8;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st4;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else if ( (*p) >= 65 )
			goto tr8;
	} else
		goto tr4;
	goto st0;
tr3:
	{ int_start = p; }
	{ float_start = p; }
	goto st5;
tr34:
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 5; goto _out;} } }
	{ int_start = p; }
	{ float_start = p; }
	goto st5;
tr42:
	{ _parse_int(int_start, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 5; goto _out;} } }
	{ int_start = p; }
	{ float_start = p; }
	goto st5;
tr66:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 5; goto _out;} } }
	{ int_start = p; }
	{ float_start = p; }
	goto st5;
tr84:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_spec(); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 5; goto _out;} } }
	{ int_start = p; }
	{ float_start = p; }
	goto st5;
tr90:
	{ _parse_float(float_start, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 5; goto _out;} } }
	{ int_start = p; }
	{ float_start = p; }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st23;
	goto st0;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
	switch( (*p) ) {
		case 32: goto tr39;
		case 33: goto tr40;
		case 39: goto tr41;
		case 44: goto tr40;
		case 46: goto st18;
		case 59: goto tr40;
		case 61: goto tr43;
		case 62: goto tr44;
		case 63: goto tr40;
		case 64: goto tr45;
		case 94: goto tr47;
		case 95: goto tr46;
		case 126: goto tr46;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr42;
		} else if ( (*p) >= 9 )
			goto tr39;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr46;
		} else if ( (*p) >= 65 )
			goto tr46;
	} else
		goto st23;
	goto st0;
tr33:
	{ _set_term((*p)); }
	goto st24;
tr40:
	{ _parse_int(int_start, p); }
	{ _set_term((*p)); }
	goto st24;
tr54:
	{ _parse_int(int_start, p); }
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _set_term((*p)); }
	goto st24;
tr64:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _set_term((*p)); }
	goto st24;
tr74:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_spec(); }
	{ _set_term((*p)); }
	goto st24;
tr88:
	{ _parse_float(float_start, p); }
	{ _set_term((*p)); }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
	switch( (*p) ) {
		case 32: goto tr48;
		case 39: goto tr49;
		case 43: goto tr34;
		case 45: goto tr34;
		case 46: goto tr35;
		case 61: goto tr50;
		case 62: goto tr51;
		case 64: goto tr37;
		case 94: goto tr52;
		case 95: goto tr38;
		case 126: goto tr38;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr48;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr38;
		} else if ( (*p) >= 65 )
			goto tr38;
	} else
		goto tr36;
	goto st0;
tr35:
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 6; goto _out;} } }
	goto st6;
tr72:
	{ _parse_int(int_start, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 6; goto _out;} } }
	goto st6;
tr67:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 6; goto _out;} } }
	goto st6;
tr77:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_spec(); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 6; goto _out;} } }
	goto st6;
tr91:
	{ _parse_float(float_start, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 6; goto _out;} } }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	if ( (*p) == 10 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	goto st0;
tr4:
	{ int_start = p; }
	{ float_start = p; }
	{ uuid_start = p; version_at = pb; }
	goto st26;
tr36:
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 26; goto _out;} } }
	{ int_start = p; }
	{ float_start = p; }
	{ uuid_start = p; version_at = pb; }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 32: goto tr53;
		case 33: goto tr54;
		case 39: goto tr56;
		case 44: goto tr54;
		case 46: goto st18;
		case 59: goto tr54;
		case 61: goto tr58;
		case 62: goto tr59;
		case 63: goto tr54;
		case 64: goto tr60;
		case 94: goto tr62;
		case 95: goto st29;
		case 126: goto st29;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr55;
		} else if ( (*p) >= 9 )
			goto tr53;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st26;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st29;
		} else
			goto st29;
	} else
		goto tr55;
	goto st0;
tr55:
	{ version_at = p; }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	switch( (*p) ) {
		case 95: goto st27;
		case 126: goto st27;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st27;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st27;
	} else
		goto st27;
	goto st0;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
	switch( (*p) ) {
		case 32: goto tr63;
		case 33: goto tr64;
		case 39: goto tr65;
		case 44: goto tr64;
		case 46: goto tr67;
		case 59: goto tr64;
		case 61: goto tr68;
		case 62: goto tr69;
		case 63: goto tr64;
		case 64: goto tr70;
		case 94: goto tr71;
		case 95: goto st27;
		case 126: goto st27;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr66;
		} else if ( (*p) >= 9 )
			goto tr63;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st27;
		} else if ( (*p) >= 65 )
			goto st27;
	} else
		goto st27;
	goto st0;
tr50:
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 8; goto _out;} } }
	goto st8;
tr43:
	{ _parse_int(int_start, p); }
	goto st8;
tr58:
	{ _parse_int(int_start, p); }
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st8;
tr68:
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st8;
tr80:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_spec(); }
	goto st8;
tr92:
	{ _parse_float(float_start, p); }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 32: goto st8;
		case 43: goto tr18;
		case 45: goto tr18;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr19;
	} else if ( (*p) >= 9 )
		goto st8;
	goto st0;
tr18:
	{ int_start = p; }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st28;
	goto st0;
tr19:
	{ int_start = p; }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
	switch( (*p) ) {
		case 32: goto tr39;
		case 33: goto tr40;
		case 39: goto tr41;
		case 44: goto tr40;
		case 46: goto tr72;
		case 59: goto tr40;
		case 61: goto tr43;
		case 62: goto tr44;
		case 63: goto tr40;
		case 64: goto tr45;
		case 94: goto tr47;
		case 95: goto tr46;
		case 126: goto tr46;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr42;
		} else if ( (*p) >= 9 )
			goto tr39;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr46;
		} else if ( (*p) >= 65 )
			goto tr46;
	} else
		goto st28;
	goto st0;
tr51:
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 10; goto _out;} } }
	goto st10;
tr44:
	{ _parse_int(int_start, p); }
	goto st10;
tr59:
	{ _parse_int(int_start, p); }
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st10;
tr69:
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st10;
tr81:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_spec(); }
	goto st10;
tr93:
	{ _parse_float(float_start, p); }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	switch( (*p) ) {
		case 32: goto st10;
		case 95: goto tr8;
		case 126: goto tr8;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto st10;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr8;
		} else if ( (*p) >= 65 )
			goto tr8;
	} else
		goto tr8;
	goto st0;
tr8:
	{ uuid_start = p; version_at = pb; }
	goto st29;
tr38:
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 29; goto _out;} } }
	{ uuid_start = p; version_at = pb; }
	goto st29;
tr46:
	{ _parse_int(int_start, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 29; goto _out;} } }
	{ uuid_start = p; version_at = pb; }
	goto st29;
tr95:
	{ _parse_float(float_start, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 29; goto _out;} } }
	{ uuid_start = p; version_at = pb; }
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 32: goto tr63;
		case 33: goto tr64;
		case 39: goto tr65;
		case 44: goto tr64;
		case 46: goto tr67;
		case 59: goto tr64;
		case 61: goto tr68;
		case 62: goto tr69;
		case 63: goto tr64;
		case 64: goto tr70;
		case 94: goto tr71;
		case 95: goto st29;
		case 126: goto st29;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr55;
		} else if ( (*p) >= 9 )
			goto tr63;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st29;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st29;
		} else
			goto st29;
	} else
		goto tr55;
	goto st0;
tr37:
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 11; goto _out;} } }
	goto st11;
tr45:
	{ _parse_int(int_start, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 11; goto _out;} } }
	goto st11;
tr60:
	{ _parse_int(int_start, p); }
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 11; goto _out;} } }
	goto st11;
tr70:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 11; goto _out;} } }
	goto st11;
tr82:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_spec(); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 11; goto _out;} } }
	goto st11;
tr94:
	{ _parse_float(float_start, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 11; goto _out;} } }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 95: goto tr21;
		case 126: goto tr21;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr21;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr21;
	} else
		goto tr21;
	goto st0;
tr21:
	{ uuid_start = p; version_at = pb; }
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 32: goto tr73;
		case 33: goto tr74;
		case 39: goto tr76;
		case 44: goto tr74;
		case 46: goto tr77;
		case 58: goto tr79;
		case 59: goto tr74;
		case 61: goto tr80;
		case 62: goto tr81;
		case 63: goto tr74;
		case 64: goto tr82;
		case 94: goto tr83;
		case 95: goto st30;
		case 126: goto st30;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr75;
		} else if ( (*p) >= 9 )
			goto tr73;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st30;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st30;
		} else
			goto st30;
	} else
		goto tr75;
	goto st0;
tr75:
	{ version_at = p; }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	switch( (*p) ) {
		case 95: goto st31;
		case 126: goto st31;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st31;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st31;
	} else
		goto st31;
	goto st0;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
	switch( (*p) ) {
		case 32: goto tr73;
		case 33: goto tr74;
		case 39: goto tr76;
		case 44: goto tr74;
		case 46: goto tr77;
		case 58: goto tr79;
		case 59: goto tr74;
		case 61: goto tr80;
		case 62: goto tr81;
		case 63: goto tr74;
		case 64: goto tr82;
		case 94: goto tr83;
		case 95: goto st31;
		case 126: goto st31;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr84;
		} else if ( (*p) >= 9 )
			goto tr73;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st31;
		} else if ( (*p) >= 65 )
			goto st31;
	} else
		goto st31;
	goto st0;
tr79:
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
	switch( (*p) ) {
		case 95: goto tr23;
		case 126: goto tr23;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr23;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr23;
	} else
		goto tr23;
	goto st0;
tr23:
	{ uuid_start = p; version_at = pb; }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
	switch( (*p) ) {
		case 32: goto tr73;
		case 33: goto tr74;
		case 39: goto tr76;
		case 44: goto tr74;
		case 46: goto tr77;
		case 59: goto tr74;
		case 61: goto tr80;
		case 62: goto tr81;
		case 63: goto tr74;
		case 64: goto tr82;
		case 94: goto tr83;
		case 95: goto st32;
		case 126: goto st32;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr85;
		} else if ( (*p) >= 9 )
			goto tr73;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st32;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st32;
		} else
			goto st32;
	} else
		goto tr85;
	goto st0;
tr85:
	{ version_at = p; }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 95: goto st33;
		case 126: goto st33;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st33;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st33;
	} else
		goto st33;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 32: goto tr73;
		case 33: goto tr74;
		case 39: goto tr76;
		case 44: goto tr74;
		case 46: goto tr77;
		case 59: goto tr74;
		case 61: goto tr80;
		case 62: goto tr81;
		case 63: goto tr74;
		case 64: goto tr82;
		case 94: goto tr83;
		case 95: goto st33;
		case 126: goto st33;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr84;
		} else if ( (*p) >= 9 )
			goto tr73;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st33;
		} else if ( (*p) >= 65 )
			goto st33;
	} else
		goto st33;
	goto st0;
tr52:
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 15; goto _out;} } }
	goto st15;
tr47:
	{ _parse_int(int_start, p); }
	goto st15;
tr62:
	{ _parse_int(int_start, p); }
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st15;
tr71:
	{ _parse_uuid(uuid_start, version_at, p); }
	goto st15;
tr83:
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_spec(); }
	goto st15;
tr97:
	{ _parse_float(float_start, p); }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
	switch( (*p) ) {
		case 32: goto st15;
		case 43: goto tr25;
		case 45: goto tr25;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr26;
	} else if ( (*p) >= 9 )
		goto st15;
	goto st0;
tr25:
	{ float_start = p; }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st17;
	goto st0;
tr26:
	{ float_start = p; }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
	if ( (*p) == 46 )
		goto st18;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st17;
	goto st0;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st34;
	goto st0;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
	switch( (*p) ) {
		case 32: goto tr87;
		case 33: goto tr88;
		case 39: goto tr89;
		case 44: goto tr88;
		case 46: goto tr91;
		case 59: goto tr88;
		case 61: goto tr92;
		case 62: goto tr93;
		case 63: goto tr88;
		case 64: goto tr94;
		case 69: goto st19;
		case 94: goto tr97;
		case 95: goto tr95;
		case 101: goto st19;
		case 126: goto tr95;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr90;
		} else if ( (*p) >= 9 )
			goto tr87;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr95;
		} else if ( (*p) >= 65 )
			goto tr95;
	} else
		goto st34;
	goto st0;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
	switch( (*p) ) {
		case 43: goto st20;
		case 45: goto st20;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st35;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st35;
	goto st0;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
	switch( (*p) ) {
		case 32: goto tr87;
		case 33: goto tr88;
		case 39: goto tr89;
		case 44: goto tr88;
		case 46: goto tr91;
		case 59: goto tr88;
		case 61: goto tr92;
		case 62: goto tr93;
		case 63: goto tr88;
		case 64: goto tr94;
		case 94: goto tr97;
		case 95: goto tr95;
		case 126: goto tr95;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr90;
		} else if ( (*p) >= 9 )
			goto tr87;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr95;
		} else if ( (*p) >= 65 )
			goto tr95;
	} else
		goto st35;
	goto st0;
	}
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 22: 
	case 24: 
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 0; goto _out;} } }
	break;
	case 23: 
	case 28: 
	{ _parse_int(int_start, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 0; goto _out;} } }
	break;
	case 27: 
	case 29: 
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 0; goto _out;} } }
	break;
	case 34: 
	case 35: 
	{ _parse_float(float_start, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 0; goto _out;} } }
	break;
	case 26: 
	{ _parse_int(int_start, p); }
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 0; goto _out;} } }
	break;
	case 30: 
	case 31: 
	case 32: 
	case 33: 
	{ _parse_uuid(uuid_start, version_at, p); }
	{ _parse_spec(); }
	{ _parse_op(); if (p<pe-1) { p--; {p++; cs = 0; goto _out;} } }
	break;
	}
	}

	_out: {}
	}



    off_ = p-pb;
    pos_++;

    if (cs==RON_error) {
	    return false;
    } else if (cs>=21) { // one of end states
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
    } else {
        return true;
    }
}

}
