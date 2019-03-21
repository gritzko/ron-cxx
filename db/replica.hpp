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
#include "key.hpp"
#include "map.hpp"
#include "rocks_store.hpp"

namespace ron {

enum replica_modes_t : uint64_t {
    KEEP_STATES = 1UL << 0,
    KEEP_OBJECT_LOGS = 1UL << 1,
    KEEP_YARNS = 1UL << 2,
    KEEP_HASHES = 1UL << 3,
    CONSISTENT_MODE = KEEP_STATES | KEEP_OBJECT_LOGS | KEEP_YARNS | KEEP_HASHES,
};

template <typename Frame>
class Replica {
   public:
    using Record = std::pair<Key, Frame>;
    using Records = std::vector<Record>;
    using Store = RocksDBStore<Frame>;
    using StoreIterator = typename Store::Iterator;
    using Batch = typename Frame::Batch;
    using Builder = typename Frame::Builder;
    using Cursor = typename Frame::Cursor;
    using tipmap_t = std::unordered_map<Word, OpMeta>;

    static const Uuid NOW_UUID;

   private:
    Store db_;
    Uuid now_;
    replica_modes_t mode_{CONSISTENT_MODE};

    // chain cache - skip db reads for ongoing op chains
    tipmap_t tips_;

    TxtMapper<Frame> txt_;

   public:
    Replica() : db_{}, now_{}, tips_{}, txt_{db_} {}

    //  L I F E C Y C L E

    Status Create(std::string home, Word origin = 0);

    Status Open(std::string home);

    Status GC();

    Status Close();

    ~Replica();

    Uuid Now();
    Status See(Uuid timestamp, Records& save);

    Store& db() { return db_; }
    inline bool open() const { return db_.open(); }

    //  B R A N C H E S / Y A R N S

    Status CreateBranch(Word branch);

    Status SplitBranch(Uuid mark);
    Status MergeBranch(Uuid mark);
    Status DropBranch(Word branch);

    //  L O W  L E V E L

    Status GetFrame(Frame& object, Uuid id, Uuid rdt, Uuid branch = Uuid::NIL);

    //  O B J E C T  L O G S

    Status FindYarn(StoreIterator& i, Uuid start, Word branch = ZERO);

    inline Status GetChain(Frame& chain, Uuid chain_id);

    //  O B J E C T  S T A T E S

    Status FillAllStates(Uuid branch);

    Status GetObject(Frame& frame, Uuid id, Uuid rdt, Uuid branch = Uuid::NIL);

    inline Status GetObjectLog(Frame& frame, Uuid id, Uuid branch = Uuid::NIL) {
        return GetFrame(frame, id, LOG_FORM_UUID, branch);
    }

    Status FindObject(Frame& frame, Uuid key, Uuid branch = Uuid::NIL);

    //  O P  M E T A

    /** If we don't know the exact chain id, we have to scan the table to
     *  find the chain. Then, we scan the chain to find the op. */
    Status FindOpMeta(OpMeta& meta, Uuid op_id, Uuid branch = Uuid::NIL);

    /** Fetches the op metadata for the chain head op.
     * @param meta - the op meta object with op id set to the chain id */
    Status FindHeadMeta(OpMeta& meta, Uuid op_id, Uuid branch = Uuid::NIL);

    /** The last op in the yarn.
     * Must be something; the first op in a yarn is a meta-record
     * @time+yarnid :yarn pubkey 'ABCDEF' name 'Victor' ...
     * @param{yarn} yarn id
     * @return a reference to the OpMeta entry (id==0 if none)
     * */
    OpMeta& GetYarnTipMeta(Word yarn, Uuid branch = Uuid::NIL);

    //  E V E N T  Q U E R I E S

    /** @object+id :xxx ? */
    Status FrameQuery(Builder& response, Cursor& query,
                      Uuid branch = Uuid::NIL);

    /** @object+id :lww ? */
    Status ObjectQuery(Builder& response, Cursor& query,
                       Uuid branch = Uuid::NIL);

    /** @head+id :span ? */
    Status OpChainQuery(Builder& response, Cursor& query,
                        Uuid branch = Uuid::NIL);

    /** @object+id :log ?   @version+id :log ?   @version+id :log till versn+id
     * ? */
    Status ObjectLogQuery(Builder& response, Cursor& query,
                          Uuid branch = Uuid::NIL);

    /** @version+id :tail ? */
    Status ObjectLogTailQuery(Builder& response, Cursor& query,
                              Uuid branch = Uuid::NIL);

    /** @version+id :patch ? */
    Status ObjectPatchQuery(Builder& response, Cursor& query,
                            Uuid branch = Uuid::NIL);

    /** @version+id :version+id ?
    Status ObjectLogSegmentQuery(Builder& response, Cursor& query, Uuid
    branch=Uuid::NIL); */

    /** @op+id :meta ?  @op+id :sha3 ?  @op+id :prev ?  @op+id :obj ?  */
    Status OpMetaQuery(Builder& response, Cursor& query,
                       Uuid branch = Uuid::NIL);

    /** @time+yarn :vv ?  @~+yarn :vv ? */
    Status YarnVVQuery(Builder& response, Cursor& query,
                       Uuid branch = Uuid::NIL);

    /** @time+yarn :yarn ? */
    Status YarnQuery(Builder& response, Cursor& query, Uuid branch = Uuid::NIL);

    //  M A P P E R  Q U E R I E S

    /**
        @object+id :map ?
        @version+id :map ?
        @object-id :dtxt,
            @version-till :version-from ?
    */
    Status MapperQuery(Builder& response, Cursor& query,
                       Uuid branch = Uuid::NIL);

    Status GetMap(Frame& result, Uuid id, Uuid map, Uuid branch = Uuid::NIL);

    //  S U B S C R I P T I O N S

    /**
        @object-id :dtxt ?
        @version-id :dtxt ?
    */
    // Status MapperSub (Builder& response, Cursor& query, Uuid
    // branch=Uuid::NIL);

    /**
        @object+id ?
        @object+id :lww ?
        @version+id ?
        @version+id :lww ?
    */
    Status ObjectSub(Builder& response, Cursor& query, Uuid branch = Uuid::NIL);

    //  W R I T E S

    Status ReadChainlet(Builder& to, OpMeta& meta, Cursor& from);

    // feed a causally ordered log - checks causality, updates the chain cache
    Status WriteNewChain(Records& save, Cursor& chain, Uuid branch = Uuid::NIL);

    Status WriteNewObject(Records& save, Cursor& chain,
                          Uuid branch = Uuid::NIL);

    Status WriteNewYarn(Records& save, Cursor& yroot, Uuid branch = Uuid::NIL);

    /**
        @version-id :txt 'text' !

        @object-id :dtxt,
            @version-to :version-from 2 -1 2 'x' !
    */
    Status MapWrite(Records& save, Cursor& write, Uuid branch = Uuid::NIL) {
        return Status::NOT_IMPLEMENTED;
    }

    //  R E C E I V E S

    Status Recv(Builder& response, Cursor& c, Uuid branch = Uuid::NIL);
    inline Status FrameRecv(Builder& response, Frame frame,
                            Uuid branch = Uuid::NIL) {
        Cursor c{frame};
        return Recv(response, c, branch);
    }
    Status DispatchRecv(Builder& resp, Records& save, Cursor& c, Uuid branch);

    // the entry point: recoder, normalizer, access control
    // converts any-coded incoming frame into internal-coded chains, queries,
    // hash checks
    template <class FrameB>
    Status AnyFrameRecv(Uuid conn_id, const FrameB& frame) {
        return Status::NOT_IMPLEMENTED;
    }

    void cerr_batch(Records& save);
};

}  // namespace ron

#endif
