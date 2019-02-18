#ifndef ron_frame_hpp
#define ron_frame_hpp
#include <cassert>
#include <vector>
#include "uuid.hpp"

namespace ron {

typedef std::vector<Atom> Atoms;

// An op in the nominal RON (open) coding.
// That's the internal format
struct Op {
    Atoms atoms_;
    TERM term_;

    Op(const Op& op) : atoms_{op.atoms_}, term_{op.term_} {}
    explicit Op(TERM term) : atoms_{}, term_{term} { atoms_.reserve(4); }
    explicit Op() : Op{TERM::RAW} {}
    Op(const Uuid& id, const Uuid& ref) : Op{TERM::HEADER} {
        atoms_.push_back(id);
        atoms_.push_back(ref);
    }
    Op(const Uuid& id, const Uuid& ref, const Atom& v0) : Op{id, ref} {
        atoms_.push_back(v0);
        term_ = TERM::RAW;
    }
    Op(const Uuid& id, const Uuid& ref, const Atom& v0, const Atom& v1)
        : Op{id, ref, v0} {
        atoms_.push_back(v1);
    }
    inline TERM term() const { return term_; }
    const Uuid& id() const { return (Uuid&)atoms_[0]; }
    const Uuid& ref() const { return (Uuid&)atoms_[1]; }
    const Atom& atom(fsize_t idx) const { return atoms_[idx]; }
    inline ATOM type(fsize_t idx) const { return atom(idx).type(); }
    inline Uuid uuid(fsize_t idx) const {
        return reinterpret_cast<const Uuid&>(atoms_[idx]);
    }
    fsize_t size() const { return (fsize_t)atoms_.size(); }
    Atoms& data() { return atoms_; }
    void SetId(const Uuid& ev) {
        atoms_.reserve(1);
        atoms_[0] = ev;
    }
    void SetRef(const Uuid& ref) {
        atoms_.reserve(2);
        atoms_[1] = ref;
    }
    inline void AddAtom(Atom value) { atoms_.push_back(value); }
};

}  // namespace ron

#endif
