#ifndef CPP_RGA_HPP
#define CPP_RGA_HPP

#include <algorithm>
#include <unordered_map>
#include "../ron/ron.hpp"
#include "merge.hpp"

namespace ron {

using namespace std;

/** Implements Causal Tree CRDT, which is mostly the same thing as Replicated
 * Growable Array CRDT. Except, RGA is stored as a tree, while CT is stored
 * as an array. We store everything as a RON frame.
 * We call it use "RGA" cause every RDT here is a CT (in the broad sense). */
template <class Frame>
class RGArrayRDT {
    static bool less_than(const Op &a, const Op &b) { return b.id() < a.id(); }
    using MCursor = MergeCursor<Frame, less_than>;
    using Builder = typename Frame::Builder;
    using Cursor = typename Frame::Cursor;
    using Cursors = typename Frame::Cursors;
    using PCursor = typename Cursors::iterator;

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
        using insmap_t::value_type insmapval_t;
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
        for (i = inputs.begin(), b = 1; i != inputs.end(); i++, b <<= 1) {
            if (i->id() == max->id()) {
                m.Add(i);
                added |= b;
            }
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

        return ++added == 1 << inputs.size()
                   ? Status::OK
                   : Status::CAUSEBREAK.comment("unattacheable pieces");
    }

    Status GC(Builder &output, const Frame &input) const {
        return Status::NOT_IMPLEMENTED;
    }

    Status MergeGC(Builder &output, Cursors &inputs) const {
        return Status::NOT_IMPLEMENTED;
    }
};

enum RGA_ENTRY : uint8_t {
    META_ENTRY = 0,
    ENTRY = 1,
    REMOVE = 2,
    UNDO = 3,
    TRASH = 4
};

inline RGA_ENTRY inc(RGA_ENTRY e) { return RGA_ENTRY(uint8_t(e) + 1); }

inline RGA_ENTRY dec(RGA_ENTRY e) { return RGA_ENTRY(uint8_t(e) - 1); }

const Uuid RM_UUID{986569793370849280UL, 0UL};
const Uuid UN_UUID{1040894463876005888UL, 0UL};
const Uuid IN_UUID{"in"};
const Uuid EQ_UUID{"eq"};

template <typename Cursor>
inline RGA_ENTRY entry_type(const Cursor &cur) {
    if (cur.size() == 3 && cur.has(2, UUID)) {
        // TODO const Uuid& v = cur.uuid(2);
        Uuid v = cur.uuid(2);
        if (v == RM_UUID) {
            return REMOVE;
        }
        if (v == UN_UUID) {
            return UNDO;
        }
    }
    return ENTRY;
}

using Booleans = std::vector<bool>;

/** Scans an RGA frame, produces a map of tombstones
 *  (true for invisible/removed ops, false for visibles). */
template <class Frame>
Status ScanRGA(Booleans &tombstones, const Frame &frame) {
    using Cursor = typename Frame::Cursor;
    tombstones.clear();
    RGA_ENTRY state{META_ENTRY};
    Cursor cur = frame.cursor();
    const Uuid root = cur.id();
    if (cur.ref() != RGA_FORM_UUID || root.version() != TIME)
        return Status::BADARGS.comment("not an RGA/CT frame");
    inc_stack<Uuid> path{};
    inc_stack<fsize_t> positions{};
    path.push_back(root);
    std::vector<bool> kills;
    kills.push_back(false);
    tombstones.push_back(true);
    positions.push_back(0);
    fsize_t depth = 1;
    fsize_t pos = 0;
    fsize_t ceiling[] = {0, 0, 0, 0, 0};
    Uuid id, ref;
    RGA_ENTRY et;

    do {
        cur.Next();
        if (cur.valid()) {
            id = cur.id();
            ref = cur.ref();
            et = entry_type(cur);
            // TODO skip by vv
        } else {
            id = Uuid::Time(NEVER, 0);
            ref = root;
            et = META_ENTRY;
        }
        ++pos;

        // sanity checks
        if (ref > id) return Status::BADARGS.comment("ref/id order reversal");
        if (id.version() != TIME || ref.version() != TIME)
            return Status::BADARGS.comment("invalid event id");

        // unroll the stack, get to the (causal) parent
        while (path.back() != ref) {
            fsize_t at;
            switch (state) {
                case META_ENTRY:
                    return Status::CAUSEBREAK.comment("not a CT");
                case ENTRY:
                    if (!kills.back()) break;
                    at = positions.back();
                    tombstones[at] = true;
                    break;
                case REMOVE:
                    if (kills.back()) break;
                    at = ceiling[REMOVE] - (depth - ceiling[REMOVE]);
                    if (at > 0) {  // aka ceiling[META_ENTRY]
                        kills[at] = true;
                    }
                    break;
                case UNDO:
                    at = ceiling[UNDO] - (depth - ceiling[UNDO]);
                    if (at > ceiling[REMOVE]) {
                        kills[at] = true;
                    }
                    break;
                case TRASH:
                    // a tombstone is already set
                    break;
            }
            --depth;
            while (depth == ceiling[state]) {
                state = dec(state);
            }
            path.pop_back();
            kills.pop_back();
            positions.pop_back();
        }

        // state switch
        // versioning here. future subtrees := TRASH
        if (et != state) {
            if (et != state + 1) {
                while (state < TRASH) {
                    state = inc(state);
                    ceiling[state] = depth;
                }
            } else {
                state = et;
                ceiling[state] = depth;
            }
        }

        tombstones.push_back(state != ENTRY);
        ++depth;
        path.push_back(id);
        kills.push_back(false);
        positions.push_back(pos);

        assert(path.size() == kills.size());
        assert(tombstones.size() == pos + 1);
    } while (cur.valid());

    tombstones.pop_back();

    return Status::OK;
}

}  // namespace ron

#endif  // CPP_RGA_HPP
