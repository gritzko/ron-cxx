#ifndef RON_INMEM_STORE_HPP
#define RON_INMEM_STORE_HPP
#include <btree_map.h>
#include <string>
#include <vector>
#include "../rdt/rdt.hpp"
#include "../ron/ron.hpp"
#include "key.hpp"

using namespace btree;

namespace ron {

template <class FrameP>
class InMemoryStore {
   public:
    using Frame = FrameP;
    using Cursor = typename Frame::Cursor;
    using Builder = typename Frame::Builder;
    using Cursors = typename Frame::Cursors;

   private:
    using Map = btree_multimap<Key, Frame>;
    using MapIter = typename Map::iterator;

    Map state_;

    static Status Merge(Frame& merged, MapIter from, MapIter till) {
        Key key = from->first;
        Cursors inputs;
        while (from != till) {
            assert(from->first == key);
            inputs.push_back(from->second.cursor());
            ++from;
        }
        return MergeCursors(merged, key.form(), inputs);
    }

   public:
    using Record = std::pair<Key, Frame>;
    using Records = std::vector<Record>;

    InMemoryStore() : state_{} {}

    Status Open(Uuid id) {
        Frame meta = OneOp<Frame>(id, YARN_FORM_UUID);
        Key zero{};
        state_.insert(Record{zero, meta});
        state_.insert(Record{Key::END, Frame{}});
        return Status::OK;
    }

    Status Create(Uuid id) { return Status::OK; }

    class Iterator {
        Map& store_;
        MapIter b_, e_;
        Frame merged_;
        fsize_t len_;
        friend class InMemoryStore;

        void scroll() {
            len_ = 0;
            while (e_ != store_.end() && e_->first == b_->first) {
                ++e_;
                ++len_;
            }
        }

       public:
        /** Creates a new iterator positioned at 0 */
        explicit Iterator(InMemoryStore& host)
            : store_{host.state_}, b_{}, e_{}, merged_{}, len_{0} {}

        Cursor value() {
            if (len_ == 0) {
                return Cursor{""};
            }
            if (len_ == 1) {
                return Cursor{b_->second};
            }
            Cursors ins;
            for (auto i = b_; i != e_; ++i) ins.push_back(Cursor{i->second});
            Status ok = MergeCursors(merged_, key().form(), ins);
            return Cursor{merged_};
        }

        inline ron::Key key() const { return len_ == 0 ? Key::END : b_->first; }

        /**
         * Moves to the next record in the store.
         * If already at the Key::END then does nothing.
         * @return OK or error
         */
        Status Next() {
            if (len_ == 0) {
                return Status::BAD_STATE.comment("invalid iterator");
            }
            if (len_ > 1 && !merged_.empty()) {
                store_.erase(b_, e_);
                e_ = store_.insert(Record{key(), std::move(merged_)});
                ++e_;
            }
            b_ = e_;
            merged_.Clear();
            scroll();
            // TODO merge'em here - kill merged_, len_, make val()/key() const
            return Status::OK;
        }

        /**
         * Moves to the given key. If none found, moves to the nearest
         * greater key (if prev==true then nearest lesser key).
         * May end up on zero or Key::END (which are always present).
         * @return OK or error if something really bad happened
         */
        Status SeekTo(ron::Key key, bool prev = false) {
            b_ = store_.lower_bound(key);
            e_ = b_;
            if (b_ == store_.end()) {
                if (prev && !store_.empty()) {
                    --b_;
                    e_ = b_;
                } else {
                    return Status::OK;
                }
            } else if (prev && b_->first != key) {
                if (b_ != store_.begin()) {
                    --b_;
                    e_ = b_;
                } else {
                    b_ = e_ = store_.end();
                    return Status::OK;
                }
            }
            scroll();
            return Status::OK;
        }

        Status Close() {
            b_ = e_ = store_.end();
            len_ = 0;
        }
    };

    Status Write(Key key, const Frame& change) {
        if (key == Key::END)
            return Status::BADARGS.comment("can't write at Key::END");
        state_.insert(Record{key, change});
        return Status::OK;
    }

    Status Read(Key key, Frame& result) {
        Cursors inputs{};
        auto range = state_.equal_range(key);
        if (range.first == range.second) {
            result = Frame{};
            return Status::OK;
        }
        auto i = range.first;
        ++i;
        if (range.second == i) {
            result = range.first->second;
            return Status::OK;
        }
        Status ok = Merge(result, range.first, range.second);
        state_.erase(range.first, range.second);
        state_.insert(Record{key, result});
        return ok;
    }

    Status Write(const Records& batch) {
        Status ok = Status::OK;
        for (auto i = batch.begin(); ok && i != batch.end(); ++i) {
            ok = Write(i->first, i->second);
        }
        return ok;
    }

    Status Compact() { return Status::NOT_IMPLEMENTED; }

    bool Release(Records& data) {
        if (state_.empty()) {
            return false;
        }
        auto i = state_.begin();
        while (i != state_.end()) {
            data.push_back(*i);
            ++i;
        }
        return true;
    }

    Status Close() {
        state_.clear();
        return Status::OK;
    }
};

}  // namespace ron

#endif
