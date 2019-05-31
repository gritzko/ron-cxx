#include "text.hpp"
#include <iostream>

namespace ron {

%% machine RON;
%% write data;


Status TextFrame::Cursor::Next () {

    Atoms& atoms = op_;
    fsize_t &line=line_;
    uint8_t &cs = ragel_state_;
    static_assert(RON_first_final<UINT8_MAX, "this grammar should not change much");

    switch (cs) {
        case RON_error:
            if (data().range().begin()!=0) {
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

    if (data().empty()) {
        cs = RON_error;
        return Status::ENDOFFRAME;
    }

    CharRef buffer = data().buffer();
    CharRef pb = data().begin();
    CharRef p = pb;
    CharRef pe = data().end();
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

    Uuid prev = atoms.empty() ? Uuid::NIL : A2U(atoms[0]);
    atoms.clear();
    op_.push_back(prev.inc());
    op_.push_back(prev);


    %%{
    include TEXT_FRAME "./text-grammar.rl";
    main := TEXT_FRAME ;
    write exec;
    }%%
    data().Consume(p-pb);

    //std::cerr << "ending with [" <<p<<"] state "<<cs<<" "<<op_.size()<<" atoms "<<(pe-p)<<" bytes left, prev_id_ "<<prev_id_.str()<<'\n';

    if (term && cs!=RON_error) {
        term_ = chr2term(term); // FIXME gen a fn
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
