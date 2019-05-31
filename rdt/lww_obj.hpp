#ifndef RON_RDT_LWW_OBJ_HPP
#define RON_RDT_LWW_OBJ_HPP
#include <cmath>
#include <unordered_map>
#include "../ron/ron.hpp"

namespace ron {

template <class Frame>
class LWWObject {
    using Cursor = typename Frame::Cursor;

    Cursor cur_;
    std::unordered_map<Uuid, Atom> vals_;
    Uuid last_;

   public:
    void Update(const Cursor& c) {
        if (c.op().size() < 4 || !HasValue(c, UUID)) {
            return;
        }
        vals_[Uuid{c.atom(2)}] = c.atom(3);
        last_ = c.id();
    }

    void Update(Frame frame) {
        Cursor c{frame};
        while (c.Next()) {
            Update(c);
        }
    }

    /** Note that LWWObject does not own the memory. It serves like an index to
     * a LWW frame. */
    LWWObject(const Frame& state) : cur_{state}, vals_{}, last_{} {
        do {
            Update(cur_);
        } while (cur_.Next());
    }

    template <typename... Ts>
    inline Frame Write(Uuid id, Uuid key, Ts... args) {
        last_ = id;
        return OneOp<Frame>(id, last_, key, args...);
    }

    Atom atom(Uuid key, ATOM type) {
        auto i = vals_.find(key);
        if (i == vals_.end() || i->second.type() != type) {
            return Uuid::NIL;
        }
        return i->second;
    }

    /*inline int64_t integer(Uuid key) {
        Atom a = atom(key, INT);
        return Uuid::NIL == a ? 0 : data_.integer(a);
    }*/

    inline Uuid uuid(Uuid key) { return Uuid{atom(key, UUID)}; }

    inline double number(Uuid key) {
        Atom a = atom(key, FLOAT);
        return Uuid::NIL == Uuid{a} ? std::nan("") : a.value.as_integer;
    }

    inline String string(Uuid key) {
        Atom a = atom(key, STRING);
        String ret;
        ReadString(ret, cur_, a);  // TODO error checking, tests :)
        return ret;
    }
};

};  // namespace ron

#endif
