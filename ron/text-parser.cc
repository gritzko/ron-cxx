
#line 1 "ragel/text-parser.rl"
#include "text.hpp"

namespace ron {


#line 6 "ragel/text-parser.rl"

#line 11 "ron/text-parser.cc"
static const int RON_start = 14;
static const int RON_first_final = 14;
static const int RON_error = 0;

static const int RON_en_main = 14;


#line 7 "ragel/text-parser.rl"
//%% variable data body;
//%% variable p body;
//%% variable cs ps.state;


// Parse consumes one op from data[], unless the buffer ends earlier.
// Fills atoms[]
bool TextFrame::Cursor::Next () {

    Cursor& cur = *this;
    Atoms& atoms = op_.atoms_;
    TERM& term = op_.term_;
    const std::string& data = this->data();

    switch (cs) {
        case RON_error:
            if (off!=0) {
                return false;
            }
            
#line 40 "ron/text-parser.cc"
	{
	cs = RON_start;
	}

#line 27 "ragel/text-parser.rl"
            atoms.resize(SPEC_SIZE);
            break;

        case RON_FULL_STOP:
            cs = RON_error;
            return false;

        case RON_start:
            atoms.resize(SPEC_SIZE);
            atm = 0;
            hlf = VALUE;
            dgt = 0;
            break;
    }

    if (data.size()<=off) {
        cs = RON_error;
        return false;
    }

    auto p = data.begin() + off;
    auto pb = p;
    auto be = data.begin();
    auto pe = data.end();
    auto eof = pe;
    int n = 0; // tmp atm value holder
    int aso = 0; // atom start offset

    
#line 75 "ron/text-parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	goto _resume;

_again:
	switch ( cs ) {
		case 14: goto st14;
		case 0: goto st0;
		case 1: goto st1;
		case 15: goto st15;
		case 16: goto st16;
		case 17: goto st17;
		case 18: goto st18;
		case 2: goto st2;
		case 3: goto st3;
		case 19: goto st19;
		case 20: goto st20;
		case 21: goto st21;
		case 4: goto st4;
		case 5: goto st5;
		case 22: goto st22;
		case 6: goto st6;
		case 23: goto st23;
		case 24: goto st24;
		case 7: goto st7;
		case 8: goto st8;
		case 9: goto st9;
		case 10: goto st10;
		case 25: goto st25;
		case 11: goto st11;
		case 12: goto st12;
		case 26: goto st26;
		case 27: goto st27;
		case 28: goto st28;
		case 13: goto st13;
		case 29: goto st29;
		case 30: goto st30;
		case 31: goto st31;
		case 32: goto st32;
	default: break;
	}

	if ( ++p == pe )
		goto _test_eof;
_resume:
	switch ( cs )
	{
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
	switch( (*p) ) {
		case 32: goto st1;
		case 33: goto tr2;
		case 35: goto tr3;
		case 39: goto tr4;
		case 42: goto tr3;
		case 44: goto tr2;
		case 46: goto tr29;
		case 58: goto tr3;
		case 59: goto tr2;
		case 61: goto tr5;
		case 62: goto tr6;
		case 63: goto tr2;
		case 64: goto tr3;
		case 94: goto tr7;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st1;
	goto st0;
st0:
cs = 0;
	goto _out;
tr141:
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 167 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st1;
		case 33: goto tr2;
		case 35: goto tr3;
		case 39: goto tr4;
		case 42: goto tr3;
		case 44: goto tr2;
		case 58: goto tr3;
		case 59: goto tr2;
		case 61: goto tr5;
		case 62: goto tr6;
		case 63: goto tr2;
		case 64: goto tr3;
		case 94: goto tr7;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st1;
	goto st0;
tr2:
	cs = 15;
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto _again;
tr31:
	cs = 15;
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto _again;
tr39:
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto st15;
tr50:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto st15;
tr60:
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto st15;
tr68:
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto st15;
tr76:
#line 59 "ragel/./text-grammar.rl"
	{  // std::cerr<<"int_atom_end "<<(p-pb)<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto st15;
tr84:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto st15;
tr93:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto st15;
tr102:
#line 65 "ragel/./text-grammar.rl"
	{  // std::cerr<<"float_atom_end"<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto st15;
tr111:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto st15;
tr122:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto st15;
tr131:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 85 "ragel/./text-grammar.rl"
	{  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[(*p)]);
    }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 437 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st15;
		case 33: goto tr31;
		case 35: goto tr32;
		case 39: goto tr33;
		case 42: goto tr32;
		case 44: goto tr31;
		case 46: goto tr34;
		case 58: goto tr32;
		case 59: goto tr31;
		case 61: goto tr35;
		case 62: goto tr36;
		case 63: goto tr31;
		case 64: goto tr32;
		case 94: goto tr37;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st15;
	goto st0;
tr3:
	cs = 16;
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 6 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_start"<<std::endl;
        atoms.resize(SPEC_SIZE);
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr32:
	cs = 16;
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 6 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_start"<<std::endl;
        atoms.resize(SPEC_SIZE);
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr40:
	cs = 16;
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr51:
	cs = 16;
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr61:
	cs = 16;
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 6 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_start"<<std::endl;
        atoms.resize(SPEC_SIZE);
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr69:
	cs = 16;
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 6 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_start"<<std::endl;
        atoms.resize(SPEC_SIZE);
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr77:
	cs = 16;
#line 59 "ragel/./text-grammar.rl"
	{  // std::cerr<<"int_atom_end "<<(p-pb)<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 6 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_start"<<std::endl;
        atoms.resize(SPEC_SIZE);
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr85:
	cs = 16;
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 6 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_start"<<std::endl;
        atoms.resize(SPEC_SIZE);
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr94:
	cs = 16;
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 6 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_start"<<std::endl;
        atoms.resize(SPEC_SIZE);
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr103:
	cs = 16;
#line 65 "ragel/./text-grammar.rl"
	{  // std::cerr<<"float_atom_end"<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 6 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_start"<<std::endl;
        atoms.resize(SPEC_SIZE);
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr112:
	cs = 16;
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 6 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_start"<<std::endl;
        atoms.resize(SPEC_SIZE);
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr123:
	cs = 16;
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
tr132:
	cs = 16;
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 16 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[(*p)]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                cs = (RON_error);
                {p++; goto _out;}
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            cs = (RON_start);
            position++;
            p--;
            {p++; goto _out;}
        } else { 
            // next UUID
            atm = n;
        }
    }
	goto _again;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 1110 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st16;
		case 33: goto tr39;
		case 35: goto tr40;
		case 39: goto tr42;
		case 42: goto tr40;
		case 44: goto tr39;
		case 46: goto tr43;
		case 58: goto tr40;
		case 59: goto tr39;
		case 61: goto tr45;
		case 62: goto tr46;
		case 63: goto tr39;
		case 64: goto tr40;
		case 94: goto tr47;
		case 96: goto tr48;
		case 126: goto tr44;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr41;
		} else if ( (*p) >= 9 )
			goto st16;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr44;
		} else if ( (*p) > 90 ) {
			if ( 95 <= (*p) && (*p) <= 122 )
				goto tr44;
		} else
			goto tr44;
	} else
		goto tr41;
	goto st0;
tr41:
#line 5 "ragel/././uuid-grammar.rl"
	{
        hlf = VALUE;
    }
#line 41 "ragel/././uuid-grammar.rl"
	{
        hlf = ORIGIN;
        atoms[atm][ORIGIN].zero_flags();
        atoms[atm][ORIGIN].set_flags(ABC[(*p)]);
    }
	goto st17;
tr133:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 41 "ragel/././uuid-grammar.rl"
	{
        hlf = ORIGIN;
        atoms[atm][ORIGIN].zero_flags();
        atoms[atm][ORIGIN].set_flags(ABC[(*p)]);
    }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 1174 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr49;
		case 33: goto tr50;
		case 35: goto tr51;
		case 39: goto tr52;
		case 42: goto tr51;
		case 44: goto tr50;
		case 46: goto tr53;
		case 58: goto tr51;
		case 59: goto tr50;
		case 61: goto tr55;
		case 62: goto tr56;
		case 63: goto tr50;
		case 64: goto tr51;
		case 94: goto tr57;
		case 95: goto tr54;
		case 126: goto tr54;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr49;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr54;
		} else if ( (*p) >= 65 )
			goto tr54;
	} else
		goto tr54;
	goto st0;
tr49:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
	goto st18;
tr121:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
	goto st18;
tr130:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 1233 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st18;
		case 33: goto tr39;
		case 35: goto tr40;
		case 39: goto tr42;
		case 42: goto tr40;
		case 44: goto tr39;
		case 46: goto tr43;
		case 58: goto tr40;
		case 59: goto tr39;
		case 61: goto tr45;
		case 62: goto tr46;
		case 63: goto tr39;
		case 64: goto tr40;
		case 94: goto tr47;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st18;
	goto st0;
tr4:
	cs = 2;
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto _again;
tr33:
	cs = 2;
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto _again;
tr42:
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st2;
tr52:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st2;
tr62:
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st2;
tr70:
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st2;
tr78:
#line 59 "ragel/./text-grammar.rl"
	{  // std::cerr<<"int_atom_end "<<(p-pb)<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st2;
tr86:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st2;
tr95:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st2;
tr104:
#line 65 "ragel/./text-grammar.rl"
	{  // std::cerr<<"float_atom_end"<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st2;
tr114:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st2;
tr124:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st2;
tr134:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 1565 "ron/text-parser.cc"
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto st0;
		case 34: goto st0;
		case 39: goto tr9;
		case 92: goto tr10;
	}
	goto tr8;
tr8:
#line 68 "ragel/./text-grammar.rl"
	{  // std::cerr<<"string_atom_start"<<std::endl;
	    atoms[atm][ORIGIN].put2(30, ATOM::STRING);
    }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 1584 "ron/text-parser.cc"
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto st0;
		case 34: goto st0;
		case 39: goto tr12;
		case 92: goto st13;
	}
	goto st3;
tr9:
#line 68 "ragel/./text-grammar.rl"
	{  // std::cerr<<"string_atom_start"<<std::endl;
	    atoms[atm][ORIGIN].put2(30, ATOM::STRING);
    }
#line 71 "ragel/./text-grammar.rl"
	{  // std::cerr<<"string_atom_end"<<std::endl;
    }
	goto st19;
tr12:
#line 71 "ragel/./text-grammar.rl"
	{  // std::cerr<<"string_atom_end"<<std::endl;
    }
	goto st19;
st19:
	if ( ++p == pe )
		goto _test_eof19;
case 19:
#line 1611 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr59;
		case 33: goto tr60;
		case 35: goto tr61;
		case 39: goto tr62;
		case 42: goto tr61;
		case 44: goto tr60;
		case 46: goto tr63;
		case 58: goto tr61;
		case 59: goto tr60;
		case 61: goto tr64;
		case 62: goto tr65;
		case 63: goto tr60;
		case 64: goto tr61;
		case 94: goto tr66;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto tr59;
	goto st0;
tr59:
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
	goto st20;
tr75:
#line 59 "ragel/./text-grammar.rl"
	{  // std::cerr<<"int_atom_end "<<(p-pb)<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
	goto st20;
tr83:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
	goto st20;
tr92:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
	goto st20;
tr101:
#line 65 "ragel/./text-grammar.rl"
	{  // std::cerr<<"float_atom_end"<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
	goto st20;
tr110:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
	goto st20;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
#line 1713 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st20;
		case 33: goto tr68;
		case 35: goto tr69;
		case 39: goto tr70;
		case 42: goto tr69;
		case 44: goto tr68;
		case 46: goto tr71;
		case 58: goto tr69;
		case 59: goto tr68;
		case 61: goto tr72;
		case 62: goto tr73;
		case 63: goto tr68;
		case 64: goto tr69;
		case 94: goto tr74;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st20;
	goto st0;
tr29:
	cs = 21;
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr34:
	cs = 21;
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr43:
	cs = 21;
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr53:
	cs = 21;
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr63:
	cs = 21;
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr71:
	cs = 21;
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr79:
	cs = 21;
#line 59 "ragel/./text-grammar.rl"
	{  // std::cerr<<"int_atom_end "<<(p-pb)<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr87:
	cs = 21;
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr96:
	cs = 21;
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr105:
	cs = 21;
#line 65 "ragel/./text-grammar.rl"
	{  // std::cerr<<"float_atom_end"<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr115:
	cs = 21;
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr125:
	cs = 21;
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
tr135:
	cs = 21;
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 111 "ragel/./text-grammar.rl"
	{  // std::cerr<<"frame_end"<<std::endl;
        cs = (RON_FULL_STOP);
        {p++; goto _out;}
    }
	goto _again;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
#line 2022 "ron/text-parser.cc"
	goto st0;
tr5:
	cs = 4;
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto _again;
tr35:
	cs = 4;
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto _again;
tr45:
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st4;
tr55:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st4;
tr64:
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st4;
tr72:
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st4;
tr80:
#line 59 "ragel/./text-grammar.rl"
	{  // std::cerr<<"int_atom_end "<<(p-pb)<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st4;
tr89:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st4;
tr98:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st4;
tr106:
#line 65 "ragel/./text-grammar.rl"
	{  // std::cerr<<"float_atom_end"<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st4;
tr118:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st4;
tr127:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st4;
tr138:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 2336 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st4;
		case 43: goto tr15;
		case 45: goto tr15;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr16;
	} else if ( (*p) >= 9 )
		goto st4;
	goto st0;
tr15:
#line 56 "ragel/./text-grammar.rl"
	{  // std::cerr<<"int_atom_start "<<(p-pb)<<std::endl;
        atoms[atm][ORIGIN].put2(30, ATOM::INT);
    }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 2358 "ron/text-parser.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st22;
	goto st0;
tr16:
#line 56 "ragel/./text-grammar.rl"
	{  // std::cerr<<"int_atom_start "<<(p-pb)<<std::endl;
        atoms[atm][ORIGIN].put2(30, ATOM::INT);
    }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 2372 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr75;
		case 33: goto tr76;
		case 35: goto tr77;
		case 39: goto tr78;
		case 42: goto tr77;
		case 44: goto tr76;
		case 46: goto tr79;
		case 58: goto tr77;
		case 59: goto tr76;
		case 61: goto tr80;
		case 62: goto tr81;
		case 63: goto tr76;
		case 64: goto tr77;
		case 94: goto tr82;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st22;
	} else if ( (*p) >= 9 )
		goto tr75;
	goto st0;
tr6:
	cs = 6;
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto _again;
tr36:
	cs = 6;
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto _again;
tr46:
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st6;
tr56:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st6;
tr65:
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st6;
tr73:
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st6;
tr81:
#line 59 "ragel/./text-grammar.rl"
	{  // std::cerr<<"int_atom_end "<<(p-pb)<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st6;
tr90:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st6;
tr99:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st6;
tr107:
#line 65 "ragel/./text-grammar.rl"
	{  // std::cerr<<"float_atom_end"<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st6;
tr119:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st6;
tr128:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st6;
tr139:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st6;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
#line 2707 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st6;
		case 43: goto tr19;
		case 45: goto tr19;
		case 95: goto tr20;
		case 126: goto tr20;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr19;
		} else if ( (*p) >= 9 )
			goto st6;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr20;
		} else if ( (*p) >= 65 )
			goto tr20;
	} else
		goto tr20;
	goto st0;
tr19:
#line 74 "ragel/./text-grammar.rl"
	{  // std::cerr<<"uuid_atom_start"<<std::endl;
        atoms[atm] = atoms[atm-1];
    }
#line 5 "ragel/././uuid-grammar.rl"
	{
        hlf = VALUE;
    }
#line 41 "ragel/././uuid-grammar.rl"
	{
        hlf = ORIGIN;
        atoms[atm][ORIGIN].zero_flags();
        atoms[atm][ORIGIN].set_flags(ABC[(*p)]);
    }
	goto st23;
tr113:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 41 "ragel/././uuid-grammar.rl"
	{
        hlf = ORIGIN;
        atoms[atm][ORIGIN].zero_flags();
        atoms[atm][ORIGIN].set_flags(ABC[(*p)]);
    }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 2761 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr83;
		case 33: goto tr84;
		case 35: goto tr85;
		case 39: goto tr86;
		case 42: goto tr85;
		case 44: goto tr84;
		case 46: goto tr87;
		case 58: goto tr85;
		case 59: goto tr84;
		case 61: goto tr89;
		case 62: goto tr90;
		case 63: goto tr84;
		case 64: goto tr85;
		case 94: goto tr91;
		case 95: goto tr88;
		case 126: goto tr88;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr83;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr88;
		} else if ( (*p) >= 65 )
			goto tr88;
	} else
		goto tr88;
	goto st0;
tr88:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 9 "ragel/././uuid-grammar.rl"
	{ // std::cerr << "digit " << fc << '\n';
        if (dgt>9) {
            {p++; cs = 24; goto _out;}
        }
        atoms[atm][hlf].put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st24;
tr97:
#line 9 "ragel/././uuid-grammar.rl"
	{ // std::cerr << "digit " << fc << '\n';
        if (dgt>9) {
            {p++; cs = 24; goto _out;}
        }
        atoms[atm][hlf].put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 2825 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr92;
		case 33: goto tr93;
		case 35: goto tr94;
		case 39: goto tr95;
		case 42: goto tr94;
		case 44: goto tr93;
		case 46: goto tr96;
		case 58: goto tr94;
		case 59: goto tr93;
		case 61: goto tr98;
		case 62: goto tr99;
		case 63: goto tr93;
		case 64: goto tr94;
		case 94: goto tr100;
		case 95: goto tr97;
		case 126: goto tr97;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr92;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr97;
		} else if ( (*p) >= 65 )
			goto tr97;
	} else
		goto tr97;
	goto st0;
tr7:
	cs = 7;
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto _again;
tr37:
	cs = 7;
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
#line 89 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            cs = (RON_start);
            {p++; goto _out;}
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto _again;
tr47:
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st7;
tr57:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st7;
tr66:
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st7;
tr74:
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st7;
tr82:
#line 59 "ragel/./text-grammar.rl"
	{  // std::cerr<<"int_atom_end "<<(p-pb)<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st7;
tr91:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st7;
tr100:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st7;
tr109:
#line 65 "ragel/./text-grammar.rl"
	{  // std::cerr<<"float_atom_end"<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st7;
tr120:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st7;
tr129:
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st7;
tr140:
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 78 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
#line 42 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
	goto st7;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
#line 3168 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st7;
		case 43: goto tr22;
		case 45: goto tr22;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr23;
	} else if ( (*p) >= 9 )
		goto st7;
	goto st0;
tr22:
#line 62 "ragel/./text-grammar.rl"
	{  // std::cerr<<"float_atom_start"<<std::endl;
        atoms[atm][ORIGIN].put2(30, ATOM::FLOAT);
    }
	goto st8;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
#line 3190 "ron/text-parser.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st9;
	goto st0;
tr23:
#line 62 "ragel/./text-grammar.rl"
	{  // std::cerr<<"float_atom_start"<<std::endl;
        atoms[atm][ORIGIN].put2(30, ATOM::FLOAT);
    }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 3204 "ron/text-parser.cc"
	if ( (*p) == 46 )
		goto st10;
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st9;
	goto st0;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st25;
	goto st0;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
	switch( (*p) ) {
		case 32: goto tr101;
		case 33: goto tr102;
		case 35: goto tr103;
		case 39: goto tr104;
		case 42: goto tr103;
		case 44: goto tr102;
		case 46: goto tr105;
		case 58: goto tr103;
		case 59: goto tr102;
		case 61: goto tr106;
		case 62: goto tr107;
		case 63: goto tr102;
		case 64: goto tr103;
		case 69: goto st11;
		case 94: goto tr109;
		case 101: goto st11;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st25;
	} else if ( (*p) >= 9 )
		goto tr101;
	goto st0;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
	switch( (*p) ) {
		case 43: goto st12;
		case 45: goto st12;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st26;
	goto st0;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st26;
	goto st0;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
	switch( (*p) ) {
		case 32: goto tr101;
		case 33: goto tr102;
		case 35: goto tr103;
		case 39: goto tr104;
		case 42: goto tr103;
		case 44: goto tr102;
		case 46: goto tr105;
		case 58: goto tr103;
		case 59: goto tr102;
		case 61: goto tr106;
		case 62: goto tr107;
		case 63: goto tr102;
		case 64: goto tr103;
		case 94: goto tr109;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st26;
	} else if ( (*p) >= 9 )
		goto tr101;
	goto st0;
tr20:
#line 74 "ragel/./text-grammar.rl"
	{  // std::cerr<<"uuid_atom_start"<<std::endl;
        atoms[atm] = atoms[atm-1];
    }
#line 5 "ragel/././uuid-grammar.rl"
	{
        hlf = VALUE;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 21 "ragel/././uuid-grammar.rl"
	{
    }
#line 9 "ragel/././uuid-grammar.rl"
	{ // std::cerr << "digit " << fc << '\n';
        if (dgt>9) {
            {p++; cs = 27; goto _out;}
        }
        atoms[atm][hlf].put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 3318 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr110;
		case 33: goto tr111;
		case 35: goto tr112;
		case 39: goto tr114;
		case 42: goto tr112;
		case 44: goto tr111;
		case 46: goto tr115;
		case 47: goto tr116;
		case 58: goto tr112;
		case 59: goto tr111;
		case 61: goto tr118;
		case 62: goto tr119;
		case 63: goto tr111;
		case 64: goto tr112;
		case 94: goto tr120;
		case 95: goto tr117;
		case 126: goto tr117;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr113;
		} else if ( (*p) >= 9 )
			goto tr110;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr117;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr117;
		} else
			goto tr117;
	} else
		goto tr113;
	goto st0;
tr117:
#line 9 "ragel/././uuid-grammar.rl"
	{ // std::cerr << "digit " << fc << '\n';
        if (dgt>9) {
            {p++; cs = 28; goto _out;}
        }
        atoms[atm][hlf].put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st28;
tr116:
#line 35 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][VALUE].set_flags(atoms[atm][VALUE].get6(9));
        atoms[atm][VALUE].zero_payload();
        dgt--;
    }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 3378 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr110;
		case 33: goto tr111;
		case 35: goto tr112;
		case 39: goto tr114;
		case 42: goto tr112;
		case 44: goto tr111;
		case 46: goto tr115;
		case 58: goto tr112;
		case 59: goto tr111;
		case 61: goto tr118;
		case 62: goto tr119;
		case 63: goto tr111;
		case 64: goto tr112;
		case 94: goto tr120;
		case 95: goto tr117;
		case 126: goto tr117;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr113;
		} else if ( (*p) >= 9 )
			goto tr110;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr117;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr117;
		} else
			goto tr117;
	} else
		goto tr113;
	goto st0;
tr10:
#line 68 "ragel/./text-grammar.rl"
	{  // std::cerr<<"string_atom_start"<<std::endl;
	    atoms[atm][ORIGIN].put2(30, ATOM::STRING);
    }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 3425 "ron/text-parser.cc"
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto st0;
	}
	goto st3;
tr54:
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 9 "ragel/././uuid-grammar.rl"
	{ // std::cerr << "digit " << fc << '\n';
        if (dgt>9) {
            {p++; cs = 29; goto _out;}
        }
        atoms[atm][hlf].put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st29;
tr126:
#line 9 "ragel/././uuid-grammar.rl"
	{ // std::cerr << "digit " << fc << '\n';
        if (dgt>9) {
            {p++; cs = 29; goto _out;}
        }
        atoms[atm][hlf].put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st29;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
#line 3464 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr121;
		case 33: goto tr122;
		case 35: goto tr123;
		case 39: goto tr124;
		case 42: goto tr123;
		case 44: goto tr122;
		case 46: goto tr125;
		case 58: goto tr123;
		case 59: goto tr122;
		case 61: goto tr127;
		case 62: goto tr128;
		case 63: goto tr122;
		case 64: goto tr123;
		case 94: goto tr129;
		case 95: goto tr126;
		case 126: goto tr126;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr121;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr126;
		} else if ( (*p) >= 65 )
			goto tr126;
	} else
		goto tr126;
	goto st0;
tr44:
#line 5 "ragel/././uuid-grammar.rl"
	{
        hlf = VALUE;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 21 "ragel/././uuid-grammar.rl"
	{
    }
#line 9 "ragel/././uuid-grammar.rl"
	{ // std::cerr << "digit " << fc << '\n';
        if (dgt>9) {
            {p++; cs = 30; goto _out;}
        }
        atoms[atm][hlf].put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st30;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
#line 3520 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr130;
		case 33: goto tr131;
		case 35: goto tr132;
		case 39: goto tr134;
		case 42: goto tr132;
		case 44: goto tr131;
		case 46: goto tr135;
		case 47: goto tr136;
		case 58: goto tr132;
		case 59: goto tr131;
		case 61: goto tr138;
		case 62: goto tr139;
		case 63: goto tr131;
		case 64: goto tr132;
		case 94: goto tr140;
		case 95: goto tr137;
		case 126: goto tr137;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr133;
		} else if ( (*p) >= 9 )
			goto tr130;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr137;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr137;
		} else
			goto tr137;
	} else
		goto tr133;
	goto st0;
tr137:
#line 9 "ragel/././uuid-grammar.rl"
	{ // std::cerr << "digit " << fc << '\n';
        if (dgt>9) {
            {p++; cs = 31; goto _out;}
        }
        atoms[atm][hlf].put6(9-dgt, ABC[(*p)]);
        dgt++;
    }
	goto st31;
tr136:
#line 35 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][VALUE].set_flags(atoms[atm][VALUE].get6(9));
        atoms[atm][VALUE].zero_payload();
        dgt--;
    }
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
#line 3580 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr130;
		case 33: goto tr131;
		case 35: goto tr132;
		case 39: goto tr134;
		case 42: goto tr132;
		case 44: goto tr131;
		case 46: goto tr135;
		case 58: goto tr132;
		case 59: goto tr131;
		case 61: goto tr138;
		case 62: goto tr139;
		case 63: goto tr131;
		case 64: goto tr132;
		case 94: goto tr140;
		case 95: goto tr137;
		case 126: goto tr137;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr133;
		} else if ( (*p) >= 9 )
			goto tr130;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr137;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr137;
		} else
			goto tr137;
	} else
		goto tr133;
	goto st0;
tr48:
#line 10 "ragel/./text-grammar.rl"
	{  // std::cerr<<"redef_uuid"<<std::endl;
        if (atm>0) {
            atoms[atm] = atoms[atm-1];
        }
    }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 3629 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr141;
		case 33: goto tr39;
		case 35: goto tr40;
		case 39: goto tr42;
		case 42: goto tr40;
		case 44: goto tr39;
		case 46: goto tr43;
		case 58: goto tr40;
		case 59: goto tr39;
		case 61: goto tr45;
		case 62: goto tr46;
		case 63: goto tr39;
		case 64: goto tr40;
		case 94: goto tr47;
		case 95: goto tr44;
		case 126: goto tr44;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr41;
		} else if ( (*p) >= 9 )
			goto tr141;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr44;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr44;
		} else
			goto tr44;
	} else
		goto tr41;
	goto st0;
	}
	_test_eof14: cs = 14; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
	_test_eof15: cs = 15; goto _test_eof; 
	_test_eof16: cs = 16; goto _test_eof; 
	_test_eof17: cs = 17; goto _test_eof; 
	_test_eof18: cs = 18; goto _test_eof; 
	_test_eof2: cs = 2; goto _test_eof; 
	_test_eof3: cs = 3; goto _test_eof; 
	_test_eof19: cs = 19; goto _test_eof; 
	_test_eof20: cs = 20; goto _test_eof; 
	_test_eof21: cs = 21; goto _test_eof; 
	_test_eof4: cs = 4; goto _test_eof; 
	_test_eof5: cs = 5; goto _test_eof; 
	_test_eof22: cs = 22; goto _test_eof; 
	_test_eof6: cs = 6; goto _test_eof; 
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof7: cs = 7; goto _test_eof; 
	_test_eof8: cs = 8; goto _test_eof; 
	_test_eof9: cs = 9; goto _test_eof; 
	_test_eof10: cs = 10; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof11: cs = 11; goto _test_eof; 
	_test_eof12: cs = 12; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof13: cs = 13; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 15: 
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
	case 20: 
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
	case 16: 
	case 18: 
	case 32: 
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
	case 19: 
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
	case 30: 
	case 31: 
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
	case 27: 
	case 28: 
#line 29 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
	case 29: 
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
	case 24: 
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
	case 22: 
#line 59 "ragel/./text-grammar.rl"
	{  // std::cerr<<"int_atom_end "<<(p-pb)<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
	case 25: 
	case 26: 
#line 65 "ragel/./text-grammar.rl"
	{  // std::cerr<<"float_atom_end"<<std::endl;
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
	case 17: 
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 38 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }
#line 108 "ragel/./text-grammar.rl"
	{  // std::cerr<<"spec_end"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
	case 23: 
#line 24 "ragel/././uuid-grammar.rl"
	{
        dgt = 0;
        hlf = ORIGIN;
    }
#line 17 "ragel/././uuid-grammar.rl"
	{
        atoms[atm][hlf].zero_payload();
    }
#line 32 "ragel/././uuid-grammar.rl"
	{
    }
#line 49 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }
#line 82 "ragel/./text-grammar.rl"
	{  // std::cerr<<"atoms"<<std::endl;
    }
#line 104 "ragel/./text-grammar.rl"
	{  // std::cerr<<"op_end"<<std::endl;
        position++;
    }
	break;
#line 3914 "ron/text-parser.cc"
	}
	}

	_out: {}
	}

#line 59 "ragel/text-parser.rl"


    off = p - data.begin();

    if (cs==RON_error) {
	    return false;
    } else if (cs>=RON_first_final) { // one of end states
        if (p>=eof) {
            // in the block mode, the final dot is optional/implied
            cs = RON_FULL_STOP;
        }
    } else if (cs==RON_FULL_STOP) {
        return true; // explicit dot
    } else if (cs==RON_start) {
    } else {
        cs = RON_error;
	    return false;
    }
    return true;
}

}
