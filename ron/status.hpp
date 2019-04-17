#ifndef ron_status_hpp
#define ron_status_hpp
#include "uuid.hpp"

namespace ron {

/** Error codes are RON UUIDs - to serialize them as ops, store, send.
 *  For example, `@error :1gOFEM+gritzko CAUSEBREAK;`
 *  The OK status is 0 (aka "nil UUID"). */
class Status {
    Uuid code_;
    String comment_;

   public:
    Status() : code_{0, 0} {}
    explicit Status(uint64_t err_code) : code_{err_code, Word::PAYLOAD_BITS} {}
    Status(Uuid code, const String& comment = "")
        : code_{code}, comment_{comment} {}

    inline explicit operator Uuid() const { return code_; }

    inline Uuid code() const { return code_; }
    inline const String& comment() const { return comment_; }

    inline Status comment(const String& add_comment) const {
        return Status{code_, add_comment};
    }

    operator bool() const {
        return code_[ORIGIN] != Word::PAYLOAD_BITS;  // not an error
    }

    bool operator==(const Status& b) const { return code() == b.code(); }
    bool operator!=(const Status& b) const { return code() != b.code(); }

    String str() const { return code().value().str() + "\t" + comment(); }

    static const Status OK;
    static const Status ENDOFFRAME;
    static const Status ENDOFINPUT;
    static const Status NOT_IMPLEMENTED;
    static const Status NOT_FOUND;
    static const Status BAD_STATE;
    static const Status BADARGS;
    static const Status BADSYNTAX;
    static const Status DB_FAIL;
    static const Status IOFAIL;
    static const Status BADFRAME;
    static const Status BADID;
    static const Status BADREF;
    static const Status BADVALUE;

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
    static const Status CONFLICT;
};

#define IFOK(x)          \
    {                    \
        Status ok = (x); \
        if (!ok) {       \
            return ok;   \
        }                \
    }

}  // namespace ron

#endif
