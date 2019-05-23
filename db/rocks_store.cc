#include "rocks_store.hpp"
#include <rocksdb/db.h>
#include <rocksdb/merge_operator.h>

namespace ron {

//  T Y P E S

using DB = rocksdb::DB;
using Options = rocksdb::Options;
using DBOptions = rocksdb::DBOptions;
using ColumnFamilyOptions = rocksdb::ColumnFamilyOptions;
using ColumnFamilyHandle = rocksdb::ColumnFamilyHandle;
using ColumnFamilyDescriptor = rocksdb::ColumnFamilyDescriptor;

String ROCKSDB_STORE_DIR{".swarmdb"};

//  C O N V E R S I O N S

static inline rocksdb::Slice slice(ron::Slice slice) {
    return rocksdb::Slice{(const char*)slice.begin(), slice.size()};
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
    using Batch = typename Frame::Batch;
    using Builder = typename Frame::Builder;
    using Cursor = typename Frame::Cursor;
    using Cursors = typename Frame::Cursors;

    RDTMerge() : reducer_{} {}

    Status MergeRDT(Builder& out, const Uuid& rdt, const Cursors& inputs) {
        return reducer_.Merge(out, rdt, inputs);
    }

    bool FullMergeV2(const MergeOperationInput& merge_in,
                     MergeOperationOutput* merge_out) const override {
        Builder out;
        Cursors inputs{};
        Key key = slice2key(merge_in.key);

        if (merge_in.existing_value) {
            inputs.push_back(Cursor{slice(*merge_in.existing_value)});
        }
        for (auto s : merge_in.operand_list) {
            inputs.push_back(Cursor{slice(s)});
        }

        Status ok = reducer_.Merge(out, key.form(), inputs);

        if (!ok) return false;
        swap(out, merge_out->new_value);
        return true;
    }

    bool PartialMergeMulti(const rocksdb::Slice& dbkey,
                           const std::deque<rocksdb::Slice>& operand_list,
                           std::string* new_value,
                           rocksdb::Logger* logger) const override {
        Key key = slice2key(dbkey);
        Builder out;
        Cursors inputs{};
        inputs.reserve(operand_list.size() + 1);
        for (auto s : operand_list) {
            inputs.push_back(Cursor{slice(s)});
        }

        Status ok = reducer_.Merge(out, key.form(), inputs);

        if (!ok) return false;
        swap(out, *new_value);
        return true;
    }

    const char* Name() const override { return "rdt"; }
};

template <class Frame>
void init_options(rocksdb::Options& options) {
    options.create_if_missing = false;
    options.error_if_exists = false;
    options.max_total_wal_size = UINT64_MAX;
    options.WAL_size_limit_MB = 1UL << 30U;
    options.WAL_ttl_seconds = UINT64_MAX;
    options.merge_operator = make_shared<RDTMerge<Frame>>();
}

#define IFROK(x)                   \
    {                              \
        rocksdb::Status rok = (x); \
        if (!rok.ok()) {           \
            return status(rok);    \
        }                          \
    }

//  S T O R E

template <typename Frame>
Status RocksDBStore<Frame>::Create(Uuid id) {
    Options options{};
    init_options<Frame>(options);
    options.create_if_missing = true;
    options.error_if_exists = false;

    if (!db_) {
        rocksdb::DB* db;
        IFROK(DB::Open(options, ROCKSDB_STORE_DIR, &db));
        db_ = SharedPtr{db};
    }

    ColumnFamilyHandle* cfh;
    auto db = static_pointer_cast<rocksdb::DB>(db_);
    if (id != Uuid::NIL) {
        IFROK(db->CreateColumnFamily(options, id.str(), &cfh));
        cf_.reset(cfh);
    } else {
        cf_ = SharedPtr{db->DefaultColumnFamily(), [](void*) {}};
    }

    tip = Uuid::NIL;
    Frame now = OneOp<Frame>(tip, ZERO_FORM_UUID);
    IFOK(Write(Key::ZERO, now));

    return Status::OK;
}

template <typename Frame>
Status RocksDBStore<Frame>::Open(Uuid id) {
    if (db_) return Status::BAD_STATE.comment("db open already");

    Options options;
    init_options<Frame>(options);

    rocksdb::DB* db;
    IFROK(DB::Open(options, ROCKSDB_STORE_DIR, &db));
    db_.reset(db);

    if (!id.zero()) {
        return Status::NOT_IMPLEMENTED.comment("use OpenAll for now");
    }

    return Status::OK;
}

template <typename Frame>
Status RocksDBStore<Frame>::Drop() {
    auto db = static_pointer_cast<rocksdb::DB>(db_);
    auto cf = static_pointer_cast<rocksdb::ColumnFamilyHandle>(cf_);
    IFROK(db->DropColumnFamily(cf.get()));
    return Close();
}

template <typename Frame>
Status RocksDBStore<Frame>::Close() {
    if (!db_.use_count()) return Status::BAD_STATE.comment("already closed");
    cf_.reset();
    db_.reset();
    return Status::OK;
}

template <typename Frame>
Status RocksDBStore<Frame>::Write(Key key, const Frame& change) {
    if (!db_) return Status::BAD_STATE.comment("closed");
    if (key == Key::END && !change.empty()) {
        return Status::BADARGS.comment("can't write to Key::END");
    }
    auto be = key.be();
    auto db = static_pointer_cast<rocksdb::DB>(db_);
    auto cf = static_pointer_cast<rocksdb::ColumnFamilyHandle>(cf_);
    Slice data{change.data()};
    LOG('w', key, change.data());
    IFROK(db->Merge(wo(), cf.get(), key2slice(be), slice(data)));
    return Status::OK;
}

template <typename Frame>
Status RocksDBStore<Frame>::Read(Key key, Frame& result) {
    result.Clear();
    if (!db_) return Status::BAD_STATE.comment("closed");
    if (key == Key::END) {
        return Status::OK;
    }
    uint64pair k = key.be();
    String ret;
    auto db = static_pointer_cast<rocksdb::DB>(db_);
    auto cf = static_pointer_cast<rocksdb::ColumnFamilyHandle>(cf_);
    auto ok = db->Get(ro(), cf.get(), key2slice(k), &ret);
    if (ok.IsNotFound()) {
        LOG('r', key, "");
        result.Clear();
        return Status::OK;
    }
    if (!ok.ok()) {
        LOG('r', key, status(ok).str());
        return status(ok);
    }
    LOG('r', key, ret);
    result.swap(ret);
    return Status::OK;
}

template <typename Frame>
Status RocksDBStore<Frame>::Write(const Records& batch) {
    rocksdb::WriteBatch b;
    auto cf = static_pointer_cast<rocksdb::ColumnFamilyHandle>(cf_);
    for (auto i = batch.begin(); i != batch.end(); ++i) {
        uint64pair k = i->first.be();
        const String& data = i->second.data();
        rocksdb::Slice slice{data};
        b.Merge(cf.get(), key2slice(k), slice);
        LOG('m', i->first, data);
    }
    auto db = static_pointer_cast<rocksdb::DB>(db_);
    IFROK(db->Write(wo(), &b));
    return Status::OK;
}

//  I T E R A T O R

template <typename Frame>
RocksDBStore<Frame>::Iterator::Iterator(RocksDBStore& host) {
    auto db = static_pointer_cast<rocksdb::DB>(host.db_);
    auto cf = static_pointer_cast<rocksdb::ColumnFamilyHandle>(host.cf_);
    auto i = db->NewIterator(ro(), cf.get());
    i_ = shared_ptr<rocksdb::Iterator>(i);
}

/*template <typename Frame>
Status RocksDBStore<Frame>::Iterator::status() {
    rocksdb::Status ok = IT_->status();
    if (ok.ok() && !IT_->Valid()) return Status::ENDOFINPUT;
    return ron::status(ok);
}*/

template <typename Frame>
Key RocksDBStore<Frame>::Iterator::key() const {
    auto i = static_pointer_cast<const rocksdb::Iterator>(i_);
    if (!i || !i->Valid()) {
        return Key::END;
    }
    return slice2key(i->key());
}

template <typename Frame>
typename Frame::Cursor RocksDBStore<Frame>::Iterator::value() {
    auto i = static_pointer_cast<const rocksdb::Iterator>(i_);
    if (!i || !i->Valid()) {
        return typename Frame::Cursor{""};
    }
    return Cursor{slice(i->value())};
}

template <typename Frame>
Status RocksDBStore<Frame>::Iterator::Next() {
    if (!i_) {
        return Status::BAD_STATE.comment("closed");
    }
    auto i = static_pointer_cast<rocksdb::Iterator>(i_);
    i->Next();
    IFROK(i->status());
    if (!i->Valid()) {
        return Status::ENDOFINPUT;
    }
    return Status::OK;
}

template <typename Frame>
Status RocksDBStore<Frame>::Iterator::SeekTo(Key key, bool prev) {
    if (!i_) {
        return Status::BAD_STATE.comment("closed");
    }
    auto i = static_pointer_cast<rocksdb::Iterator>(i_);
    auto be = key.be();
    auto k = key2slice(be);
    prev ? i->SeekForPrev(k) : i->Seek(k);
    IFROK(i->status());
    return Status::OK;
}

template <typename Frame>
Status RocksDBStore<Frame>::Iterator::Close() {
    i_.reset();
    return Status::OK;
}

template <class Frame>
Status RocksDBStore<Frame>::OpenAll(Branches& branches) {
    Options options;
    using CFD = ColumnFamilyDescriptor;
    vector<ColumnFamilyDescriptor> families;
    vector<ColumnFamilyHandle*> handles;
    ColumnFamilyOptions cfo{};
    cfo.merge_operator = make_shared<RDTMerge<Frame>>();

    init_options<Frame>(options);

    vector<std::string> cfnames;
    IFROK(
        rocksdb::DB::ListColumnFamilies(options, ROCKSDB_STORE_DIR, &cfnames));
    for (auto& name : cfnames) {
        families.push_back(CFD{name, cfo});
    }

    rocksdb::DB* db;
    IFROK(DB::Open(options, ROCKSDB_STORE_DIR, families, &handles, &db));

    branches.clear();
    branches.reserve(families.size());

    SharedPtr dbsh{db};
    for (auto* cf : handles) {
        SharedPtr cfsh{cf};
        auto name = cf->GetName();
        Uuid id =
            name == rocksdb::kDefaultColumnFamilyName ? Uuid::NIL : Uuid{name};
        RocksDBStore<Frame> next{dbsh, cfsh};
        branches.emplace(id, next);
    }

    return Status::OK;
}

template <class Frame>
Status RocksDBStore<Frame>::Repair() {
    Options options;
    init_options<Frame>(options);

    using CFD = ColumnFamilyDescriptor;
    vector<ColumnFamilyDescriptor> families;
    ColumnFamilyOptions cfo{};
    cfo.merge_operator = make_shared<RDTMerge<Frame>>();
    vector<std::string> cfnames;
    IFROK(
        rocksdb::DB::ListColumnFamilies(options, ROCKSDB_STORE_DIR, &cfnames));
    for (auto& name : cfnames) {
        families.push_back(CFD{name, cfo});
    }

    IFROK(rocksdb::RepairDB(ROCKSDB_STORE_DIR, options, families));

    return Status::OK;
}

template class RocksDBStore<TextFrame>;

}  // namespace ron
