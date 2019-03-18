#ifndef RON_FORM_HPP
#define RON_FORM_HPP

#include <cstdint>
#include <vector>
#include "uuid.hpp"

namespace ron {

enum form_idx_t : uint8_t {

	ZERO_RAW_FORM = 0,
	YARN_RAW_FORM = 1,
	LOG_RAW_FORM = 2,
	TAIL_RAW_FORM = 3,
	PATCH_RAW_FORM = 4,
	SPAN_RAW_FORM = 5,
	CHAIN_RAW_FORM = 6,
	GRAPH_RAW_FORM = 7,
	META_META_FORM = 8,
	PREV_META_FORM = 9,
	OBJ_META_FORM = 10,
	SHA3_META_FORM = 11,
	VV_META_FORM = 12,
	LWW_RDT_FORM = 13,
	RGA_RDT_FORM = 14,
	MX_RDT_FORM = 15,
	JSON_MAP_FORM = 16,
	CSV_MAP_FORM = 17,
	TXT_MAP_FORM = 18,
	RESERVED_ANY_FORM = 200

};
const uint64_t FORMS[] {
	0UL, // zero
	1109533813702131712UL, // yarn
	879235468267356160UL, // log
	1019422101297168384UL, // tail
	947412314540212224UL, // patch
	1005594780505210880UL, // span
	715112314629521408UL, // chain
	789985133028442112UL, // graph
	894494834235015168UL, // meta
	952132676872044544UL, // prev
	929632683238096896UL, // obj
	1003339750876119040UL, // sha3
	1061160662199173120UL, // vv
	881557636825219072UL, // lww
	985043671231496192UL, // rga
	899594025567256576UL, // mx
	844371191501160448UL, // json
	718297752286527488UL, // csv
	1025941105738252288UL, // txt

};

const Uuid ZERO_FORM_UUID{FORMS[ZERO_RAW_FORM],0UL}; // NOLINT
const Uuid YARN_FORM_UUID{FORMS[YARN_RAW_FORM],0UL}; // NOLINT
const Uuid LOG_FORM_UUID{FORMS[LOG_RAW_FORM],0UL}; // NOLINT
const Uuid TAIL_FORM_UUID{FORMS[TAIL_RAW_FORM],0UL}; // NOLINT
const Uuid PATCH_FORM_UUID{FORMS[PATCH_RAW_FORM],0UL}; // NOLINT
const Uuid SPAN_FORM_UUID{FORMS[SPAN_RAW_FORM],0UL}; // NOLINT
const Uuid CHAIN_FORM_UUID{FORMS[CHAIN_RAW_FORM],0UL}; // NOLINT
const Uuid GRAPH_FORM_UUID{FORMS[GRAPH_RAW_FORM],0UL}; // NOLINT
const Uuid META_FORM_UUID{FORMS[META_META_FORM],0UL}; // NOLINT
const Uuid PREV_FORM_UUID{FORMS[PREV_META_FORM],0UL}; // NOLINT
const Uuid OBJ_FORM_UUID{FORMS[OBJ_META_FORM],0UL}; // NOLINT
const Uuid SHA3_FORM_UUID{FORMS[SHA3_META_FORM],0UL}; // NOLINT
const Uuid VV_FORM_UUID{FORMS[VV_META_FORM],0UL}; // NOLINT
const Uuid LWW_FORM_UUID{FORMS[LWW_RDT_FORM],0UL}; // NOLINT
const Uuid RGA_FORM_UUID{FORMS[RGA_RDT_FORM],0UL}; // NOLINT
const Uuid MX_FORM_UUID{FORMS[MX_RDT_FORM],0UL}; // NOLINT
const Uuid JSON_FORM_UUID{FORMS[JSON_MAP_FORM],0UL}; // NOLINT
const Uuid CSV_FORM_UUID{FORMS[CSV_MAP_FORM],0UL}; // NOLINT
const Uuid TXT_FORM_UUID{FORMS[TXT_MAP_FORM],0UL}; // NOLINT


/** By the definition, a name UUID must be pre-defined (transcendent).
 *  Hence, this header forward-defines all name UUIDs corresponding
 *  to data forms (mappers, RDTs, any other op groupings that could be
 *  sent over the wire).
 *  Those name UUIDs get mapped to an internal 8-bit id. First, to
 *  use switch(){} everywhere we could. Second, to fit the @id:form
 *  pair into 16-byte keys (8 bits of UUID are guessable in this case).
 *  swarmdb key-value records are very fine-grained, so this helps. */
class Form {

    form_idx_t idx_;

public:

    Form(Uuid name) : idx_{ZERO_RAW_FORM} {
        uint8_t i{0};
        while (i<sizeof(FORMS) && FORMS[i]!=static_cast<uint64_t>(name.value())) ++i;
        idx_ = i == sizeof(FORMS) ? ZERO_RAW_FORM : static_cast<form_idx_t>(i);
    }

    Uuid id() const {
        return Uuid{FORMS[idx_], 0};
    }

    form_idx_t index () const {
        return idx_;
    }

};

}

#endif //CPP_FORM_HPP

