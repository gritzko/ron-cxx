#include "uuid.hpp"
#include <inttypes.h>

namespace ron {

    constexpr int8_t Word::OFFSET6[10] ;
    constexpr uint64_t Word::LOWER6[11];
    const Uuid Uuid::ZERO{};
    const Uuid Uuid::FATAL{Word::MAX_VALUE, Word::MAX_VALUE};


    std::string Value::str(const char *buf) const {
        char pad[32];
        int l;
        fsize_t i0, i1;
        switch (type()) {
            case ATOM::INT:
                l = sprintf(pad, "%" PRId64, int_value());
                return std::string(pad, l);
            case ATOM::FLOAT:
                l = sprintf(pad, "%lf", float_value());
                return std::string(pad, l);
            case ATOM::STRING:
                if (!buf) return std::string();
                i0 = origin().get30(0);
                i1 = origin().get30(1);
                return std::string(buf + i0, i1);
            case ATOM::BUF:
                if (!buf) return std::string();
                i0 = origin().get30(0);
                i1 = origin().get30(1);
                return std::string(buf + i0, buf + i0 + i1);
        }
    }


    Word::Word (const char* word, fsize_t size) {
        _64[0] = 0;
        for(int i=0; i<size; i++) {
            put6(9-i, ABC[word[i]]); // TODO check
        }
    }


    int Word::write_base64(std::string &str) const {
        uint64_t val = _64[0];
        int len = 0;
        uint64_t mask = Word::MAX_VALUE;
        do {
            str.push_back(BASE_PUNCT[0x3f&(val>>OFFSET6[len])]);
            len++;
        } while (val&LOWER6[len]);
        return len;
    }

    std::string Uuid::str () const {
        std::string ret;
        int vrt = variety();
        if (vrt) {
            ret.push_back(BASE_PUNCT[vrt]);
            ret.push_back('/');
        }
        words_.first.write_base64(ret);
        int schm = version();
        if (schm!=0 || !origin().is_zero()) {
            ret.push_back(UUID_PUNCT[schm]);
            words_.second.write_base64(ret);
        }
        return ret;
    }

    std::string unescape (const char* buf, fsize_t size) {
        // TODO
        return "";
    }

    std::string escape (const char* buf, fsize_t size) {
        // TODO
        return "";
    }

    uint8_t ABC[128] = { 255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,2,255,1,0,1,255,2,0,5,0,2,1,3,0,255,0,1,2,3,4,5,6,7,8,9,3,0,255,1,0,3,2,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,1,255,4,3,36,0,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,2,255,3,63,255 };
    uint64_t IS_BASE[2] = {0x03ff000000000000, 0x47fffffe87fffffe};

}