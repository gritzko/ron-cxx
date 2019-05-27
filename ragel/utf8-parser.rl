#include "slice.hpp"
#include "status.hpp"
#include <iostream>

using namespace std;

namespace ron {

%% machine UTF8CP;
%% write data;

Result ParseUtf8 (Codepoints& into, const String& from) {
    CharRef p = reinterpret_cast<CharRef>(from.data());
    CharRef pe = p + from.size();
    CharRef eof = pe;
    Codepoint cp = 0;
    int cs=0;
    %%write init;
    %%{
        include UTF8 "./utf8-grammar.rl";
        action end_cp { 
            into.push_back(cp); 
            cp = 0;
        }
        CP2 = CODEPOINT %end_cp;
        main := CP2 *;
        write exec;
    }%%
    return cs>=UTF8CP_first_final ? OK : BADSYNTAX;
}


}
