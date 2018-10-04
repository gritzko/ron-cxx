#include "uuid.hpp"

namespace ron {


    Uuid::Uuid (const char* buf, size_t len) {
        %% machine UUID;
        %% write data;
        int atm=0, dgt=0, hlf=0;
        auto pe = buf+len;
        auto p = buf;
        auto eof = pe;
        int cs = 0;

        %%{ 

            include UUID "./uuid-grammar.rl";
            main := UUID ;

            write init;
            write exec;
        }%%

    }

}
