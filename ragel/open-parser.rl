#include "open-text-ron.hpp"
#include <iostream>

namespace ron {

%% machine RON;
%% write data;

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
            %% write init;
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

    %%{
    include OPEN_TEXT_FRAME "./open-grammar.rl";
    main := OPEN_TEXT_FRAME ;
    write exec;
    }%%

    off_ = p-pb;
    pos_++;

    if (cs==RON_error) {
	    return false;
    } else if (cs>=%%{ write first_final; }%%) { // one of end states
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
