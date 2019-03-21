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
   public:
    using Cursor = typename Frame::Cursor;
    using Builder = typename Frame::Builder;
    using Record = std::pair<Key, Frame>;
    using Records = std::vector<Record>;

   private:
    /** rocksdb::DB*, but we hide rocksdb types inside the implementation
     * because that is the entire point of this class */
    void* db_;

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
    friend class Iterator;

    inline bool open() const { return db_ != nullptr; }

    Status Create(std::string path);

    Status Open(std::string path);

    Status Put(Key key, const Frame& state, Uuid branch = Uuid::NIL);

    Status Merge(Key key, const Frame& change, Uuid branch = Uuid::NIL);

    Status Get(Key key, Frame& result, Uuid branch = Uuid::NIL);

    Status Read(Key key, Builder& to, Uuid branch = Uuid::NIL);

    Status Write(const Records& batch, Uuid branch = Uuid::NIL);

    Status Close();
};

}  // namespace ron

#endif
