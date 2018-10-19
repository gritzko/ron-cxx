#ifndef RON_REPLICA_HPP
#define RON_REPLICA_HPP

#include <string>
#include <unordered_map>
#include "rocksdb/db.h"
#include <rdt/lww.hpp>
#include "ron/ron.hpp"
#include "rdt/rdt.hpp"

namespace ron {


template<typename Frame>
class Replica {

    rocksdb::DB* db_;

public:

    typedef typename Frame::Batch Batch;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;

    rocksdb::ColumnFamilyHandle *data_cf_, *history_cf_, *log_cf_;

    Replica(): db_{nullptr} {}

    //  L I F E C Y C L E

    Status Create (std::string home);

    Status Open (std::string home);

    Status GC();

    Status Close ();

    ~Replica();

    //  R E C E I V E S

    Status LogRecv (const Op& op);

    Status DataRecv (const Op& op);

    Status HistoryRecv (const Op& op);

    Status TrustedDataRecv (Cursor& cursor);

    // Q U E R I E S

    Status DataQuery (Builder& response, Uuid id);

    //  @object+id :type ?
    //  @last+version ?
    //  @from+version :till+version ?
    //
    //  @object+id :type !
    //  @diffop1 'value'
    //  @diffop2 'value'
    //  ...
    Status DiffQuery (Builder& response, Uuid id);

    //  @queried-id :vv ?
    //
    //  @queried-id :vv !
    //  @time1-origin1 ,
    //  @time2-origin2 ,
    //  ...
    Status VVQuery (Builder& response, Uuid id);

    //  @queried-id :sha3 ?
    //
    //  @queried-id :sha3 'sha3hash' !
    Status SHA3Query (Builder& response, Uuid id);

    //  E N T R Y  P O I N T S

    // This is THE entry method for received frames. Here those frames
    // are scanned, checked, converted, split... all the other Recv
    // methods are called from here. The original (or converted) frame's
    // scope is this method.
    template<class FrameB>
    Status Recv (Builder& response, const FrameB& input);

    Status Recv (Builder& response, const Batch& input);

    Status RecvQuery(Builder& response, const Frame& query) {
        Cursor cur = query.cursor();
//        switch (cur.op().type().value().get30(1)) {
//            case LWW_INT:
//            //case RGA_DATA_ID[VALUE]:
//                return data_.RecvQuery(response, cur);
//            case VV_FUNC_INT:
//                return vv_.RecvQuery(response, cur);
//            case SHA3_FUNC_INT:
//                return sha3_.RecvQuery(response, cur);
//        }
    }

    Status RecvData(Builder& response, const Frame& query) {

    }

private:

    //  U T I L

    rocksdb::ColumnFamilyOptions DataCFOptions () const;
    rocksdb::ColumnFamilyOptions HistoryCFOptions () const;
    rocksdb::ColumnFamilyOptions LogCFOptions () const;

};

extern const Uuid HISTORY_CF_UUID;
extern const Uuid LOG_CF_UUID;
extern const std::string HISTORY_CF_NAME;
extern const std::string LOG_CF_NAME;


}

#endif