#include "text.hpp"

namespace ron {

%% machine RON;
%% write data;
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
            %% write init;
            atoms.resize(SPEC_SIZE);
            break;

        case RON_FULL_STOP:
            cs = RON_error;
            return false;

        case RON_start:
            atoms.resize(SPEC_SIZE);
            atm = 0;
            hlf = 0;
            dgt = 0;
            break;
    }

    if (data.size()<=off) {
        cs = RON_error;
        return false;
    }

    auto p = data.begin() + off;
    auto pb = p;
    auto pe = data.end();
    auto eof = pe;
    int n = 0;

    %%{
    include FRAME "./text-grammar.rl";
    main := FRAME ;
    write exec;
    }%%

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
