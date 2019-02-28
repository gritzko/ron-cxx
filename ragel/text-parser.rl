#include "text.hpp"
#include <iostream>

namespace ron {

%% machine RON;
%% write data;


Status TextFrame::Cursor::Next () {

    Atoms& atoms = op_.atoms_;

    int line=1;


    switch (cs) {
        case RON_error:
            if (off_!=0) {
                return Status::BAD_STATE;
            }
            %% write init;
            break;

        case RON_FULL_STOP:
            cs = RON_error;
            return Status::ENDOFFRAME;

        default:
            break;
    }

    if (data().size()<=off_) {
        cs = RON_error;
        return Status::ENDOFFRAME;
    }

    slice_t body{data()};
    const char* pb = body.buf_;
    const char* p = pb + off_;
    const char* pe = pb + body.size();
    const char* eof = pe;
    const char* lineb = pb;
    slice_t intb{p,0};
    slice_t floatb{p,0};
    slice_t strb{p,0};
    slice_t uuidb{p,0};
    const char* lastintb{0};
    char term{0};
    slice_t value, origin;
    char variety, version;

    atoms.clear();
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
        op_.term_ = TERM(strchr(TERM_PUNCT, term) - TERM_PUNCT); // FIXME gen a fn
        return Status::OK;
    } else {
        cs = RON_error;
        char msg[64];
        size_t msglen = sprintf(msg, "syntax error at line %d col %d (offset %d)", line, (int)(p-lineb), (int)(p-pb));
        return Status::BAD_STATE.comment(std::string{msg, msglen});
    }

}

}
