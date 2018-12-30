#ifndef ron_status_hpp
#define ron_status_hpp
#include "uuid.hpp"

namespace ron {

/** Error codes are RON UUIDs - to serialize them as ops, store, send.
 *  For example, `@error :1gOFEM+gritzko CAUSEBREAK;`
 *  The OK status is 0 (aka "nil UUID"). */
class Status {
    Uuid code_;
    std::string comment_;

   public:
    Status() : code_{0, 0} {}
    explicit Status(uint64_t err_code) : code_{err_code, Word::PAYLOAD_BITS} {}
    Status(const Uuid& code, const std::string& comment)
        : code_{code}, comment_{comment} {}

    inline explicit operator Uuid() const { return code_; }

    inline const Uuid& code() const { return code_; }
    inline const std::string& comment() const { return comment_; }

    inline Status comment(const std::string& add_comment) const {
        return Status{code_, add_comment};
    }

    operator bool() const {
        return code_[ORIGIN] != Word::PAYLOAD_BITS;  // not an error
    }

    std::string str() const { return code().value().str() + '\t' + comment(); }

    static const Status OK;
    static const Status NOT_IMPLEMENTED;
    static const Status NOT_FOUND;
    static const Status BAD_STATE;
    static const Status BADARGS;
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
