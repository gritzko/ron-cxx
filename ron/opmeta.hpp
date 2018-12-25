#ifndef CPP_OPMETA_HPP
#define CPP_OPMETA_HPP

#include "hash.hpp"
#include "status.hpp"

namespace ron {

/** Op meta is the information we derive from the op DAG; keeping
 * it as a part of each op may turn too expensive, so we derive it
 * on demand. */
struct OpMeta {
    /** op id */
    Uuid id;
    /** op Merkle hash */
    SHA2 hash;
    /** the object (the causal tree the op is attached to) */
    Uuid object;
    /** the type of the object */
    Uuid rdt;
    /** yarn-previous op id */
    Word prev;
    /** chain id */
    Word chain;

    static Uuid SHA2_UUID;
    static Uuid OBJ_UUID;
    static Uuid RDT_UUID;
    static Uuid PREV_UUID;
    static Uuid HEAD_UUID;

    OpMeta(const Uuid& op_id, const SHA2& op_hash, const Uuid& object_id,
           const Uuid& rdt_code, const Uuid& prev_op_id,
           const Uuid& chain_head_id)
        : id(op_id),
          hash(op_hash),
          object(object_id),
          rdt(rdt_code),
          prev(prev_op_id.value()),
          chain(chain_head_id.value()) {
        assert(object_id <= op_id);
        assert(prev_op_id <= op_id);
        assert(chain_head_id <= op_id);
    }

    OpMeta(const OpMeta& orig) = default;

    OpMeta() = default;

    /** RDT-root meta (an rdt root is referenced by an object creation op). */
    explicit OpMeta(const Uuid& rdt_id)
        : OpMeta{rdt_id, SHA2{rdt_id}, Uuid::ZERO,
                 rdt_id, Uuid::ZERO,   Uuid::ZERO} {
        assert(rdt_id.version() == NAME);
    }

    /** Yarn-root meta, serves as the prev op for the first op in a yarn. */
    OpMeta(Word yarn, const SHA2& creds)
        : OpMeta{Uuid{0, yarn}, SHA2{SHA2{Uuid{0, yarn}}, creds},
                 Uuid::ZERO,    Uuid::ZERO,
                 Uuid::ZERO,    Uuid{0, yarn}} {}

    /** Regular op meta.
     * @param {op} the op
     * @param {prev} meta for the previous op in the yarn
     * @param {ref} meta for the referenced op (in a chain, prev==ref) */
    template <typename Cursor>
    explicit OpMeta(const Cursor& op, const OpMeta& prev, const OpMeta& ref)
        : OpMeta{op.id(),    SHA2{op, prev.hash, ref.hash},
                 ref.object, ref.rdt,
                 prev.id,    prev.id == ref.id ? ref.id : op.id()} {}

    template <typename Cursor>
    Status ScanAnno(Cursor& cur) {
        //        const Uuid& anno = cur.id();
        //        const Uuid& ref = cur.ref();
        //        if (ref.version() != TIME) return Status::BAD_STATE;
        //        if (ref!=id) return Status::BAD_STATE;
        //        if (anno == SHA2_UUID && cur.has(2, STRING)) {
        //            // TODO
        //        } else if (anno == OBJ_UUID && cur.has(2, UUID)) {
        //            ShouldBe<&OpMeta::object>(cur.parse_uuid(2));
        //        } else if (anno == RDT_UUID && cur.has(2, UUID)) {
        //            ShouldBe<&OpMeta::rdt>(cur.parse_uuid(2));
        //        } else if (anno == PREV_UUID && cur.has(2, UUID)) {
        //            ShouldBe<&OpMeta::prev>(cur.parse_uuid(2));
        //        }
        return Status::OK;
    }

    /** Regular op meta, reads the annotations.
     * @param {cur} the op and its annotations */
    template <typename Frame>
    explicit OpMeta(typename Frame::Cursor& cur)
        : id{}, hash{}, object{}, rdt{} {
        ScanAnno(cur);
    }

    template <Uuid OpMeta::*uuid>
    bool ShouldBe(const Uuid& value) {
        Uuid& i = this->*uuid;
        if (i == Uuid::ZERO) {
            i = value;
            return true;
        }
        return value == i;
    }

    template <typename Builder>
    Status AppendAnnos(Builder& to) {
        to.AppendNewOp(RAW, SHA2_UUID, id, hash.base64());
        to.AppendNewOp(RAW, OBJ_UUID, id, object);
        // to.AppendNewOp(PREV_UUID, id, Uuid::Time(prev, id.origin()));
        to.AppendNewOp(RAW, RDT_UUID, id, rdt);
        return Status::OK;
    }

    inline bool head() const { return chain == id.value(); }

    inline Uuid chain_id() const { return Uuid::Time(chain, id.origin()); }

    template <typename Cursor>
    inline bool next(const Cursor& cur) const {
        return cur.id().version() == TIME && cur.id().origin() == id.origin() &&
               cur.id() > id && cur.ref() == id;
    }
};

}  // namespace ron

#endif  // CPP_OPMETA_HPP
