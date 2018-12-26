#ifndef ron_status_hpp
#define ron_status_hpp
#include "uuid.hpp"

namespace ron {

/** Error codes are RON UUIDs - to serialize them as ops, store, send.
 *  For example, `@error :1gOFEM+gritzko CAUSEBREAK;`
 *  The OK status is 0 (aka "nil UUID"). */
struct Status {
    Uuid code_;

    Status() : code_{0, 0} {}
    explicit Status(uint64_t err_code) : code_{err_code, 0} {}

    operator bool() const {
        return code_[ORIGIN] != Word::PAYLOAD_BITS;  // not an error
    }

    std::string str() const { return code_.str(); }

    static const Status OK;
    static const Status NOT_IMPLEMENTED;
    static const Status NOT_FOUND;
    static const Status BAD_STATE;
    static const Status DB_FAIL;
    static const Status IOFAIL;

    static const Status NOTYPE;
    static const Status NOTOPEN;

    static const Status CHAINBREAK;
    static const Status HASHBREAK;
    static const Status TREEBREAK;
    static const Status CAUSEBREAK;

    static const Status TREEGAP;
    static const Status YARNGAP;

    static const Status REPEAT;
    static const Status REORDER;
};

}  // namespace ron

#endif
