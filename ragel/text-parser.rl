#include "text.hpp"
#include <iostream>

namespace ron {

%% machine RON;
%% write data;


bool TextFrame::Cursor::Next () {

    Atoms& atoms = op_.atoms_;




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

    slice_t body{frame_.data()};
    iter pb = data().data();
    iter p = pb + off_;
    iter pe = pb + data().size();
    iter eof = pe;

    slice_t intb{p,0};
    slice_t floatb{p,0};
    slice_t strb{p,0};
    slice_t uuidb{p,0};
    const char* lastintb{0};
    char term{0};
    slice_t value, origin;
    char variety, version;

    atoms.resize(0);
    op_.AddAtom(prev_id_.inc());
    op_.AddAtom(prev_id_);

    //std::cerr<<"starting with "<<cs<<" ["<<p<<"]\n";

    %%{
    include TEXT_FRAME "./text-grammar.rl";
    main := TEXT_FRAME ;
    write exec;
    }%%

    off_ = p-pb;

    if (op_.size()) prev_id_ = op_.id();

    //std::cerr << "ending with [" <<p<<"] state "<<cs<<" "<<op_.size()<<" atoms "<<(pe-p)<<" bytes left, prev_id_ "<<prev_id_.str()<<'\n';

    if (term && cs!=RON_error) {
        return true;
    } else {
        cs = RON_error;
        return false;
    }

}

}
