#ifndef RON_ROCKS_STORE_HPP
#define RON_ROCKS_STORE_HPP
#include <memory>
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
    using SharedPtr = std::shared_ptr<void>;

   private:
    /** rocksdb::DB*, but we hide rocksdb types inside the implementation
     * because that is the entire point of this class. */
    SharedPtr db_;

    /** rocksdb::ColumnFamilyHandle*, the column family to use.
     * shared_ptr<void> works, thanks to type erasure.  */
    SharedPtr cf_;

    RocksDBStore(SharedPtr db, SharedPtr cf)
        : db_{std::move(db)}, cf_{std::move(cf)} {}

   public:
    /** used by Commit and others to cache the last written event id */
    Uuid tip;

    RocksDBStore() : db_{nullptr}, cf_{nullptr}, tip{} {}

    explicit RocksDBStore(SharedPtr db) : db_{std::move(db)}, cf_{nullptr} {}

    inline SharedPtr db() const { return db_; }

    class Iterator {
        SharedPtr i_;

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

    /**
     *  Creates and opens a new store. If already open, or if
     *  ./.swarmdb already exists, creates a new column family
     *  in the database. Otherwise, creates a new database in
     *  ./.swarmdb and opens the default CF.
     */
    Status Create(Uuid id);

    Status Open(Uuid id);

    static Status Repair();

    static Status OpenAll(Branches& branches);

    Status Write(Key key, const Frame& change);

    Status Read(Key key, Frame& result);

    Status Write(const Records& batch);

    Status Close();

    ~RocksDBStore() { Close(); }
};

extern String ROCKSDB_STORE_DIR;

}  // namespace ron

#endif
