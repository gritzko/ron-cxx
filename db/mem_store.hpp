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
        static MasterRDT<Frame> rdt_{};
        Key key = from->first;
        Cursors inputs;
        while (from != till) {
            assert(from->first == key);
            inputs.push_back(from->second.cursor());
            ++from;
        }
        Builder out;
        Status ok = rdt_.Merge(out, key.form(), inputs);
        if (ok) {
            merged = out.Release();
        }
        return ok;
    }

   public:
    using Record = std::pair<Key, Frame>;
    using Records = std::vector<Record>;

    InMemoryStore() : state_{} {}

    Status Open(std::string path, bool create = false) { return Status::OK; }

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
        Iterator(InMemoryStore& host)
            : store_{host.state_}, b_{}, e_{}, merged_{}, len_{0} {}

        Cursor Value() {
            if (b_==store_.end()) { return Cursor{std::string{}}; }
            if (len_ == 1) {
                return Cursor{b_->second};
            }
            Cursors ins;
            for (auto i = b_; i != e_; ++i) ins.push_back(Cursor{i->second});
            Status ok = MergeCursors(merged_, Key().form(), ins);
            return Cursor{merged_};
        }

        inline ron::Key Key() const {
            return b_==store_.end() ? END_KEY : b_->first;
        }

        Status Next() {
            if (len_ > 1 && !merged_.empty()) {
                store_.erase(b_, e_);
                e_ = store_.insert(Record{Key(), std::move(merged_)});
                ++e_;
            }
            b_ = e_;
            merged_.Clear();
            scroll();
            return Status::OK;
        }

        Status SeekTo(ron::Key key, bool prev = false) {
            b_ = store_.lower_bound(key);
            if (prev && b_->first != key) {
                if (b_ != store_.begin()) {
                    --b_;
                } else {
                    b_ = store_.end();
                }
            }
            e_ = b_;
            scroll();
            return Status::OK;  //?! FIXME contract
        }
    };

    Status Write(Key key, const Frame& change) {
        state_.insert(Record{key, change});
        return Status::OK;
    }

    Status Read(Key key, Frame& result) {
        Cursors inputs{};
        auto range = state_.equal_range(key);
        if (range.first == range.second) {
            result = Frame{};
            return Status::NOT_FOUND;
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

    Status Close() {
        state_.clear();
        return Status::OK;
    }
};

}  // namespace ron

#endif
