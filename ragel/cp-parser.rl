#include "encdec.hpp"
#include "text.hpp"
#include <iostream>

using namespace std;

namespace ron {

%% machine CP;
%% write data;

bool TextFrame::StringIterator::Next () {
    CharRef p = data_.begin();
    CharRef pe = data_.end();
    /*if (p==pe) {
        cp_ = 0;
        return false;
    }*/
    CharRef eof = 0;
    Codepoint cp = 0;
    int cs=0;
    %%write init;
    %%{
        include TEXT_FRAME "./text-grammar.rl";
        action end_cp { --p; fbreak; }
        CP1 = (CP) %end_cp;
        main := CP1*;
        write exec;
    }%%
    if (cs!=CP_error) {
        cp_ = cp;
        data_.consume(p-data_.begin());
        return true;
    } else {
        cp_ = 0;
        return false;
    }
}


}
