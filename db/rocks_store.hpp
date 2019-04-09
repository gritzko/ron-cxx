#ifndef RON_ROCKS_STORE_HPP
#define RON_ROCKS_STORE_HPP
#include <string>
#include <unordered_map>
#include <vector>
#include "../rdt/rdt.hpp"
#include "../ron/ron.hpp"
#include "key.hpp"

namespace ron {

template <class FrameP>
class RocksDBStore {
   public:
    using Frame = FrameP;
    using Cursor = typename Frame::Cursor;
    using Builder = typename Frame::Builder;
    using Record = std::pair<Key, Frame>;
    using Records = std::vector<Record>;
    using Branches = std::unordered_map<Uuid, RocksDBStore<Frame>>;

   private:
    /** rocksdb::DB*, but we hide rocksdb types inside the implementation
     * because that is the entire point of this class */
    void* db_;
    std::vector<void*> cfs_;

   public:
    RocksDBStore() : db_{nullptr} {}

    class Iterator {
        void* i_;

       public:
        explicit Iterator(RocksDBStore& host);
        Key key() const;
        Cursor value();
        Status Next();
        Status SeekTo(Key key, bool prev = false);
        Status Close();
        ~Iterator() { Close(); }
    };
    friend class Iterator;

    inline bool open() const { return db_ != nullptr; }

    Status Create(const String& path);

    Status Open(const String& path);

    static Status OpenAll(Branches& branches, const String& path);

    Status Write(Key key, const Frame& change);

    Status Read(Key key, Frame& result);

    Status Write(const Records& batch);

    Status Close();

    ~RocksDBStore() { Close(); }
};

}  // namespace ron

#endif
