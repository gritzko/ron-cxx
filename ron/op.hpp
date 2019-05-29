#ifndef ron_op_hpp
#define ron_op_hpp
#include <cassert>
#include <vector>
#include "encdec.hpp"
#include "status.hpp"
#include "uuid.hpp"

namespace ron {

// An op in the nominal RON (open) coding.
// That's the internal format
struct Op {
    Atoms atoms_;
    Codepoints strings_;

    const Atoms& op() const { return atoms_; }
    inline Uuid id() const { return A2U(atoms_[0]); }
    inline Uuid ref() const { return A2U(atoms_[1]); }

    Result NextCodepoint(Atom& a) const {
        if (!a.value.cp_size) {
            a.value.cp = 0;
            return OUTOFRANGE;
        }
        a.value.cp = strings_[a.origin.as_range.begin()];
        --a.value.cp_size;
        ++a.origin.as_range;
        return OK;
    }

    // terminates the op
    void WriteAtoms() {}

    template <typename... Ts>
    void WriteAtoms(Integer value, Ts... args) {
        atoms_.push_back(Atom::Integer(value));
        WriteAtoms(args...);
    }

    template <typename... Ts>
    void WriteAtoms(Uuid value, Ts... args) {
        atoms_.push_back(value);
        WriteAtoms(args...);
    }

    template <typename... Ts>
    void WriteAtoms(Float value, Ts... args) {
        atoms_.push_back(Atom::Float(value));
        WriteAtoms(args...);
    }

    template <typename... Ts>
    void WriteAtoms(const String& value, Ts... args) {
        fsize_t b = strings_.size();
        Result re = ParseUtf8(strings_, value);
        fsize_t cp_size = strings_.size() - b;
        if (cp_size) {
            Range range{b + 1, FSIZE(strings_.size())};
            atoms_.push_back(Atom::String(strings_[b], range, cp_size - 1));
        } else {
            atoms_.push_back(Atom::String(0, Range{b, b}, cp_size));
        }
        WriteAtoms(args...);
    }

    /** A convenience API method to add an op with any number of atoms. */
    template <typename... Ts>
    Op(Uuid id, Uuid ref, Ts... args) {
        atoms_.clear();
        atoms_.push_back(id);
        atoms_.push_back(ref);
        WriteAtoms(args...);
    }

    template <typename... Ts>
    Op(const String& id, const String& ref, Ts... args)
        : Op{Uuid{id}, Uuid{ref}, args...} {}

    template <class Cursor>
    Result WriteValues(const Cursor& cur) {
        const Atoms& op = cur.op();
        for (fsize_t i = 2; i < op.size(); ++i) {
            Atom a = op[i];
            if (a.type() != STRING) {
                atoms_.push_back(a);
                continue;
            }
            fsize_t b = strings_.size();
            fsize_t cp_size{0};
            while (a.value.cp_size) {  // Oopsie!!! FIXME
                strings_.push_back(a.value.cp);
                cur.NextCodepoint(a);
                ++cp_size;
            }
            Range range{b, FSIZE(strings_.size())};
            atoms_.push_back(Atom::String(strings_[b], range, cp_size));
        }
        return OK;
    }

    template <class Cursor>
    static Op Amend(Uuid id, Uuid ref, const Cursor& cur) {
        Op ret{id, ref};
        ret.WriteValues(cur);
        return ret;
    }
};

}  // namespace ron

#endif
