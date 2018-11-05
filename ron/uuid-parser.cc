
#line 1 "ragel/uuid-parser.rl"
#include "uuid.hpp"

namespace ron {


    Uuid::Uuid (slice_t data) {
        
#line 8 "ragel/uuid-parser.rl"
        
#line 13 "ron/uuid-parser.cc"
static const int UUID_start = 1;
static const int UUID_first_final = 1;
static const int UUID_error = 0;

static const int UUID_en_main = 1;


#line 9 "ragel/uuid-parser.rl"

        const char* pe = data.buf_+data.size_;
        const char* p = data.buf_;
        const char* eof = pe;
        int cs = 0;

        slice_t value{}, origin{}, uuid{};
        char variety{'0'}, version{'$'};

        
#line 32 "ron/uuid-parser.cc"
	{
	cs = UUID_start;
	}

#line 37 "ron/uuid-parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 43: goto tr0;
		case 45: goto tr0;
		case 95: goto tr3;
		case 126: goto tr3;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 37 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr2;
		} else if ( (*p) >= 36 )
			goto tr0;
	} else if ( (*p) > 70 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr3;
		} else if ( (*p) >= 71 )
			goto tr3;
	} else
		goto tr2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 7 "ragel/./uuid-grammar.rl"
	{ value = p; }
#line 8 "ragel/./uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 5 "ragel/./uuid-grammar.rl"
	{ uuid = p; }
#line 9 "ragel/./uuid-grammar.rl"
	{ version = (*p); }
	goto st2;
tr6:
#line 8 "ragel/./uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 5 "ragel/./uuid-grammar.rl"
	{ uuid = p; }
#line 9 "ragel/./uuid-grammar.rl"
	{ version = (*p); }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 90 "ron/uuid-parser.cc"
	switch( (*p) ) {
		case 95: goto tr4;
		case 126: goto tr4;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr4;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr4;
	} else
		goto tr4;
	goto st0;
tr4:
#line 10 "ragel/./uuid-grammar.rl"
	{ origin = p; }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 112 "ron/uuid-parser.cc"
	switch( (*p) ) {
		case 95: goto st3;
		case 126: goto st3;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st3;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto st3;
	} else
		goto st3;
	goto st0;
tr2:
#line 7 "ragel/./uuid-grammar.rl"
	{ value = p; }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 134 "ron/uuid-parser.cc"
	switch( (*p) ) {
		case 43: goto tr6;
		case 45: goto tr6;
		case 47: goto tr7;
		case 95: goto st6;
		case 126: goto st6;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto tr6;
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
#line 6 "ragel/./uuid-grammar.rl"
	{ variety = *(p-1); }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 162 "ron/uuid-parser.cc"
	switch( (*p) ) {
		case 43: goto tr0;
		case 45: goto tr0;
		case 95: goto tr3;
		case 126: goto tr3;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto tr0;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr3;
		} else if ( (*p) >= 65 )
			goto tr3;
	} else
		goto tr3;
	goto st0;
tr3:
#line 7 "ragel/./uuid-grammar.rl"
	{ value = p; }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 189 "ron/uuid-parser.cc"
	switch( (*p) ) {
		case 43: goto tr6;
		case 45: goto tr6;
		case 95: goto st6;
		case 126: goto st6;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto tr6;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st6;
		} else if ( (*p) >= 65 )
			goto st6;
	} else
		goto st6;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 3: 
#line 11 "ragel/./uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/./uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
	break;
	case 4: 
	case 6: 
#line 8 "ragel/./uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 5 "ragel/./uuid-grammar.rl"
	{ uuid = p; }
#line 12 "ragel/./uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
	break;
	case 2: 
#line 10 "ragel/./uuid-grammar.rl"
	{ origin = p; }
#line 11 "ragel/./uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/./uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
	break;
	case 1: 
	case 5: 
#line 7 "ragel/./uuid-grammar.rl"
	{ value = p; }
#line 8 "ragel/./uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 5 "ragel/./uuid-grammar.rl"
	{ uuid = p; }
#line 12 "ragel/./uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
	break;
#line 253 "ron/uuid-parser.cc"
	}
	}

	_out: {}
	}

#line 25 "ragel/uuid-parser.rl"


        if (cs) {
            words_.first = Word{ABC[variety], value};
            words_.second = Word{ABC[version], origin};
        } else {
            *this = FATAL;
        }

    }

}
