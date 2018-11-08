
#line 1 "ragel/text-parser.rl"
#include "text.hpp"
#include <iostream>

namespace ron {


#line 7 "ragel/text-parser.rl"

#line 12 "ron/text-parser.cc"
static const int RON_start = 48;
static const int RON_first_final = 48;
static const int RON_error = 0;

static const int RON_en_main = 48;


#line 8 "ragel/text-parser.rl"


bool TextFrame::Cursor::Next () {

    Atoms& atoms = op_.atoms_;

    // hint: the machine must start parsing in the "space" state
    // to allow for frames like `1, 2, 3 or 'string'`

    switch (cs) {
        case RON_error:
            if (off_!=0) {
                return false;
            }
            
#line 36 "ron/text-parser.cc"
	{
	cs = RON_start;
	}

#line 23 "ragel/text-parser.rl"
            break;

        case RON_FULL_STOP:
            cs = RON_error;
            return false;

        default:
            break;
    }

    if (data().size()<=off_) {
        cs = RON_error;
        return false;
    }

    iter pb = data().data();
    iter p = pb + off_;
    iter pe = pb + data().size();
    iter eof = pe;

    const char* intb{p};
    const char* floatb{p};
    const char* strb{p};
    char term{0};
    slice_t uuid, value, origin;
    char variety, version;

    atoms.resize(0);
    op_.AddAtom(prev_id_.inc());
    op_.AddAtom(prev_id_);

    std::cerr<<"starting with "<<cs<<" ["<<p<<"]\n";

    
#line 76 "ron/text-parser.cc"
	{
	if ( p == pe )
		goto _test_eof;
	switch ( cs )
	{
tr148:
#line 25 "ragel/./text-grammar.rl"
	{  }
	goto st48;
st48:
	if ( ++p == pe )
		goto _test_eof48;
case 48:
#line 90 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st48;
		case 33: goto tr6;
		case 39: goto st1;
		case 44: goto tr6;
		case 46: goto st50;
		case 59: goto tr6;
		case 61: goto st9;
		case 62: goto st12;
		case 63: goto tr6;
		case 64: goto st35;
		case 94: goto st16;
		case 95: goto tr14;
		case 126: goto tr14;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr11;
		} else if ( (*p) >= 9 )
			goto st48;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 71 ) {
			if ( 65 <= (*p) && (*p) <= 70 )
				goto tr13;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr14;
		} else
			goto tr14;
	} else
		goto tr12;
	goto st0;
st0:
cs = 0;
	goto _out;
tr6:
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr17:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr26:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr37:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr47:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr60:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr72:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr80:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr89:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr96:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr104:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr118:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr128:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr137:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
tr149:
#line 25 "ragel/./text-grammar.rl"
	{  }
#line 22 "ragel/./text-grammar.rl"
	{ term = (*p); pos_++; if (p<pe-1) { {p++; cs = 49; goto _out;} } }
	goto st49;
st49:
	if ( ++p == pe )
		goto _test_eof49;
case 49:
#line 308 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr148;
		case 33: goto tr149;
		case 39: goto tr150;
		case 44: goto tr149;
		case 46: goto tr152;
		case 59: goto tr149;
		case 61: goto tr154;
		case 62: goto tr155;
		case 63: goto tr149;
		case 64: goto tr156;
		case 94: goto tr159;
		case 95: goto tr158;
		case 126: goto tr158;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr151;
		} else if ( (*p) >= 9 )
			goto tr148;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 71 ) {
			if ( 65 <= (*p) && (*p) <= 70 )
				goto tr157;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr158;
		} else
			goto tr158;
	} else
		goto tr153;
	goto st0;
tr18:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
	goto st1;
tr27:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
	goto st1;
tr39:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st1;
tr48:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st1;
tr62:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st1;
tr73:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st1;
tr81:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st1;
tr90:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st1;
tr97:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st1;
tr106:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st1;
tr120:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
	goto st1;
tr129:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
	goto st1;
tr138:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st1;
tr150:
#line 25 "ragel/./text-grammar.rl"
	{  }
	goto st1;
st1:
	if ( ++p == pe )
		goto _test_eof1;
case 1:
#line 491 "ron/text-parser.cc"
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto st0;
		case 39: goto tr2;
		case 92: goto st0;
	}
	goto tr0;
tr0:
#line 20 "ragel/./text-grammar.rl"
	{ strb = p; }
	goto st2;
st2:
	if ( ++p == pe )
		goto _test_eof2;
case 2:
#line 507 "ron/text-parser.cc"
	switch( (*p) ) {
		case 10: goto st0;
		case 13: goto st0;
		case 39: goto tr4;
		case 92: goto st0;
	}
	goto st2;
tr2:
#line 20 "ragel/./text-grammar.rl"
	{ strb = p; }
#line 21 "ragel/./text-grammar.rl"
	{ std::cerr<<"S\n"; op_.AddAtom(Atom::String(strb-pb, p-strb)); }
	goto st3;
tr4:
#line 21 "ragel/./text-grammar.rl"
	{ std::cerr<<"S\n"; op_.AddAtom(Atom::String(strb-pb, p-strb)); }
	goto st3;
st3:
	if ( ++p == pe )
		goto _test_eof3;
case 3:
#line 529 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st4;
		case 33: goto tr6;
		case 39: goto st1;
		case 44: goto tr6;
		case 59: goto tr6;
		case 61: goto st9;
		case 62: goto st12;
		case 63: goto tr6;
		case 94: goto st16;
	}
	if ( 9 <= (*p) && (*p) <= 13 )
		goto st4;
	goto st0;
tr16:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
	goto st4;
tr25:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
	goto st4;
tr36:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st4;
tr46:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st4;
tr59:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st4;
tr71:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st4;
tr79:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st4;
tr88:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st4;
tr95:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st4;
tr117:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
	goto st4;
tr127:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
	goto st4;
st4:
	if ( ++p == pe )
		goto _test_eof4;
case 4:
#line 667 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st4;
		case 33: goto tr6;
		case 39: goto st1;
		case 44: goto tr6;
		case 59: goto tr6;
		case 61: goto st9;
		case 62: goto st12;
		case 63: goto tr6;
		case 94: goto st16;
		case 95: goto tr14;
		case 126: goto tr14;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr11;
		} else if ( (*p) >= 9 )
			goto st4;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 71 ) {
			if ( 65 <= (*p) && (*p) <= 70 )
				goto tr13;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr14;
		} else
			goto tr14;
	} else
		goto tr12;
	goto st0;
tr11:
#line 14 "ragel/./text-grammar.rl"
	{ intb = p; }
#line 23 "ragel/./text-grammar.rl"
	{ floatb = p; }
	goto st5;
tr151:
#line 25 "ragel/./text-grammar.rl"
	{  }
#line 14 "ragel/./text-grammar.rl"
	{ intb = p; }
#line 23 "ragel/./text-grammar.rl"
	{ floatb = p; }
	goto st5;
st5:
	if ( ++p == pe )
		goto _test_eof5;
case 5:
#line 717 "ron/text-parser.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st6;
	goto st0;
st6:
	if ( ++p == pe )
		goto _test_eof6;
case 6:
	switch( (*p) ) {
		case 32: goto tr16;
		case 33: goto tr17;
		case 39: goto tr18;
		case 44: goto tr17;
		case 46: goto st7;
		case 59: goto tr17;
		case 61: goto tr20;
		case 62: goto tr21;
		case 63: goto tr17;
		case 69: goto st19;
		case 94: goto tr23;
		case 101: goto st19;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st6;
	} else if ( (*p) >= 9 )
		goto tr16;
	goto st0;
st7:
	if ( ++p == pe )
		goto _test_eof7;
case 7:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st8;
	goto st0;
st8:
	if ( ++p == pe )
		goto _test_eof8;
case 8:
	switch( (*p) ) {
		case 32: goto tr25;
		case 33: goto tr26;
		case 39: goto tr27;
		case 44: goto tr26;
		case 59: goto tr26;
		case 61: goto tr28;
		case 62: goto tr29;
		case 63: goto tr26;
		case 69: goto st19;
		case 94: goto tr30;
		case 101: goto st19;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st8;
	} else if ( (*p) >= 9 )
		goto tr25;
	goto st0;
tr20:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
	goto st9;
tr28:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
	goto st9;
tr42:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st9;
tr50:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st9;
tr65:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st9;
tr75:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st9;
tr82:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st9;
tr92:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st9;
tr98:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st9;
tr110:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st9;
tr123:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
	goto st9;
tr131:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
	goto st9;
tr141:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st9;
tr154:
#line 25 "ragel/./text-grammar.rl"
	{  }
	goto st9;
st9:
	if ( ++p == pe )
		goto _test_eof9;
case 9:
#line 924 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st9;
		case 43: goto tr31;
		case 45: goto tr31;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr32;
	} else if ( (*p) >= 9 )
		goto st9;
	goto st0;
tr31:
#line 14 "ragel/./text-grammar.rl"
	{ intb = p; }
	goto st10;
st10:
	if ( ++p == pe )
		goto _test_eof10;
case 10:
#line 944 "ron/text-parser.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st11;
	goto st0;
tr32:
#line 14 "ragel/./text-grammar.rl"
	{ intb = p; }
	goto st11;
st11:
	if ( ++p == pe )
		goto _test_eof11;
case 11:
#line 956 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr16;
		case 33: goto tr17;
		case 39: goto tr18;
		case 44: goto tr17;
		case 59: goto tr17;
		case 61: goto tr20;
		case 62: goto tr21;
		case 63: goto tr17;
		case 94: goto tr23;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st11;
	} else if ( (*p) >= 9 )
		goto tr16;
	goto st0;
tr21:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
	goto st12;
tr29:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
	goto st12;
tr43:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st12;
tr51:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st12;
tr66:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st12;
tr76:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st12;
tr83:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st12;
tr93:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st12;
tr99:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st12;
tr111:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st12;
tr124:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
	goto st12;
tr132:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
	goto st12;
tr142:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st12;
tr155:
#line 25 "ragel/./text-grammar.rl"
	{  }
	goto st12;
st12:
	if ( ++p == pe )
		goto _test_eof12;
case 12:
#line 1123 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st12;
		case 95: goto tr35;
		case 126: goto tr35;
	}
	if ( (*p) < 65 ) {
		if ( (*p) > 13 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto tr34;
		} else if ( (*p) >= 9 )
			goto st12;
	} else if ( (*p) > 70 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr35;
		} else if ( (*p) >= 71 )
			goto tr35;
	} else
		goto tr34;
	goto st0;
tr34:
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st13;
st13:
	if ( ++p == pe )
		goto _test_eof13;
case 13:
#line 1154 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr36;
		case 33: goto tr37;
		case 39: goto tr39;
		case 44: goto tr37;
		case 47: goto tr40;
		case 59: goto tr37;
		case 61: goto tr42;
		case 62: goto tr43;
		case 63: goto tr37;
		case 94: goto tr44;
		case 95: goto st23;
		case 126: goto st23;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr38;
		} else if ( (*p) >= 9 )
			goto tr36;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st23;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st23;
		} else
			goto st23;
	} else
		goto tr38;
	goto st0;
tr38:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 9 "ragel/././uuid-grammar.rl"
	{ version = (*p); }
	goto st14;
st14:
	if ( ++p == pe )
		goto _test_eof14;
case 14:
#line 1197 "ron/text-parser.cc"
	switch( (*p) ) {
		case 95: goto tr45;
		case 126: goto tr45;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr45;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr45;
	} else
		goto tr45;
	goto st0;
tr45:
#line 10 "ragel/././uuid-grammar.rl"
	{ origin.SetTo(p); }
	goto st15;
st15:
	if ( ++p == pe )
		goto _test_eof15;
case 15:
#line 1219 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr46;
		case 33: goto tr47;
		case 39: goto tr48;
		case 44: goto tr47;
		case 59: goto tr47;
		case 61: goto tr50;
		case 62: goto tr51;
		case 63: goto tr47;
		case 94: goto tr52;
		case 95: goto st15;
		case 126: goto st15;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr46;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st15;
		} else if ( (*p) >= 65 )
			goto st15;
	} else
		goto st15;
	goto st0;
tr23:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
	goto st16;
tr30:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
	goto st16;
tr44:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st16;
tr52:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 26 "ragel/./text-grammar.rl"
	{
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st16;
tr69:
#line 15 "ragel/./text-grammar.rl"
	{ 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st16;
tr77:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st16;
tr84:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st16;
tr94:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st16;
tr100:
#line 24 "ragel/./text-grammar.rl"
	{ std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 29 "ragel/./text-grammar.rl"
	{ 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
	goto st16;
tr112:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st16;
tr125:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
	goto st16;
tr133:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 10 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
	goto st16;
tr143:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st16;
tr159:
#line 25 "ragel/./text-grammar.rl"
	{  }
	goto st16;
st16:
	if ( ++p == pe )
		goto _test_eof16;
case 16:
#line 1394 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st16;
		case 43: goto tr53;
		case 45: goto tr53;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr54;
	} else if ( (*p) >= 9 )
		goto st16;
	goto st0;
tr53:
#line 23 "ragel/./text-grammar.rl"
	{ floatb = p; }
	goto st17;
st17:
	if ( ++p == pe )
		goto _test_eof17;
case 17:
#line 1414 "ron/text-parser.cc"
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st18;
	goto st0;
tr54:
#line 23 "ragel/./text-grammar.rl"
	{ floatb = p; }
	goto st18;
st18:
	if ( ++p == pe )
		goto _test_eof18;
case 18:
#line 1426 "ron/text-parser.cc"
	switch( (*p) ) {
		case 46: goto st7;
		case 69: goto st19;
		case 101: goto st19;
	}
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st18;
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
		goto st21;
	goto st0;
st20:
	if ( ++p == pe )
		goto _test_eof20;
case 20:
	if ( 48 <= (*p) && (*p) <= 57 )
		goto st21;
	goto st0;
st21:
	if ( ++p == pe )
		goto _test_eof21;
case 21:
	switch( (*p) ) {
		case 32: goto tr25;
		case 33: goto tr26;
		case 39: goto tr27;
		case 44: goto tr26;
		case 59: goto tr26;
		case 61: goto tr28;
		case 62: goto tr29;
		case 63: goto tr26;
		case 94: goto tr30;
	}
	if ( (*p) > 13 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto st21;
	} else if ( (*p) >= 9 )
		goto tr25;
	goto st0;
tr40:
#line 6 "ragel/././uuid-grammar.rl"
	{ variety = *(p-1); }
	goto st22;
st22:
	if ( ++p == pe )
		goto _test_eof22;
case 22:
#line 1482 "ron/text-parser.cc"
	switch( (*p) ) {
		case 95: goto tr58;
		case 126: goto tr58;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr58;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr58;
	} else
		goto tr58;
	goto st0;
tr35:
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st23;
tr58:
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st23;
st23:
	if ( ++p == pe )
		goto _test_eof23;
case 23:
#line 1510 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr36;
		case 33: goto tr37;
		case 39: goto tr39;
		case 44: goto tr37;
		case 59: goto tr37;
		case 61: goto tr42;
		case 62: goto tr43;
		case 63: goto tr37;
		case 94: goto tr44;
		case 95: goto st23;
		case 126: goto st23;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr38;
		} else if ( (*p) >= 9 )
			goto tr36;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st23;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st23;
		} else
			goto st23;
	} else
		goto tr38;
	goto st0;
tr12:
#line 14 "ragel/./text-grammar.rl"
	{ intb = p; }
#line 23 "ragel/./text-grammar.rl"
	{ floatb = p; }
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st24;
tr153:
#line 25 "ragel/./text-grammar.rl"
	{  }
#line 14 "ragel/./text-grammar.rl"
	{ intb = p; }
#line 23 "ragel/./text-grammar.rl"
	{ floatb = p; }
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st24;
st24:
	if ( ++p == pe )
		goto _test_eof24;
case 24:
#line 1568 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr59;
		case 33: goto tr60;
		case 39: goto tr62;
		case 44: goto tr60;
		case 46: goto st7;
		case 47: goto tr63;
		case 59: goto tr60;
		case 61: goto tr65;
		case 62: goto tr66;
		case 63: goto tr60;
		case 69: goto st30;
		case 94: goto tr69;
		case 95: goto st28;
		case 101: goto st30;
		case 126: goto st28;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr61;
		} else if ( (*p) >= 9 )
			goto tr59;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st29;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st28;
		} else
			goto st28;
	} else
		goto tr61;
	goto st0;
tr61:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 9 "ragel/././uuid-grammar.rl"
	{ version = (*p); }
	goto st25;
st25:
	if ( ++p == pe )
		goto _test_eof25;
case 25:
#line 1614 "ron/text-parser.cc"
	switch( (*p) ) {
		case 95: goto tr70;
		case 126: goto tr70;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr70;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr70;
	} else
		goto tr70;
	goto st0;
tr70:
#line 10 "ragel/././uuid-grammar.rl"
	{ origin.SetTo(p); }
	goto st26;
st26:
	if ( ++p == pe )
		goto _test_eof26;
case 26:
#line 1636 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr71;
		case 33: goto tr72;
		case 39: goto tr73;
		case 44: goto tr72;
		case 59: goto tr72;
		case 61: goto tr75;
		case 62: goto tr76;
		case 63: goto tr72;
		case 94: goto tr77;
		case 95: goto st26;
		case 126: goto st26;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr71;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st26;
		} else if ( (*p) >= 65 )
			goto st26;
	} else
		goto st26;
	goto st0;
tr63:
#line 6 "ragel/././uuid-grammar.rl"
	{ variety = *(p-1); }
	goto st27;
st27:
	if ( ++p == pe )
		goto _test_eof27;
case 27:
#line 1670 "ron/text-parser.cc"
	switch( (*p) ) {
		case 95: goto tr78;
		case 126: goto tr78;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr78;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr78;
	} else
		goto tr78;
	goto st0;
tr14:
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st28;
tr78:
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st28;
tr158:
#line 25 "ragel/./text-grammar.rl"
	{  }
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st28;
st28:
	if ( ++p == pe )
		goto _test_eof28;
case 28:
#line 1706 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr79;
		case 33: goto tr80;
		case 39: goto tr81;
		case 44: goto tr80;
		case 59: goto tr80;
		case 61: goto tr82;
		case 62: goto tr83;
		case 63: goto tr80;
		case 94: goto tr84;
		case 95: goto st28;
		case 126: goto st28;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr61;
		} else if ( (*p) >= 9 )
			goto tr79;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st28;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st28;
		} else
			goto st28;
	} else
		goto tr61;
	goto st0;
st29:
	if ( ++p == pe )
		goto _test_eof29;
case 29:
	switch( (*p) ) {
		case 32: goto tr59;
		case 33: goto tr60;
		case 39: goto tr62;
		case 44: goto tr60;
		case 46: goto st7;
		case 59: goto tr60;
		case 61: goto tr65;
		case 62: goto tr66;
		case 63: goto tr60;
		case 69: goto st30;
		case 94: goto tr69;
		case 95: goto st28;
		case 101: goto st30;
		case 126: goto st28;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr61;
		} else if ( (*p) >= 9 )
			goto tr59;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st29;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st28;
		} else
			goto st28;
	} else
		goto tr61;
	goto st0;
st30:
	if ( ++p == pe )
		goto _test_eof30;
case 30:
	switch( (*p) ) {
		case 32: goto tr79;
		case 33: goto tr80;
		case 39: goto tr81;
		case 44: goto tr80;
		case 59: goto tr80;
		case 61: goto tr82;
		case 62: goto tr83;
		case 63: goto tr80;
		case 94: goto tr84;
		case 95: goto st28;
		case 126: goto st28;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr61;
		} else if ( (*p) >= 9 )
			goto tr79;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st33;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st28;
		} else
			goto st28;
	} else
		goto tr85;
	goto st0;
tr85:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 9 "ragel/././uuid-grammar.rl"
	{ version = (*p); }
	goto st31;
st31:
	if ( ++p == pe )
		goto _test_eof31;
case 31:
#line 1821 "ron/text-parser.cc"
	switch( (*p) ) {
		case 95: goto tr70;
		case 126: goto tr70;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr87;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr70;
	} else
		goto tr70;
	goto st0;
tr87:
#line 10 "ragel/././uuid-grammar.rl"
	{ origin.SetTo(p); }
	goto st32;
st32:
	if ( ++p == pe )
		goto _test_eof32;
case 32:
#line 1843 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr88;
		case 33: goto tr89;
		case 39: goto tr90;
		case 44: goto tr89;
		case 59: goto tr89;
		case 61: goto tr92;
		case 62: goto tr93;
		case 63: goto tr89;
		case 94: goto tr94;
		case 95: goto st26;
		case 126: goto st26;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr88;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st26;
		} else if ( (*p) >= 65 )
			goto st26;
	} else
		goto st32;
	goto st0;
st33:
	if ( ++p == pe )
		goto _test_eof33;
case 33:
	switch( (*p) ) {
		case 32: goto tr95;
		case 33: goto tr96;
		case 39: goto tr97;
		case 44: goto tr96;
		case 59: goto tr96;
		case 61: goto tr98;
		case 62: goto tr99;
		case 63: goto tr96;
		case 94: goto tr100;
		case 95: goto st28;
		case 126: goto st28;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr61;
		} else if ( (*p) >= 9 )
			goto tr95;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st33;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st28;
		} else
			goto st28;
	} else
		goto tr61;
	goto st0;
tr13:
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st34;
tr157:
#line 25 "ragel/./text-grammar.rl"
	{  }
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st34;
st34:
	if ( ++p == pe )
		goto _test_eof34;
case 34:
#line 1922 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr79;
		case 33: goto tr80;
		case 39: goto tr81;
		case 44: goto tr80;
		case 47: goto tr63;
		case 59: goto tr80;
		case 61: goto tr82;
		case 62: goto tr83;
		case 63: goto tr80;
		case 94: goto tr84;
		case 95: goto st28;
		case 126: goto st28;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr61;
		} else if ( (*p) >= 9 )
			goto tr79;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st28;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st28;
		} else
			goto st28;
	} else
		goto tr61;
	goto st0;
tr152:
#line 25 "ragel/./text-grammar.rl"
	{  }
	goto st50;
st50:
	if ( ++p == pe )
		goto _test_eof50;
case 50:
#line 1963 "ron/text-parser.cc"
	goto st0;
tr156:
#line 25 "ragel/./text-grammar.rl"
	{  }
	goto st35;
st35:
	if ( ++p == pe )
		goto _test_eof35;
case 35:
#line 1973 "ron/text-parser.cc"
	switch( (*p) ) {
		case 95: goto tr102;
		case 126: goto tr102;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr101;
	} else if ( (*p) > 70 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr102;
		} else if ( (*p) >= 71 )
			goto tr102;
	} else
		goto tr101;
	goto st0;
tr101:
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st36;
st36:
	if ( ++p == pe )
		goto _test_eof36;
case 36:
#line 2000 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr103;
		case 33: goto tr104;
		case 39: goto tr106;
		case 44: goto tr104;
		case 47: goto tr107;
		case 58: goto tr109;
		case 59: goto tr104;
		case 61: goto tr110;
		case 62: goto tr111;
		case 63: goto tr104;
		case 94: goto tr112;
		case 95: goto st47;
		case 126: goto st47;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr105;
		} else if ( (*p) >= 9 )
			goto tr103;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st47;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st47;
		} else
			goto st47;
	} else
		goto tr105;
	goto st0;
tr103:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st37;
tr136:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st37;
st37:
	if ( ++p == pe )
		goto _test_eof37;
case 37:
#line 2060 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto st37;
		case 33: goto tr6;
		case 39: goto st1;
		case 44: goto tr6;
		case 58: goto st38;
		case 59: goto tr6;
		case 61: goto st9;
		case 62: goto st12;
		case 63: goto tr6;
		case 94: goto st16;
		case 95: goto tr14;
		case 126: goto tr14;
	}
	if ( (*p) < 48 ) {
		if ( (*p) > 13 ) {
			if ( 43 <= (*p) && (*p) <= 45 )
				goto tr11;
		} else if ( (*p) >= 9 )
			goto st37;
	} else if ( (*p) > 57 ) {
		if ( (*p) < 71 ) {
			if ( 65 <= (*p) && (*p) <= 70 )
				goto tr13;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr14;
		} else
			goto tr14;
	} else
		goto tr12;
	goto st0;
tr109:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st38;
tr140:
#line 11 "ragel/././uuid-grammar.rl"
	{ origin.ExtendUntil(p); }
#line 12 "ragel/././uuid-grammar.rl"
	{ uuid.ExtendUntil(p); }
#line 6 "ragel/./text-grammar.rl"
	{
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
	goto st38;
st38:
	if ( ++p == pe )
		goto _test_eof38;
case 38:
#line 2119 "ron/text-parser.cc"
	switch( (*p) ) {
		case 95: goto tr116;
		case 126: goto tr116;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr115;
	} else if ( (*p) > 70 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto tr116;
		} else if ( (*p) >= 71 )
			goto tr116;
	} else
		goto tr115;
	goto st0;
tr115:
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st39;
st39:
	if ( ++p == pe )
		goto _test_eof39;
case 39:
#line 2146 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr117;
		case 33: goto tr118;
		case 39: goto tr120;
		case 44: goto tr118;
		case 47: goto tr121;
		case 59: goto tr118;
		case 61: goto tr123;
		case 62: goto tr124;
		case 63: goto tr118;
		case 94: goto tr125;
		case 95: goto st43;
		case 126: goto st43;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr119;
		} else if ( (*p) >= 9 )
			goto tr117;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st43;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st43;
		} else
			goto st43;
	} else
		goto tr119;
	goto st0;
tr119:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 9 "ragel/././uuid-grammar.rl"
	{ version = (*p); }
	goto st40;
st40:
	if ( ++p == pe )
		goto _test_eof40;
case 40:
#line 2189 "ron/text-parser.cc"
	switch( (*p) ) {
		case 95: goto tr126;
		case 126: goto tr126;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr126;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr126;
	} else
		goto tr126;
	goto st0;
tr126:
#line 10 "ragel/././uuid-grammar.rl"
	{ origin.SetTo(p); }
	goto st41;
st41:
	if ( ++p == pe )
		goto _test_eof41;
case 41:
#line 2211 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr127;
		case 33: goto tr128;
		case 39: goto tr129;
		case 44: goto tr128;
		case 59: goto tr128;
		case 61: goto tr131;
		case 62: goto tr132;
		case 63: goto tr128;
		case 94: goto tr133;
		case 95: goto st41;
		case 126: goto st41;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr127;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st41;
		} else if ( (*p) >= 65 )
			goto st41;
	} else
		goto st41;
	goto st0;
tr121:
#line 6 "ragel/././uuid-grammar.rl"
	{ variety = *(p-1); }
	goto st42;
st42:
	if ( ++p == pe )
		goto _test_eof42;
case 42:
#line 2245 "ron/text-parser.cc"
	switch( (*p) ) {
		case 95: goto tr134;
		case 126: goto tr134;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr134;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr134;
	} else
		goto tr134;
	goto st0;
tr116:
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st43;
tr134:
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st43;
st43:
	if ( ++p == pe )
		goto _test_eof43;
case 43:
#line 2273 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr117;
		case 33: goto tr118;
		case 39: goto tr120;
		case 44: goto tr118;
		case 59: goto tr118;
		case 61: goto tr123;
		case 62: goto tr124;
		case 63: goto tr118;
		case 94: goto tr125;
		case 95: goto st43;
		case 126: goto st43;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr119;
		} else if ( (*p) >= 9 )
			goto tr117;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st43;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st43;
		} else
			goto st43;
	} else
		goto tr119;
	goto st0;
tr105:
#line 8 "ragel/././uuid-grammar.rl"
	{ value.ExtendUntil(p); }
#line 9 "ragel/././uuid-grammar.rl"
	{ version = (*p); }
	goto st44;
st44:
	if ( ++p == pe )
		goto _test_eof44;
case 44:
#line 2315 "ron/text-parser.cc"
	switch( (*p) ) {
		case 95: goto tr135;
		case 126: goto tr135;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr135;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr135;
	} else
		goto tr135;
	goto st0;
tr135:
#line 10 "ragel/././uuid-grammar.rl"
	{ origin.SetTo(p); }
	goto st45;
st45:
	if ( ++p == pe )
		goto _test_eof45;
case 45:
#line 2337 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr136;
		case 33: goto tr137;
		case 39: goto tr138;
		case 44: goto tr137;
		case 58: goto tr140;
		case 59: goto tr137;
		case 61: goto tr141;
		case 62: goto tr142;
		case 63: goto tr137;
		case 94: goto tr143;
		case 95: goto st45;
		case 126: goto st45;
	}
	if ( (*p) < 48 ) {
		if ( 9 <= (*p) && (*p) <= 13 )
			goto tr136;
	} else if ( (*p) > 57 ) {
		if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st45;
		} else if ( (*p) >= 65 )
			goto st45;
	} else
		goto st45;
	goto st0;
tr107:
#line 6 "ragel/././uuid-grammar.rl"
	{ variety = *(p-1); }
	goto st46;
st46:
	if ( ++p == pe )
		goto _test_eof46;
case 46:
#line 2372 "ron/text-parser.cc"
	switch( (*p) ) {
		case 95: goto tr144;
		case 126: goto tr144;
	}
	if ( (*p) < 65 ) {
		if ( 48 <= (*p) && (*p) <= 57 )
			goto tr144;
	} else if ( (*p) > 90 ) {
		if ( 97 <= (*p) && (*p) <= 122 )
			goto tr144;
	} else
		goto tr144;
	goto st0;
tr102:
#line 5 "ragel/././uuid-grammar.rl"
	{ uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st47;
tr144:
#line 7 "ragel/././uuid-grammar.rl"
	{ value.SetTo(p); }
	goto st47;
st47:
	if ( ++p == pe )
		goto _test_eof47;
case 47:
#line 2400 "ron/text-parser.cc"
	switch( (*p) ) {
		case 32: goto tr103;
		case 33: goto tr104;
		case 39: goto tr106;
		case 44: goto tr104;
		case 58: goto tr109;
		case 59: goto tr104;
		case 61: goto tr110;
		case 62: goto tr111;
		case 63: goto tr104;
		case 94: goto tr112;
		case 95: goto st47;
		case 126: goto st47;
	}
	if ( (*p) < 43 ) {
		if ( (*p) > 13 ) {
			if ( 36 <= (*p) && (*p) <= 37 )
				goto tr105;
		} else if ( (*p) >= 9 )
			goto tr103;
	} else if ( (*p) > 45 ) {
		if ( (*p) < 65 ) {
			if ( 48 <= (*p) && (*p) <= 57 )
				goto st47;
		} else if ( (*p) > 90 ) {
			if ( 97 <= (*p) && (*p) <= 122 )
				goto st47;
		} else
			goto st47;
	} else
		goto tr105;
	goto st0;
	}
	_test_eof48: cs = 48; goto _test_eof; 
	_test_eof49: cs = 49; goto _test_eof; 
	_test_eof1: cs = 1; goto _test_eof; 
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
	_test_eof23: cs = 23; goto _test_eof; 
	_test_eof24: cs = 24; goto _test_eof; 
	_test_eof25: cs = 25; goto _test_eof; 
	_test_eof26: cs = 26; goto _test_eof; 
	_test_eof27: cs = 27; goto _test_eof; 
	_test_eof28: cs = 28; goto _test_eof; 
	_test_eof29: cs = 29; goto _test_eof; 
	_test_eof30: cs = 30; goto _test_eof; 
	_test_eof31: cs = 31; goto _test_eof; 
	_test_eof32: cs = 32; goto _test_eof; 
	_test_eof33: cs = 33; goto _test_eof; 
	_test_eof34: cs = 34; goto _test_eof; 
	_test_eof50: cs = 50; goto _test_eof; 
	_test_eof35: cs = 35; goto _test_eof; 
	_test_eof36: cs = 36; goto _test_eof; 
	_test_eof37: cs = 37; goto _test_eof; 
	_test_eof38: cs = 38; goto _test_eof; 
	_test_eof39: cs = 39; goto _test_eof; 
	_test_eof40: cs = 40; goto _test_eof; 
	_test_eof41: cs = 41; goto _test_eof; 
	_test_eof42: cs = 42; goto _test_eof; 
	_test_eof43: cs = 43; goto _test_eof; 
	_test_eof44: cs = 44; goto _test_eof; 
	_test_eof45: cs = 45; goto _test_eof; 
	_test_eof46: cs = 46; goto _test_eof; 
	_test_eof47: cs = 47; goto _test_eof; 

	_test_eof: {}
	if ( p == eof )
	{
	switch ( cs ) {
	case 49: 
#line 25 "ragel/./text-grammar.rl"
	{  }
	break;
#line 2493 "ron/text-parser.cc"
	}
	}

	_out: {}
	}

#line 60 "ragel/text-parser.rl"


    off_ = p-pb;

    if (op_.size()) prev_id_ = op_.id();

    std::cerr << "ending with [" <<p<<"] state "<<cs<<" "<<op_.size()<<" atoms "<<(pe-p)<<" bytes left, prev_id_ "<<prev_id_.str()<<'\n';

    if (term && cs!=RON_error) {
        return true;
    } else {
        cs = RON_error;
        return false;
    }

}

}
