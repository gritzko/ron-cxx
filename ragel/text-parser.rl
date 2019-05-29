#include "text.hpp"
#include <iostream>

namespace ron {

%% machine RON;
%% write data;


Status TextFrame::Cursor::Next () {

    Atoms& atoms = op_;

    int line=line_;


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

    Slice body{data_};
    CharRef pb = body.begin();
    CharRef p = pb + off_;
    CharRef pe = body.end();
    CharRef eof = pe;
    CharRef lineb = pb;
    CharRef intb{p};
    CharRef floatb{p};
    CharRef strb{p};
    CharRef uuidb{p};
    CharRef wordb{p};
    Codepoint cp{0};
    fsize_t cp_size{0};
    char term{0};
    Slice value, origin;
    char variety{0}, version{0};

    atoms.clear();
    op_.push_back(prev_id_.inc());
    op_.push_back(prev_id_);

    //std::cerr<<"starting with "<<cs<<" ["<<p<<"]\n";

    %%{
    include TEXT_FRAME "./text-grammar.rl";
    main := TEXT_FRAME ;
    write exec;
    }%%
    at_ = off_;
    off_ = p-pb;
    line_ = line;

    if (op_.size()) prev_id_ = id();

    //std::cerr << "ending with [" <<p<<"] state "<<cs<<" "<<op_.size()<<" atoms "<<(pe-p)<<" bytes left, prev_id_ "<<prev_id_.str()<<'\n';

    if (term && cs!=RON_error) {
        term_ = chr2term(term); // FIXME gen a fn
        if ((options_&PARSE_ON_DEMAND)==0) {
            ParseValues();
        }
        return Status::OK;
    } else if (cs>=RON_first_final) {
        cs = RON_error;
        return Status::ENDOFFRAME;
    } else {
        cs = RON_error;
        char msg[64];
        size_t msglen = sprintf((char*)msg, "syntax error at line %d col %d (offset %d)", line, (int)(p-lineb), (int)(p-pb));
        return Status::BAD_STATE.comment(msg);
    }

}

}
