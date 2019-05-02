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
    using StoreIterator = typename Store::Iterator;

    using Names = std::unordered_map<Uuid, Uuid>;

    static const Uuid NOW_UUID;
    class Commit;

   private:
    /** the largest feasible timestamp seen */
    Word now_;

    /** the default (active) branch/snapshot */
    Uuid active_;

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
    std::unordered_map<Uuid, Store> stores_;

    Frame config_;

    const static MemStore EMPTY;

   public:
    Replica() = default;

    //  L I F E C Y C L E

    /** Create an empty replica (no branches, only the 0-store). */
    static Status CreateReplica();

    /** Open the replica in the current directory (all branches). */
    Status Open();

    Status ListStores(Uuids &stores) {
        stores.clear();
        for (auto &p : stores_) {
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

    inline bool open() const { return !stores_.empty(); }

    const Frame &config() const { return config_; }

    Status ReadConfigRecord(Uuid key, Atoms &values);

    Status WriteConfigRecord(Uuid key, const Atoms &values);

    //  B R A N C H E S / Y A R N S

    // TODO address stores, snapshots by Uuid, branches by yarn_id

    /**
     * Creates a branch. A branch is a transitive closure of a yarn.
     * \param yarn_id either crypto-generated or random or anything; we only
     * need general collision resistance here (mostly in the scope of a
     * replica). \param transcendent branches are not created by any particular
     * event but exist by convention, like the `test` branch.
     */
    Status CreateBranch(Word yarn_id, bool transcendent = false);
    Status CreateSnapshotOffBranch(Uuid point);
    Status ForkBranch(Word new_yarn_id, Word orig_yarn_id);

    Status SplitBranch(Uuid mark);
    Status MergeBranch(Uuid mark);

    Status DropStore(Uuid store);
    inline Status DropBranch(Word yarn_id) {
        return DropStore(yarn2branch(yarn_id));
    }

    inline bool HasStore(Uuid store) const {
        auto i = stores_.find(store);
        return i != stores_.end();
    }

    //  H I G H  L E V E L  A C C E S S O R S

    static inline Uuid yarn2branch(Word yarn_id) {
        return Uuid::Time(NEVER, yarn_id);
    }

    inline bool HasBranch(Word yarn) {
        return stores_.find(yarn2branch(yarn)) != stores_.end();
    }

    inline Uuid active_store() const { return active_; }

    inline Store &GetMetaStore() { return GetStore(Uuid::NIL); }

    inline Store &GetActiveStore() { return stores_.find(active_)->second; }

    Status SetActiveStore(Uuid store);

    inline Store &GetStore(Uuid store_id) {
        auto i = stores_.find(store_id);
        assert(i != stores_.end());
        return i->second;
    }

    inline Store &GetBranch(Word yarn_id) {
        return GetStore(Uuid::Time(NEVER, yarn_id));
    }

    inline Status GetChain(Frame &chain, Uuid chain_id);

    Status FillAllStates(Store &branch);

    /** A Commit is an ongoing transaction in a branch; in case all ops in a
     * Frame apply correctly, the Commit is saved. Otherwise, not. Commits are
     * stack-allocated.
     *  TODO ensure only 1 open commit per a branch
     *  TODO read-onlies */
    class Commit {
        Replica &host_;
        MemStore mem_;
        Store &main_;
        CommitStore join_;

        Uuid base_;
        Uuid max_;  // FIXME ensure the closing op is present
        Uuid tip_;
        String comment_;

       public:
        using Iterator = typename CommitStore::Iterator;

        Commit(Replica &host, Store &main_store)
            : host_{host},
              mem_{},
              main_{main_store},
              join_{main_store, mem_},
              base_{main_store.tip},
              tip_{base_},
              comment_{} {}

        Commit(Replica &host, Uuid store_id)
            : Commit{host, host.GetStore(store_id)} {}

        explicit Commit(Replica &host) : Commit{host, host.active_store()} {}

        friend class Replica;

        using Frame = Replica::Frame;
        using Records = Replica::Records;

        inline Uuid base() const { return base_; }

        inline Uuid tip() const { return tip_; }

        inline Uuid Now() { return tip_ = host_.Now(yarn_id()); }

        inline Word yarn_id() const { return base_.origin(); }

        inline Status status() const { return Status{tip(), comment_}; }

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

        inline Status GetObject(Frame &frame, Uuid id, Uuid rdt) {
            return GetFrame(frame, id, rdt);
        }

        Status GetFrame(Frame &object, Uuid id, Uuid rdt);

        inline Status GetObjectLog(Frame &frame, Uuid id) {
            return GetFrame(frame, id, LOG_FORM_UUID);
        }

        Status FindObject(Frame &frame, Uuid key);

        // FIXME old conv, rework
        Status GetMap(Frame &result, Uuid id, Uuid map);

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

        Status ReadNames(Names &names);

        Status ReadName(Uuid &id, Uuid name);

        Status WriteName(Uuid key, Uuid value);

        //  R E C E I V E S

        Status ReceiveQuery(Builder &response, Cursor &c);

        Status ReceiveWrites(Builder &resp, Cursor &c);

        Status Save();

        /** as of now, a no-op */
        Status Abort() {
            base_ = tip_ = Uuid::NIL;
            return Status::OK;
        }

        inline Status Read(Key key, Frame &into) {
            return join_.Read(key, into);
        }

        Status Close() { return tip_ == Uuid::NIL ? Status::OK : Abort(); }

        ~Commit() { Close(); }
    };

    Status Receive(Builder &response, Cursor &c, Word branch = ZERO);

    inline Status ReceiveFrame(Builder &response, Frame frame,
                               Word branch = ZERO) {
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
