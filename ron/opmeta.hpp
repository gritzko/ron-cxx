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

    /** no-data-yet constructor */
    OpMeta(const Uuid& op_id)
        : OpMeta{op_id, SHA2{}, Uuid::NIL, Uuid::NIL, Uuid::NIL, op_id} {}

    /** RDT-root meta (an rdt root is referenced by an object creation op). */
    explicit OpMeta(const Uuid& id, const Uuid& rdt_id)
        : OpMeta{id, SHA2{rdt_id}, id, rdt_id, Uuid::NIL, Uuid::NIL} {
        assert(rdt_id.version() == NAME);
    }

    /** Yarn-root meta, serves as the prev op for the first op in a yarn. */
    OpMeta(Word yarn, const SHA2& creds)
        : OpMeta{
              Uuid{0, yarn}, SHA2::MerklePairHash(SHA2{Uuid{0, yarn}}, creds),
              Uuid::NIL,     Uuid::NIL,
              Uuid::NIL,     Uuid{0, yarn}} {}

    /** Regular op meta.
     * @param {op} the op
     * @param {prev} meta for the previous op in the yarn
     * @param {ref} meta for the referenced op (in a chain, prev==ref) */
    template <typename Cursor>
    explicit OpMeta(const Cursor& op, const OpMeta& prev, const OpMeta& ref)
        : OpMeta{op.id(),    SHA2::OpMerkleHash(op, prev.hash, ref.hash),
                 ref.object, ref.rdt,
                 prev.id,    prev.id == ref.id ? ref.chain_id() : op.id()} {}

    template <typename Cursor>
    Status ReadAnnotation(Cursor& cur) {
        const Uuid& name = cur.id();
        if (name.version() != NAME)
            return Status::BAD_STATE.comment("not an annotation");
        if (cur.ref() != id)
            return Status::BAD_STATE.comment("annotation for a wrong op");
        if (name == SHA2_UUID && cur.has(2, STRING)) {
            SHA2 annhash =
                SHA2::ParseBase64(cur.string(2));  // TODO format check
            if (!hash.matches(annhash)) return Status::HASHBREAK;
            if (annhash.known_bits_ > hash.known_bits_) hash = annhash;
        } else if (name == OBJ_UUID && cur.has(2, UUID)) {
            Uuid annobj = cur.uuid(2);
            if (object.zero()) {
                object = annobj;
            } else if (object != annobj) {
                return Status::TREEBREAK;
            }
        } else if (name == RDT_UUID && cur.has(2, UUID)) {
            Uuid annrdt = cur.uuid(2);
            if (rdt.zero()) {
                rdt = annrdt;
            } else if (annrdt != rdt) {
                return Status::TREEBREAK;
            }
        }
        return Status::OK;
    }

    template <typename Cursor>
    Status ScanAnnotations(Cursor& cur) {
        Status ok = Status::OK;
        while (ok && cur.valid()) {
            ok = ReadAnnotation(cur);
            cur.Next();
        }
        return ok;
    }

    /** Regular op meta, reads the annotations.
     * @param {cur} the op and its annotations */
    template <typename Frame>
    explicit OpMeta(typename Frame::Cursor& cur)
        : id{}, hash{}, object{}, rdt{} {
        ReadAnnotation(cur);
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
