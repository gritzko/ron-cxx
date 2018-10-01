#include "uuid.hpp"

namespace ron {


    Uuid::Uuid (const std::string& buf) {
        %% machine UUID;
        %% write data;
        int atm=0, dgt=0, hlf=0;
        auto pe = buf.end();
        auto p = buf.begin();
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
