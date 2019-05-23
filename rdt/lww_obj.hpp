#ifndef RON_RDT_LWW_OBJ_HPP
#define RON_RDT_LWW_OBJ_HPP
#include <cmath>
#include <unordered_map>
#include "../ron/ron.hpp"

namespace ron {

template <class Frame>
class LWWObject {
    using Cursor = typename Frame::Cursor;

    Frame data_;
    std::unordered_map<Uuid, Atom> vals_;
    Uuid last_;

   public:
    void Update(const Cursor& c) {
        if (c.size() < 4 || !c.has(2, UUID)) {
            return;
        }
        vals_[c.uuid(2)] = c.atom(3);
        last_ = c.id();
    }

    void Update(Frame frame) {
        Cursor c{frame, false};
        while (c.Next()) {
            Update(c);
        }
    }

    LWWObject(Frame state) : data_{std::move(state)}, vals_{}, last_{} {
        Cursor c{data_};
        do {
            Update(c);
        } while (c.Next());
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
        return Uuid::NIL == Uuid{a} ? std::nan("") : data_.number(a);
    }

    inline String string(Uuid key) {
        Atom a = atom(key, STRING);
        return Uuid::NIL == Uuid{a} ? "" : data_.string(a);
    }
};

};  // namespace ron

#endif
