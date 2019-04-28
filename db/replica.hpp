#ifndef RON_REPLICA_HPP
#define RON_REPLICA_HPP

#include <string>
#include <unordered_map>
#include "../rdt/lww.hpp"
#include "../rdt/rdt.hpp"
#include "../ron/hash.hpp"
#include "../ron/opmeta.hpp"
#include "../ron/ron.hpp"
#include "const.hpp"
#include "joined_store.hpp"
#include "key.hpp"
#include "map.hpp"
#include "map/csv.hpp"
#include "map/txt.hpp"
#include "mem_store.hpp"
#include "rocks_store.hpp"

namespace ron {

enum replica_modes_t : uint64_t {
    KEEP_STATES = 1UL << 0,
    KEEP_OBJECT_LOGS = 1UL << 1,
    KEEP_YARNS = 1UL << 2,
    KEEP_HASHES = 1UL << 3,
    CONSISTENT_MODE = KEEP_STATES | KEEP_OBJECT_LOGS | KEEP_YARNS | KEEP_HASHES,
};

template <typename Store>
class Replica {
   public:
    using Frame = typename Store::Frame;
    using Record = std::pair<Key, Frame>;
    using Records = std::vector<Record>;
    using Batch = typename Frame::Batch;
    using Builder = typename Frame::Builder;
    using Cursor = typename Frame::Cursor;

    using tipmap_t = std::unordered_map<Word, OpMeta>;

    using MemStore = InMemoryStore<Frame>;
    using CommitStore = JoinedStore<Store, MemStore>;

    using Names = std::unordered_map<Uuid, Uuid>;

    static const Uuid NOW_UUID;
    class Commit;

   private:
    /** the largest feasible timestamp seen */
    Word now_;

    /** the default (home) branch */
    Uuid home_;

    replica_modes_t mode_{CONSISTENT_MODE};

    // chain cache - skip db reads for ongoing op chains
    tipmap_t tips_;

    TxtMapper<Commit> txt_;

    /** Stores for all the db's existing branches and snapshots.
     *  Branch stores are names ~+BranchId while snapshot stores
     *  are named VerS10N+BranchId.
     *  The 0-store contains metadata: branch names, peer URLs,
     *  private keys etc; it is not replicated unless in cluster
     *  scenarios.
     *   */
    std::unordered_map<Uuid, Store> branches_;

    Frame config_;

    const static MemStore EMPTY;

   public:
    Replica() = default;

    //  L I F E C Y C L E

    /** Create an empty replica (no branches, only the 0-store). */
    static Status CreateReplica();

    /** Open the replica in the current directory (all branches). */
    Status Open();

    Status CreateBranch(Uuid branch_id);

    Status ListStores(Uuids &stores) {
        stores.clear();
        for (auto &p : branches_) {
            stores.push_back(p.first);
        }
        return Status::OK;
    }

    Status GC();

    Status Close();

    ~Replica();

    Uuid Now(Word origin = ZERO);
    Status See(Uuid timestamp);

    inline mode_t mode() const { return mode_; }

    inline bool open() const { return !branches_.empty(); }

    const Frame &config() const { return config_; }

    Uuid current_branch() const { return Uuid::NIL; }

    Status ReadConfigRecord(Uuid key, Atoms &values);

    Status WriteConfigRecord(Uuid key, const Atoms &values);

    //  B R A N C H E S / Y A R N S

    Status SplitBranch(Uuid mark);
    Status MergeBranch(Uuid mark);
    Status DropBranch(Uuid branch);

    //  H I G H  L E V E L  A C C E S S O R S

    Status GetFrame(Frame &object, Uuid id, Uuid rdt, Uuid branch = Uuid::NIL);

    inline bool HasBranch(Uuid branch) {
        return branches_.find(branch) != branches_.end();
    }

    inline Store &GetCurrentBranch() {
        return branches_.find(Uuid::NIL)->second;
    }

    inline Store &GetBranch(Uuid branch) {
        return branches_.find(branch)->second;
    }

    Status ReadNames(Names &names, Uuid branch = Uuid::NIL);
    Status ReadName(Uuid &id, Uuid name, Uuid branch = Uuid::NIL);

    Status WriteName(Uuid key, Uuid value, Uuid branch = Uuid::NIL);

    inline Store &GetMeta() { return GetBranch(Uuid::NIL); }

    inline Status GetChain(Frame &chain, Uuid chain_id);

    Status FillAllStates(Store &branch);

    inline Status GetObject(Frame &frame, Uuid id, Uuid rdt,
                            Uuid branch = Uuid::NIL) {
        return GetFrame(frame, id, rdt, branch);
    }

    inline Status GetObjectLog(Frame &frame, Uuid id, Uuid branch = Uuid::NIL) {
        return GetFrame(frame, id, LOG_FORM_UUID);
    }

    Status FindObject(Frame &frame, Uuid key, Uuid branch = Uuid::NIL);

    // FIXME old conv, rework
    Status GetMap(Frame &result, Uuid id, Uuid map, Uuid branch = Uuid::NIL);

    class Commit {
        Replica &host_;
        Word yarn_id_;
        MemStore mem_;
        Store &main_;
        CommitStore join_;

        using Iterator = typename CommitStore::Iterator;

        Commit(Replica &host, Store &main_store, Word id)
            : host_{host},
              yarn_id_{id},
              mem_{},
              main_{main_store},
              join_{main_store, mem_} {}
        friend class Replica;

       public:
        using Frame = Replica::Frame;
        using Records = Replica::Records;

        inline Word yarn_id() const { return yarn_id_; }

        /** The last op in the yarn.
         *  Must be something; the first op in a yarn is a meta-record
         * @time+yarnid :yarn pubkey 'ABCDEF' name 'Victor' ...
         * @param{yarn} yarn id
         * @return a reference to the OpMeta entry (id==0 if none)
         * */
        Status FindYarnTipMeta(OpMeta &meta, Word yarn);

        //  O T H E R  A C C E S S O R S

        /** If we don't know the exact chain id, we have to scan the table to
         *  find the chain. Then, we scan the chain to find the op.
         *  @param{op_id} the op id or ~+yarn_id for the yarn tip
         */
        Status FindOpMeta(OpMeta &meta, Uuid op_id);

        /** Fetches the op metadata for the chain head op.
         * @param meta - the op meta object with op id set to the chain id */
        Status FindChainHeadMeta(OpMeta &meta, Uuid op_id);

        Status FindObjectLog(Frame &frame, Uuid id);

        Status CheckEventSanity(const Cursor &op);

        //  T R A N S A C T I O N A L  R E A D S  W R I T E S

        /** @object+id :xxx ? */
        Status QueryFrame(Builder &response, Cursor &query);

        /** @object+id :lww ? */
        Status QueryObject(Builder &response, Cursor &query);

        /** @head+id :span ? */
        Status QueryOpChain(Builder &response, Cursor &query);

        /** @object+id :log ?   @version+id :log ?   @version+id :log till
         * versn+id
         * ? */
        Status QueryObjectLog(Builder &response, Cursor &query);

        /** @version+id :tail ? */
        Status QueryObjectLogTail(Builder &response, Cursor &query);

        /** @version+id :patch ? */
        Status QueryObjectPatch(Builder &response, Cursor &query);

        /** @version+id :version+id ?
        Status ObjectLogSegmentQuery(Builder& response, Cursor& query, Uuid
        branch=Uuid::Uuid::NIL); */

        /** @op+id :meta ?  @op+id :sha3 ?  @op+id :prev ?  @op+id :obj ?  */
        Status QueryOpMeta(Builder &response, Cursor &query);

        /** @time+yarn :vv ?  @~+yarn :vv ? */
        Status QueryYarnVV(Builder &response, Cursor &query);

        /** @time+yarn :yarn ? */
        Status QueryYarn(Builder &response, Cursor &query);

        //  M A P P E R  Q U E R I E S

        /**
            @object+id :map ?
            @version+id :map ?
            @object-id :dtxt,
                @version-till :version-from ?
        */
        Status QueryMapper(Builder &response, Cursor &query);

        //  S U B S C R I P T I O N S

        /**
            @object-id :dtxt ?
            @version-id :dtxt ?
        */
        // Status MapperSub (Builder& response, Cursor& query, Uuid
        // branch=Uuid::Uuid::NIL);

        /**
            @object+id ?
            @object+id :lww ?
            @version+id ?
            @version+id :lww ?
        */
        Status ObjectSub(Builder &response, Cursor &query);

        //  W R I T E S

        Status SaveChainlet(Builder &to, OpMeta &meta, Cursor &from);

        // feed a causally ordered log - checks causality, updates the chain
        // cache
        Status SaveChain(Builder &, Cursor &chain);

        Status WriteNewEvents(Builder &, Cursor &chain);

        /**
            @version-id :txt 'text' !

            @object-id :dtxt,
                @version-to :version-from 2 -1 2 'x' !
        */
        Status WriteThroughMap(Cursor &write) {
            return Status::NOT_IMPLEMENTED.comment("MapWrite");
        }

        //  R E C E I V E S

        Status ReceiveQuery(Builder &response, Cursor &c);

        Status ReceiveWrites(Builder &resp, Cursor &c);

        Status Save();

        inline Status Read(Key key, Frame &into) {
            return join_.Read(key, into);
        }
    };

    Status Receive(Builder &response, Cursor &c, Uuid branch = Uuid::NIL);

    inline Status ReceiveFrame(Builder &response, Frame frame,
                               Uuid branch = Uuid::NIL) {
        Cursor c{frame};
        return Receive(response, c, branch);
    }

    // the entry point: recoder, normalizer, access control
    // converts any-coded incoming frame into internal-coded chains, queries,
    // hash checks
    template <class FrameB>
    Status AnyFrameRecv(Uuid conn_id, const FrameB &frame) {
        return Status::NOT_IMPLEMENTED.comment("AnyFrameRecv");
    }
};

}  // namespace ron

#endif
