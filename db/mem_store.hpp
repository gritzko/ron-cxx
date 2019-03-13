#ifndef RON_INMEM_STORE_HPP
#define RON_INMEM_STORE_HPP
#include <map>
#include <string>
#include <vector>
#include "key.hpp"
#include "rdt/rdt.hpp"
#include "ron/ron.hpp"

namespace ron {

template <class Frame>
class InMemoryStore {
    using Cursor = typename Frame::Cursor;
    using Builder = typename Frame::Builder;
    using Cursors = typename Frame::Cursors;

    // TODO try https://github.com/JGRennison/cpp-btree
    using Store = std::multimap<Key, Frame>;
    using StoreIter = typename Store::const_iterator;

    Store state_;

   public:
    using Record = std::pair<Key, Frame>;
    using Records = std::vector<Record>;

    static Status Merge(Frame& merged, StoreIter from, StoreIter till) {
        static MasterRDT<Frame> rdt_{};
        Key key = from->first;
        Cursors inputs;
        while (from != till) {
            assert(from->first == key);
            inputs.push_back(from->second.cursor());
            ++from;
        }
        Builder out;
        Status ok = rdt_.Merge(out, key.rdt(), inputs);
        if (ok) {
            merged = out.frame();
        }
        return ok;
    }

    class const_iterator {
        Store& store_;
        StoreIter b_, e_;
        Record merged_;
        fsize_t len_;
        friend class InMemoryStore;
        const_iterator(Store& store, StoreIter at)
            : store_{store}, b_{at}, e_{at}, merged_{}, len_{0} {
            scroll();
        }
        void scroll() {
            len_ = 0;
            while (e_ != store_.end() && e_->first == b_->first) {
                ++e_;
                ++len_;
            }
        }

       public:
        const Record& operator*() {
            assert(b_ != e_);
            if (len_ == 1) {
                merged_ = *b_;
            } else {
                merged_.first = b_->first;
                Merge(merged_.second, b_, e_);
            }
            return merged_;
        }
        void operator++() {
            if (len_ > 1) {
                store_.insert(b_, merged_);
                store_.erase(b_, e_);
            }
            b_ = e_;
            scroll();
        }
        inline bool operator==(const const_iterator& b) const {
            return b_ == b.b_;
        }
    };

    const_iterator begin() { return const_iterator{state_, state_.begin()}; }

    const_iterator end() { return const_iterator{state_, state_.end()}; }

    InMemoryStore() : state_{} {}

    Status Create(std::string path) { return Status::OK; }

    Status Open(std::string path) { return Status::OK; }

    Status Put(Key key, const Frame& state, Uuid branch = Uuid::NIL) {
        auto r = state_.equal_range(key);
        if (r.first != r.second) {
            state_.erase(r.first, r.second);
        }
        return Merge(key, state, branch);
    }

    Status Merge(Key key, const Frame& change, Uuid branch = Uuid::NIL) {
        state_.insert(Record{key, change});
        return Status::OK;
    }

    Status Get(Key key, Frame& result, Uuid branch = Uuid::NIL) {
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
            ok = Merge(i->first, i->second);
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
