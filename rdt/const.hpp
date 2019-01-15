#ifndef CPP_RDT_CONST_HPP
#define CPP_RDT_CONST_HPP

namespace ron {

enum RDT : uint8_t {
    META_RDT,
    CHAIN_RDT,
    ACK_RDT,
    LWW_RDT,
    RGA_RDT,
    MX_RDT,
    RDT_COUNT
};

static const uint64_t META_RDT_NAME{894494834235015168UL};
static const Uuid META_RDT_ID{META_RDT_NAME, 0};
static const uint64_t CHAIN_RDT_NAME{715112314629521408UL};
static const Uuid CHAIN_RDT_ID{CHAIN_RDT_NAME, 0};
static const uint64_t ACK_RDT_NAME{677716977128570880UL};
static const Uuid ACK_RDT_ID{ACK_RDT_NAME, 0};
static const uint64_t LWW_RDT_NAME{881557636825219072UL};
static const Uuid LWW_RDT_ID{LWW_RDT_NAME, 0};
static const uint64_t RGA_RDT_NAME{985043671231496192UL};
static const Uuid RGA_RDT_ID{RGA_RDT_NAME, 0};
static const uint64_t MX_RDT_NAME{899594025567256576UL};
static const Uuid MX_RDT_ID{MX_RDT_NAME, 0};

static const Uuid RDT_IDS[] = {META_RDT_ID, CHAIN_RDT_ID, ACK_RDT_ID,
                               LWW_RDT_ID,  RGA_RDT_ID,   MX_RDT_ID,
                               FATAL};

inline Uuid rdt2uuid(RDT e) { return RDT_IDS[e]; }

inline RDT uuid2rdt(const Uuid& u) {
    switch (uint8_t(u.value()._64 >> 52)) {
        case uint8_t(META_RDT_NAME >> 52):
            return u == META_RDT_ID ? META_RDT : RDT_COUNT;
        case uint8_t(CHAIN_RDT_NAME >> 52):
            return u == CHAIN_RDT_ID ? CHAIN_RDT : RDT_COUNT;
        case uint8_t(ACK_RDT_NAME >> 52):
            return u == ACK_RDT_ID ? ACK_RDT : RDT_COUNT;
        case uint8_t(LWW_RDT_NAME >> 52):
            return u == LWW_RDT_ID ? LWW_RDT : RDT_COUNT;
        case uint8_t(RGA_RDT_NAME >> 52):
            return u == RGA_RDT_ID ? RGA_RDT : RDT_COUNT;
        case uint8_t(MX_RDT_NAME >> 52):
            return u == MX_RDT_ID ? MX_RDT : RDT_COUNT;
        default:
            return RDT_COUNT;
    }
}

}  // namespace ron

#endif  // CPP_CONST_HPP
