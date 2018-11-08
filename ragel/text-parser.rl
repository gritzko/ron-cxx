#include "text.hpp"
#include <iostream>

namespace ron {

%% machine RON;
%% write data;


bool TextFrame::Cursor::Next () {

    Atoms& atoms = op_.atoms_;

    // hint: the machine must start parsing in the "space" state
    // to allow for frames like `1, 2, 3 or 'string'`

    switch (cs) {
        case RON_error:
            if (off_!=0) {
                return false;
            }
            %% write init;
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

    %%{
    include TEXT_FRAME "./text-grammar.rl";
    main := TEXT_FRAME ;
    write exec;
    }%%

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
