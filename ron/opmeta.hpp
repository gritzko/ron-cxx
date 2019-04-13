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
    /** the type of the object */
    Uuid rdt;  // FIXME FORM
    /** the object (the causal tree the op is attached to) */
    Uuid object;
    /** op Merkle hash */
    SHA2 hash;
    /** yarn-previous op id */
    Word prev;
    /** chain id */
    Word chain;

    static Uuid SHA2_UUID;
    static Uuid OBJ_UUID;
    static Uuid RDT_UUID;
    static Uuid PREV_UUID;
    static Uuid HEAD_UUID;

    OpMeta(const OpMeta& orig) = default;

    /** Nil op metadata @0:0; */
    OpMeta()
        : id{Uuid::NIL},
          rdt{Uuid::NIL},
          object{Uuid::NIL},
          hash{SHA2::ZERO},
          prev{Word{}},
          chain{Word{}} {}

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

    template <typename Cursor>
    inline bool is_next(const Cursor& cur) const {
        return cur.id().version() == TIME && cur.id().origin() == id.origin() &&
               cur.id() > id && cur.ref() == id;
    }

    template <typename Cursor>
    inline bool is_check(const Cursor& cur) const {
        return cur.id().version() == NAME && cur.ref() == id;
    }

    /** Advance to the next op in the yarn.
     * @param{op} the next op
     * @param{ref} the meta for the op referenced by the new op
     * @return should be OK
     */
    template <class Cursor>
    Status Next(const Cursor& op, const OpMeta& refd) {
        prev = id.value();
        id = op.id();
        Uuid ref = op.ref();
        if (ref.origin() != id.origin() || prev != ref.value()) {
            chain = id.value();
        }
        rdt = refd.rdt;
        object = refd.object;
        hash = SHA2::OpMerkleHash(op, hash, refd.hash);
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
        save.AppendNewOp(id, META_FORM_UUID, object, rdt, hash.base64());
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
        // hash;
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
