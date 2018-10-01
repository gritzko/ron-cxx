
#line 1 "ragel/uuid-parser.rl"
#include "uuid.hpp"

namespace ron {


    Uuid::Uuid (const std::string& buf) {
        
#line 8 "ragel/uuid-parser.rl"
        
#line 13 "ron/uuid-parser.cc"
static const int UUID_start = 1;
static const int UUID_first_final = 2;
static const int UUID_error = 0;

static const int UUID_en_main = 1;


#line 9 "ragel/uuid-parser.rl"
        int atm=0, dgt=0, hlf=0;
        auto pe = buf.end();
        auto p = buf.begin();
        auto eof = pe;
        int cs = 0;

        
#line 29 "ron/uuid-parser.cc"
	{
	cs = UUID_start;
	}

#line 34 "ron/uuid-parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
case 1:
	switch( (*p) ) {
		case 43: goto tr0;
		case 45: goto tr0;
		case 95: goto tr2;
		case 126: goto tr2;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto tr0;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr2;
		} else if ( (*p) >= 65 )
			goto tr2;
	} else
		goto tr2;
	goto st0;
st0:
cs = 0;
	goto _out;
tr0:
#line 5 "ragel/./uuid-grammar.rl"
	{
    }
#line 40 "ragel/./uuid-grammar.rl"
	{
        hlf = 1;
        word(atm, 1).zero_flags();
        word(atm, 1).set_flags(ABC[(*p)]);
    }
	goto st2;
tr5:
#line 28 "ragel/./uuid-grammar.rl"
	{
    }
#line 40 "ragel/./uuid-grammar.rl"
	{
        hlf = 1;
        word(atm, 1).zero_flags();
        word(atm, 1).set_flags(ABC[(*p)]);
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 88 "ron/uuid-parser.cc"
	switch( (*p) ) {
		case 95: goto tr3;
		case 126: goto tr3;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr3;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr3;
	} else
		goto tr3;
	goto st0;
tr3:
#line 23 "ragel/./uuid-grammar.rl"
	{
        dgt = 0;
        hlf = 1;
    }
#line 16 "ragel/./uuid-grammar.rl"
	{
        //word(atm, hlf).zero_payload();
    }
#line 8 "ragel/./uuid-grammar.rl"
	{
        if (dgt>9) {
            {p++; cs = 3; goto _out;}
        }
        word(atm,hlf).put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st3;
tr4:
#line 8 "ragel/./uuid-grammar.rl"
	{
        if (dgt>9) {
            {p++; cs = 3; goto _out;}
        }
        word(atm,hlf).put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 135 "ron/uuid-parser.cc"
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
tr2:
#line 5 "ragel/./uuid-grammar.rl"
	{
    }
#line 20 "ragel/./uuid-grammar.rl"
	{
    }
#line 8 "ragel/./uuid-grammar.rl"
	{
        if (dgt>9) {
            {p++; cs = 4; goto _out;}
        }
        word(atm,hlf).put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 169 "ron/uuid-parser.cc"
	switch( (*p) ) {
		case 43: goto tr5;
		case 45: goto tr5;
		case 47: goto tr6;
		case 95: goto tr7;
		case 126: goto tr7;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto tr5;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr7;
		} else if ( (*p) >= 65 )
			goto tr7;
	} else
		goto tr7;
	goto st0;
tr7:
#line 8 "ragel/./uuid-grammar.rl"
	{
        if (dgt>9) {
            {p++; cs = 5; goto _out;}
        }
        word(atm,hlf).put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st5;
tr6:
#line 34 "ragel/./uuid-grammar.rl"
	{
        word(atm, 0).set_flags(word(atm, 0).get6(9));
        word(atm, hlf).zero_payload();
        dgt--;
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 211 "ron/uuid-parser.cc"
	switch( (*p) ) {
		case 43: goto tr5;
		case 45: goto tr5;
		case 95: goto tr7;
		case 126: goto tr7;
	}
	if ( (*p) < 48 ) {
		if ( 36 <= (*p) && (*p) <= 37 )
			goto tr5;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr7;
		} else if ( (*p) >= 65 )
			goto tr7;
	} else
		goto tr7;
	goto st0;
	}
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 4: 
	case 5: 
#line 28 "ragel/./uuid-grammar.rl"
	{
    }
	break;
	case 3: 
#line 31 "ragel/./uuid-grammar.rl"
	{
    }
	break;
	case 2: 
#line 23 "ragel/./uuid-grammar.rl"
	{
        dgt = 0;
        hlf = 1;
    }
#line 16 "ragel/./uuid-grammar.rl"
	{
        //word(atm, hlf).zero_payload();
    }
#line 31 "ragel/./uuid-grammar.rl"
	{
    }
	break;
#line 265 "ron/uuid-parser.cc"
	}
	}

	_out: {}
	}

#line 22 "ragel/uuid-parser.rl"


    }

}
