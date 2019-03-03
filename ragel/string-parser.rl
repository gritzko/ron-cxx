#include "stdlib.h"
#include <string>
#include <iostream>
#include "text.hpp"

%%{
    machine UTF8ESC;
    alphtype unsigned char;

    include TEXT_FRAME "./text-grammar.rl";

    action begin_uniesc { uniesc = p; }
    action end_uniesc   { push_back_uniesc(ret, uniesc, p); }

    action begin_esc { esc = p; }
    action end_esc   { push_back_esc(ret, esc, p); }

    action begin_cp { cp = p; }
    action end_cp   { push_back_cp(ret, cp, p); }

    UTF8ESC = (
          UNIESC >begin_uniesc %end_uniesc
        | ESC    >begin_esc %end_esc
        | CHAR   >begin_cp %end_cp
        )*;
}%%

namespace ron {

    using namespace std;

    using iterator = const unsigned char *;

    void push_back_codepoint (u16string& res, uint32_t codepoint) {
        if (codepoint<0xd800) {
            res.push_back((char16_t)codepoint);
        } else if (codepoint>=0xe000 && codepoint<0x10000) {
            res.push_back((char16_t)codepoint);
        } else {
            res.push_back(0xd800+(codepoint>>10));
            res.push_back(0xdc00+(codepoint&1023));
        }
    }

    void push_back_uniesc (u16string& res, iterator b, iterator e) {
        int sz = e-b;
        assert(sz==6);
        char num[5];
        memcpy(num, b+2, 4);
        num[4] = 0;
        uint32_t codepoint = strtol(num, NULL, 16);
        push_back_codepoint(res, codepoint);
        // FIXME validity
        // FIXME surrogates
    }

    void push_back_esc (u16string& res, iterator b, iterator e) {
        assert(*b=='\\');
        unsigned char c{*(b+1)};
        switch (c) {
            case 'b': c = '\b'; break;
            case 'f': c = '\f'; break;
            case 'n': c = '\n'; break;
            case 'r': c = '\r'; break;
            case 't': c = '\t'; break;
        }
        res.push_back(c);
    }

    void push_back_cp (u16string& res, iterator b, iterator e) {
        // deserialize
        const unsigned int sz = e-b;
        assert(sz && sz<=4);
        static uint8_t MASK[5] = {0, 127, 31, 15, 7};
        uint32_t codepoint{static_cast<uint32_t>(*b & MASK[sz])};
        switch (sz) {
            case 4:
               codepoint <<= 6;
               codepoint |= *(++b) & 63;
            case 3:
               codepoint <<= 6;
               codepoint |= *(++b) & 63;
            case 2:
               codepoint <<= 6;
               codepoint |= *(++b) & 63;
            case 1:
               break;
            default:
                assert(false);
        }
        // serialized
        push_back_codepoint(res, codepoint);
    }


    u16string TextFrame::utf16string(Atom str_atom) const {

        assert(str_atom.origin().flags()==STRING_ATOM);

        frange_t range = str_atom.origin().range();
        fsize_t offset = range.first, size = range.second;

        u16string ret;
        ret.reserve(size);

        iterator pb = (iterator)data_.data() + offset;
        auto pe = pb + size;
        auto p = pb;
        auto eof = pe;
        int cs = 0;

        iterator uniesc, esc, cp;

        %% machine UTF8ESC;
        %% write data;
        %% write init;

        %%{

            main := UTF8ESC;
            write exec;

        }%%

        // The string was already pre-parsed by Cursor::Next()
        assert(cs!=0);

        return ret;

    }

}
