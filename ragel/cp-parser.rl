#include "encdec.hpp"
#include "text.hpp"
#include <iostream>

using namespace std;

namespace ron {

%% machine CP;
%% write data;

Result TextFrame::Cursor::NextCodepoint (Atom& a) const {
    if (!a.value.cp_size) {
        a.value.cp = 0;
        return ENDOFINPUT;
    }
    Slice str_data = data(a);
    CharRef p = str_data.begin();
    CharRef pe = str_data.end();
    CharRef pb = p;
    CharRef eof = 0;
    Codepoint cp = 0;
    fsize_t cp_size{0};
    int cs=0;
    %%write init;
    %%{
        include TEXT_FRAME "./text-grammar.rl";
        action end_cp_brk { --p; fbreak; }
        CP1 = (CP) %end_cp_brk;
        main := CP1*;
        write exec;
    }%%
    if (cs!=CP_error) {
        a.value.cp = cp;
        --a.value.cp_size;
        a.origin.as_range.Consume(p-pb);
        assert(a.origin.as_range.valid());
        return OK;
    } else {
        a.value.cp = 0;
        return BADSYNTAX;
    }
}


}
