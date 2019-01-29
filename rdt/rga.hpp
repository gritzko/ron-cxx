#ifndef CPP_RGA_HPP
#define CPP_RGA_HPP

#include <algorithm>
#include <unordered_map>
#include "rdt/merge.hpp"

namespace ron {

using namespace std;

/** Implements Causal Tree CRDT, which is mostly the same thing as Replicated
 * Growable Array CRDT. Except, RGA is stored as a tree, while CT is stored
 * as an array. We store everything as a RON frame.
 * We call it use "RGA" cause every RDT here is a CT (in the broad sense). */
template <class Frame>
class RGArrayRDT {
    static bool less_than(const Op &a, const Op &b) { return b.id() < a.id(); }
    typedef MergeCursor<Frame, less_than> MCursor;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef typename Frame::Cursors Cursors;
    typedef typename Cursors::iterator PCursor;

    struct {
        bool operator()(const Cursor &a, const Cursor &b) const {
            return a.id() < b.id();
        }
    } id_cmp;

   public:
    Status MergeBig(Builder &output, Cursors &inputs) const {
        /*
        typedef unordered_multimap<Uuid, Cursor *> insmap_t;
        insmap_t inserts;
        typedef typename insmap_t::value_type insmapval_t;
        insmapval_t least{FATAL, nullptr};
        for (int i = 0; i < inputs.size(); i++) {
            const Cursor &c = inputs[i];
            insmapval_t entry{c.id(), &c};
            if (c.id() < least.first) least = entry;
            inserts.insert(entry);
        }

        MCursor m{};
        m.Add(*least.second);
        inserts.erase(least);  //?
        */
        return Status::NOT_IMPLEMENTED;
    }

    Status Merge(Builder &output, Cursors &inputs) const {
        if (inputs.size() > 3) return MergeBig(output, inputs);

        MCursor m{};
        auto max = std::min_element(inputs.begin(), inputs.end(), id_cmp);
        uint64_t added{0}, b{1};  // TODO refs[MAX_INPUTS]
        PCursor i;
        for (i = inputs.begin(), b = 1; i != inputs.end(); i++, b <<= 1)
            if (i->id() == max->id()) {
                m.Add(i);
                added |= b;
            }

        TERM term = HEADER;
        while (!m.empty()) {
            auto &cur = m.current();
            output.AppendAmendedOp(cur, term, cur.id(), cur.ref());
            term = REDUCED;
            const Uuid id = cur.id();
            m.Next();
            for (i = inputs.begin(), b = 1; i != inputs.end(); i++, b <<= 1)
                if ((b & added) == 0 && i->ref() == id) {
                    m.Add(i);
                    added |= b;
                }
        }

        return ++added == 1 << inputs.size() ? Status::OK : Status::CAUSEBREAK;
    }

    Status GC(Builder &output, const Frame &input) const {
        return Status::NOT_IMPLEMENTED;
    }

    Status MergeGC(Builder &output, Cursors &inputs) const {
        return Status::NOT_IMPLEMENTED;
    }
};

}  // namespace ron

#endif  // CPP_RGA_HPP
