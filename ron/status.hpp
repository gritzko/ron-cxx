#ifndef ron_status_hpp
#define ron_status_hpp
#include "uuid.hpp"

namespace ron {

struct Status {
    Uuid code_;

    Status() : code_{0,0} {}
    Status(uint64_t err_code) : code_{err_code, Word::MAX_VALUE} {}

    operator bool () const  {
        return code_[VALUE]==0;
    }

    static const Status OK;
    static const Status NOT_FOUND;
};

const Status Status::OK{};
const Status Status::NOT_FOUND{428933766657507328UL};

}

#endif