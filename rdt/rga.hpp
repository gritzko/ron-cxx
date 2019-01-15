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
    typedef MergeCursor<Frame, less_than> MergeCursor;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef typename Frame::Cursors Cursors;

    struct {
        bool operator()(const Cursor &a, const Cursor &b) const {
            return a.id() < b.id();
        }
    } id_cmp;

   public:
    Status MergeBig(Builder &output, Cursors &inputs) const {
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

        MergeCursor m{};
        m.Add(*least.second);
        inserts.erase(least);  //?
    }

    Status Merge(Builder &output, Cursors &inputs) const {
        // FIXME Merger should not own the cursors
        MergeCursor m{};
        sort(inputs.begin(), inputs.end(), id_cmp);
        while (!inputs.empty()) {
            Uuid root = inputs.front().id();
            while (!inputs.empty() && inputs.front().id() == root) {
                m.Add(inputs.front());
                inputs.erase(inputs.begin());  // FIXME host destroyed
            }
            // render subtree
            while (!m.empty()) {
                output.AppendOp(m.current());
                const Uuid id = m.current().id();
                m.Next();
                auto i = inputs.begin();
                while (i != inputs.end()) {
                    if (i->ref() == id) {
                        m.Add(*i);
                        inputs.erase(i);
                        i = inputs.begin();
                    } else {
                        i++;
                    }
                }
            }
        }

        return Status::OK;
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
