#ifndef RON_INMEM_STORE_HPP
#define RON_INMEM_STORE_HPP
#include <string>
#include <vector>
#include "../rdt/rdt.hpp"
#include "../ron/ron.hpp"
#include "key.hpp"

namespace ron {

template <class Frame>
class RocksDBStore {
    using Builder = typename Frame::Builder;
    using Record = std::pair<Key, Frame>;
    using Records = std::vector<Record>;

    void* db_;
    friend class Iterator;

   public:
    RocksDBStore() : db_{nullptr} {}

    class Iterator {
        void* i_;

       public:
        explicit Iterator(RocksDBStore& host);
        Status status();
        Key key();
        ron::Slice value();
        Status Next();
        Status SeekTo(Key key, bool prev = false);
        Status Close();
        ~Iterator() { Close(); }
    };

    inline bool open() const { return db_ != nullptr; }

    Status Create(std::string path);

    Status Open(std::string path);

    Status Put(Key key, const Frame& state, Uuid branch = Uuid::NIL);

    Status Merge(Key key, const Frame& change, Uuid branch = Uuid::NIL);

    Status Get(Key key, Frame& result, Uuid branch = Uuid::NIL);

    Status Write(const Records& batch);

    Status Close();
};

}  // namespace ron

#endif
