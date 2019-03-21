#ifndef CPP_OPMETA_HPP
#define CPP_OPMETA_HPP

#include "form.hpp"
#include "hash.hpp"
#include "slice.hpp"
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

    OpMeta() = default;

    OpMeta(const OpMeta& orig) = default;

    /** Yarn root.
      @param{op} the root op
      @param{form} the name UUID for the form (RDT id or "yarn")
      @param{prev} the hash of the yarn-preceding op or the credential hash for
      the yarn root */
    template <class Cursor>
    OpMeta(const Cursor& op)
        : id{op.id()},
          rdt{YARN_FORM_UUID},
          object{id},
          prev{},
          chain{id.value()} {
        assert(op.ref() == YARN_FORM_UUID);
        hash = SHA2::OpMerkleHash(op, SHA2{}, SHA2{rdt});
    }

    /** Causal tree root (object creation).
      @param{op} the root op
      @param{form} the name UUID for the form (normally, RDT id)
      @param{prev} the meta of the yarn-preceding op */
    template <class Cursor>
    OpMeta(const Cursor& op, const OpMeta& prev)
        : id{op.id()},
          rdt{op.ref()},
          object{id},
          prev{prev.id.value()},
          chain{id.value()} {
        assert(id.version() == TIME);
        assert(rdt.version() == NAME);
        hash = SHA2::OpMerkleHash(op, prev.hash, SHA2{rdt});
    }

    /** Non-root op (has a previous op on the same yarn, references some past
     * op).
     * @param {op} the op
     * @param {ref} meta for the referenced op (in a chain, prev==ref)
     * @param {prev} meta for the previous op in the yarn
     */
    template <class Cursor>
    OpMeta(const Cursor& op, const OpMeta& ref, const OpMeta& prev)
        : id{op.id()},
          rdt{ref.rdt},
          object{ref.object},
          prev{prev.id.value()},
          chain{prev.id == ref.id ? ref.chain_id() : op.id()} {
        hash = SHA2::OpMerkleHash(op, prev.hash, ref.hash);
    }

    template <typename Cursor>
    inline bool is_next(const Cursor& cur) const {
        return cur.id().version() == TIME && cur.id().origin() == id.origin() &&
               cur.id() > id && cur.ref() == id;
    }

    template <typename Cursor>
    inline bool is_check(const Cursor& cur) const {
        return cur.ref() == id && cur.id().version() == NAME;
    }

    template <class Cursor>
    Status Next(const Cursor& op) {
        return Status::OK;
    }

    template <class Cursor>
    Status Check(const Cursor& op) {
        FORM form = uuid2form(op.ref());
        switch (form) {
            case META_META_FORM:
                break;
            case PREV_META_FORM:
                break;
            case OBJ_META_FORM:
                break;
            case SHA3_META_FORM:
                // known bits
                break;
            default:
                return Status::BADARGS.comment("unknown meta record " +
                                               op.ref().str());
        }
        return Status::OK;
    }

    template <class Builder>
    void Save(Builder& save) {
        save.AppendNewOp(RAW, id, META_FORM_UUID, object, rdt, hash.base64());
    }

    template <class Cursor>
    Status Load(const Cursor& load) {
        // see above, check format
        id = load.id();
        chain = id.value();
        prev = NEVER;
        if (load.ref() != META_FORM_UUID) {
            return Status::BADARGS.comment("not a meta record");
        }
        if (!load.has(2, UUID)) {
            return Status::BADSYNTAX.comment("no object id");
        }
        object = load.uuid(2);
        if (!load.has(3, UUID)) {
            return Status::BADSYNTAX.comment("no rdt id");
        }
        rdt = load.uuid(3);
        hash;
        return Status::OK;
    }

    /*
        Status ReadAnnotation(Cursor& cur) {
            const Uuid& name = cur.id();
            if (name.version() != NAME) {
                return Status::BAD_STATE.comment("not an annotation");
            }
            if (cur.ref() != id) {
                return Status::BAD_STATE.comment("annotation for a wrong op");
            }
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
        }*/

    inline bool is_head() const { return chain == id.value(); }

    inline Uuid chain_id() const { return Uuid::Time(chain, id.origin()); }
};

}  // namespace ron

#endif  // CPP_OPMETA_HPP
