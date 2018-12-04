#ifndef ron_status_hpp
#define ron_status_hpp
#include "uuid.hpp"

namespace ron {

struct Status {
    Uuid code_;

    Status() : code_{0, 0} {}
    Status(uint64_t err_code) : code_{err_code, Word::MAX_VALUE} {}

    operator bool() const {
        return code_[ORIGIN] != Word::PAYLOAD_BITS;  // not an error
    }

    std::string str() const { return code_.str(); }

    static const Status OK;
    static const Status NOT_IMPLEMENTED;
    static const Status NOT_FOUND;
    static const Status BAD_STATE;
    static const Status DB_FAIL;
};

}  // namespace ron

#endif
