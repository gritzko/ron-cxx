#include "db/replica.hpp"

using namespace rocksdb;
using namespace std;

namespace ron {

    //  U T I L

    template<typename Frame>
    ColumnFamilyOptions Replica<Frame>::DataCFOptions () const {
        ColumnFamilyOptions cfo{};
        //cfo.merge_operator;
        return cfo;
    }
    template<typename Frame>
    ColumnFamilyOptions Replica<Frame>::HistoryCFOptions () const {
        ColumnFamilyOptions cfo{};
        //cfo.merge_operator;
        return cfo;
    }
    template<typename Frame>
    ColumnFamilyOptions Replica<Frame>::LogCFOptions () const {
        ColumnFamilyOptions cfo{};
        return cfo;
    }

    const Uuid HISTORY_CF_UUID = Uuid{805545704900788224UL, 0};
    const string HISTORY_CF_NAME = HISTORY_CF_UUID.str();
    const Uuid LOG_CF_UUID = Uuid{879235468267356160UL, 0};
    const string LOG_CF_NAME = LOG_CF_UUID.str();

    //  L I F E C Y C L E

    template<typename Frame>
    Status Replica<Frame>::Create (std::string home) {
        if (db_) return Status::BAD_STATE;

        DBOptions db_options;
        db_options.create_if_missing = true;
        db_options.error_if_exists = true;
        db_options.max_total_wal_size = UINT64_MAX;
        db_options.WAL_size_limit_MB = 1UL<<30;
        db_options.WAL_ttl_seconds = UINT64_MAX;

        ColumnFamilyOptions cf_options = DataCFOptions();
        Options options{db_options, cf_options};

        auto status = DB::Open(options, home, &db_);
        if (!status.ok()) return Status::DB_FAIL;

        status = db_->CreateColumnFamily(HistoryCFOptions(), HISTORY_CF_NAME, &history_cf_);
        if (!status.ok()) return Status::DB_FAIL;

        status = db_->CreateColumnFamily(LogCFOptions(), LOG_CF_NAME, &log_cf_);
        if (!status.ok()) return Status::DB_FAIL;

        return Status::OK;
    }

    template<typename Frame>
    Status Replica<Frame>::Open (std::string home) {
        if (db_) return Status::BAD_STATE;

        Options options;
        options.create_if_missing = false;
        options.error_if_exists = false;
        options.max_total_wal_size = UINT64_MAX;
        options.WAL_size_limit_MB = 1UL<<30;
        options.WAL_ttl_seconds = UINT64_MAX;

        typedef ColumnFamilyDescriptor CFD;
        vector<ColumnFamilyDescriptor> families;
        vector<ColumnFamilyHandle*> handles;
        families.push_back(CFD{kDefaultColumnFamilyName, DataCFOptions()});
        families.push_back(CFD{HISTORY_CF_NAME, HistoryCFOptions()});
        families.push_back(CFD{LOG_CF_NAME, LogCFOptions()});

        auto status = DB::Open(options, home, families, &handles, &db_);
        if (!status.ok()) {
            return Status::DB_FAIL;
        }

        auto i = handles.begin();
        data_cf_ = *i++;
        history_cf_ = *i++;
        log_cf_ = *i++;

        return Status::OK;
    }

    template<typename Frame>
    Status Replica<Frame>::GC() {

    }

    template<typename Frame>
    Status Replica<Frame>::Close () {
        delete data_cf_;
        data_cf_ = nullptr;
        delete history_cf_;
        history_cf_ = nullptr;
        delete log_cf_;
        log_cf_ = nullptr;
        delete db_;
        db_ = nullptr;
    }

    template<typename Frame>
    Replica<Frame>::~Replica () {
        if (!db_) Close();
    }

    //  R E C E I V E S


    template<typename Frame>
Status Replica<Frame>::Recv (Builder& response, const Batch& input) {
   /* Status ok = RefCheck(input);
    if (!ok) return ok;

    ok = AuthCheck(input);
    if (!ok) return ok;

    // *sha3 @myop :prev 'hash'
    ok = HashCheck(input);
    if (!ok) return ok;

    // *vv @myop ?

    // *ack @id :vv_elem+recur
    // @id :tag 'name'
    // @id2 :id (implicitly takes all acks)

    ok = WriteLog(input);
    if (!ok) return ok;

    ok = WriteData(input);
    ok = WirteHistory(input);

    store.Rollback();
    ok = store.Commit();
    return ok;*/
}

template<typename Frame>
template<class FrameB>
Status Replica<Frame>::Recv (Builder& response, const FrameB& input) {
    // scan, check, convert, split
    Batch to_process;
    Builder next;
    //typedef typename FrameB::Cursor CursorB;
    auto cur = input.cursor();
    do {
        if (!next.empty() && cur.op().term()!=TERM::REDUCED) {
            to_process.push_back(next.frame());
            next = Builder{};
        }
        next.AddOp(cur.op(), input.data());
    } while (cur.Next());

    return Recv(response, to_process);
}

template class Replica<TextFrame>;

}