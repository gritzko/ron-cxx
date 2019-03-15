#include "rocks_store.hpp"
#include <rocksdb/merge_operator.h>
#include "rocksdb/db.h"

namespace ron {

//  T Y P E S

using DB = rocksdb::DB;
using Options = rocksdb::Options;
using DBOptions = rocksdb::DBOptions;
using ColumnFamilyOptions = rocksdb::ColumnFamilyOptions;
using ColumnFamilyHandle = rocksdb::ColumnFamilyHandle;
using ColumnFamilyDescriptor = rocksdb::ColumnFamilyDescriptor;

#define DB_ reinterpret_cast<rocksdb::DB*>(db_)

//  C O N V E R S I O N S

static inline rocksdb::Slice slice(ron::Slice slice) {
    return rocksdb::Slice{(const char*)slice.data(), slice.size()};
}

static inline ron::Slice slice(rocksdb::Slice slice) {
    return Slice{slice.data(), slice.size()};
}

static inline rocksdb::Slice key2slice(const uint64pair& key) {
    return rocksdb::Slice{(const char*)&key, sizeof(key)};
}

static inline Key slice2key(rocksdb::Slice key) { return Key::be(slice(key)); }

inline static Status status(const rocksdb::Status& orig) {
    return orig.ok() ? Status::OK : Status::DB_FAIL.comment(orig.ToString());
}

inline rocksdb::WriteOptions wo() { return rocksdb::WriteOptions{}; }

inline rocksdb::ReadOptions ro() { return rocksdb::ReadOptions{}; }

//  M E R G E  O P E R A T O R

template <typename Frame>
class RDTMerge : public rocksdb::MergeOperator {
    MasterRDT<Frame> reducer_;

   public:
    typedef typename Frame::Batch Batch;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef typename Frame::Cursors Cursors;

    RDTMerge() : reducer_{} {}

    Status MergeRDT(Builder& out, const Uuid& rdt, const Cursors& inputs) {
        return reducer_.Merge(out, rdt, inputs);
    }

    bool FullMergeV2(const MergeOperationInput& merge_in,
                     MergeOperationOutput* merge_out) const override {
        Builder out;
        Cursors inputs{};
        Key key{merge_in.key};
        RDT rdt = key.rdt();

        if (merge_in.existing_value) {
            inputs.push_back(Cursor{slice(*merge_in.existing_value)});
        }
        for (auto s : merge_in.operand_list) {
            inputs.push_back(Cursor{slice(s)});
        }

        Status ok = reducer_.Merge(out, rdt, inputs);

        if (!ok) return false;
        swap(out, merge_out->new_value);
        return true;
    }

    bool PartialMergeMulti(const rocksdb::Slice& dbkey,
                           const std::deque<rocksdb::Slice>& operand_list,
                           std::string* new_value,
                           rocksdb::Logger* logger) const override {
        Key key{dbkey};
        RDT rdt = key.rdt();
        Builder out;
        Cursors inputs{};
        inputs.reserve(operand_list.size() + 1);
        for (auto s : operand_list) {
            inputs.push_back(Cursor{slice(s)});
        }

        Status ok = reducer_.Merge(out, rdt, inputs);

        if (!ok) return false;
        swap(out, *new_value);
        return true;
    }

    const char* Name() const override { return "rdt"; }
};

//  S T O R E

template <typename Frame>
Status RocksDBStore<Frame>::Create(std::string home) {
    DBOptions db_options;
    db_options.create_if_missing = true;
    db_options.error_if_exists = true;
    db_options.max_total_wal_size = UINT64_MAX;
    db_options.WAL_size_limit_MB = 1UL << 30U;
    db_options.WAL_ttl_seconds = UINT64_MAX;

    ColumnFamilyOptions cf_options = ColumnFamilyOptions();
    Options options{db_options, cf_options};
    options.merge_operator =
        shared_ptr<rocksdb::MergeOperator>{new RDTMerge<Frame>()};

    rocksdb::DB* db;
    auto status = DB::Open(options, home, &db);
    if (!status.ok()) {
        return Status::DB_FAIL.comment(status.ToString());
    }
    db_ = db;

    return Status::OK;
}

template <typename Frame>
Status RocksDBStore<Frame>::Open(std::string home) {
    if (db_) return Status::BAD_STATE.comment("db open already");

    Options options;
    options.create_if_missing = false;
    options.error_if_exists = false;
    options.max_total_wal_size = UINT64_MAX;
    options.WAL_size_limit_MB = 1UL << 30U;
    options.WAL_ttl_seconds = UINT64_MAX;
    options.merge_operator =
        shared_ptr<rocksdb::MergeOperator>{new RDTMerge<Frame>()};

    typedef ColumnFamilyDescriptor CFD;
    vector<ColumnFamilyDescriptor> families;
    vector<ColumnFamilyHandle*> handles;
    ColumnFamilyOptions cfo{};
    families.push_back(CFD{rocksdb::kDefaultColumnFamilyName, cfo});
    // TODO branches

    rocksdb::DB* db;
    auto ok = DB::Open(options, home, families, &handles, &db);
    if (ok.ok()) {
        db_ = db;
    }
    return status(ok);
}

template <typename Frame>
Status RocksDBStore<Frame>::Close() {
    if (db_) {
        delete DB_;
        db_ = nullptr;
    }
    return Status::OK;
}

template <typename Frame>
Status RocksDBStore<Frame>::Put(Key key, const Frame& state, Uuid branch) {
    auto ok = DB_->Put(wo(), key, state.data());
    return status(ok);
}

template <typename Frame>
Status RocksDBStore<Frame>::Merge(Key key, const Frame& change, Uuid branch) {
    auto be = key.be();
    Slice data{change.data()};
    auto ok = DB_->Merge(wo(), key2slice(be), slice(data));
    return status(ok);
}

template <typename Frame>
Status RocksDBStore<Frame>::Get(Key key, Frame& result, Uuid branch) {
    uint64pair k = key.be();
    String ret;
    auto ok = DB_->Get(ro(), key2slice(k), &ret);
    result = Frame{ret};
    return status(ok);
}

template <typename Frame>
Status RocksDBStore<Frame>::Write(const Records& batch) {
    rocksdb::WriteBatch b;
    for (auto i = batch.begin(); i != batch.end(); ++i) {
        uint64pair k = i->first.be();
        const String& data = i->second.data();
        rocksdb::Slice slice{data};
        b.Merge(key2slice(k), slice);
    }
    auto ok = DB_->Write(wo(), &b);
    return status(ok);
}

//  I T E R A T O R

#define IT_ reinterpret_cast<rocksdb::Iterator*>(i_)

template <typename Frame>
RocksDBStore<Frame>::Iterator::Iterator(RocksDBStore& host) {
    void* db_ = host.db_;
    i_ = DB_->NewIterator(ro());
}

template <typename Frame>
Status RocksDBStore<Frame>::Iterator::status() {
    rocksdb::Status ok = IT_->status();
    if (ok.ok() && !IT_->Valid()) return Status::ENDOFINPUT;
    return ron::status(ok);
}

template <typename Frame>
Key RocksDBStore<Frame>::Iterator::key() {
    assert(IT_);
    return slice2key(IT_->key());
}

template <typename Frame>
ron::Slice RocksDBStore<Frame>::Iterator::value() {
    assert(IT_);
    return slice(IT_->value());
}

template <typename Frame>
Status RocksDBStore<Frame>::Iterator::Next() {
    assert(IT_);
    IT_->Next();
    Status ok = status();
    if (!ok) {
        delete IT_;
        i_ = nullptr;
    }
    return ok;
}

template <typename Frame>
Status RocksDBStore<Frame>::Iterator::SeekTo(Key key, bool prev) {
    auto be = key.be();
    auto k = key2slice(be);
    prev ? IT_->SeekForPrev(k) : IT_->Seek(k);
    return status();
}

template <typename Frame>
Status RocksDBStore<Frame>::Iterator::Close() {
    if (i_) {
        delete IT_;
        i_ = nullptr;
    }
    return Status::OK;
}

template class RocksDBStore<TextFrame>;

}  // namespace ron
