#ifndef ron_frame_hpp
#define ron_frame_hpp
#include <vector>
#include <cassert>
#include "uuid.hpp"


namespace ron {

typedef std::vector<Atom> Atoms;

// An op in the nominal RON (open) coding.
// That's the internal format
struct Op {
    Atoms atoms_;
    TERM term_;
    Op(const Op& op) : atoms_{op.atoms_}, term_{op.term_} {}
    explicit Op(TERM term) : atoms_{}, term_{term} {}
    explicit Op(fsize_t capacity) : atoms_{capacity}, term_{TERM::RAW} {}
    Op() : Op{4} {}
    Op(const Uuid& id, const Uuid& ref) : atoms_{}, term_{TERM::HEADER} {
        atoms_.push_back(id);
        atoms_.push_back(ref);
    }
    Op(const Uuid& id, const Uuid& ref, const Value& v0) : Op{id, ref} {
        atoms_.push_back(v0);
        term_ = TERM::RAW;
    }
    Op(const Uuid& id, const Uuid& ref, const Value& v0, const Value& v1) : Op{id, ref, v0} {
        atoms_.push_back(v1);
    }
    const Uuid& id() const {
        return (Uuid&)atoms_[0];
    }
    const Uuid& ref() const {
        return (Uuid&)atoms_[1];
    }
    const Value& value(fsize_t idx) const {
        assert(idx+1<atoms_.size());
        return (Value&)atoms_[idx+2];
    }
    fsize_t size() const {
        return (fsize_t) atoms_.size();
    }
    void resize (fsize_t new_size = 2) { atoms_.resize(new_size); }
    Atoms& data() { return atoms_; }
    // a nominal RON (closed) op, with a descriptor
    // mostly for outer interfaces / bindings
    Atom* closed(Uuid rdt, Uuid object) const;
    void SetEv(const Uuid& ev);
    void SetRef(const Uuid& ref);
    void AddValue(const Atom& value);
    void AddFloat(double value);
    void AddString(fsize_t from, fsize_t till);

    const Op& Fill(const Uuid& ev, const Uuid& ref);
    const Op& Fill(const Uuid& ev, const Uuid& ref, const frange_t range);
    const Op& Fill(const Uuid& ev, const Uuid& ref, const Uuid& value);
    const Op& Fill(const Uuid& ev, const Uuid& ref, double value);
    const Op& Fill(const Uuid& ev, const Uuid& ref, int64_t value);
};

typedef int Status; // FIXME


} // namespace ron

#endif