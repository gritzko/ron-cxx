#include "uuid.hpp"

namespace ron {


    Uuid::Uuid (slice_t data) {
        %% machine UUID;
        %% write data;

        const char* pe = data.buf_+data.size_;
        const char* p = data.buf_;
        const char* eof = pe;
        int cs = 0;

        slice_t value{}, origin{}, uuid{};
        char variety{'0'}, version{'$'};

        %%{ 

            include UUID "./uuid-grammar.rl";
            main := UUID ;

            write init;
            write exec;
        }%%

        if (cs) {
            words_.first = Word{ABC[variety], value};
            words_.second = Word{ABC[version], origin};
        } else {
            *this = FATAL;
        }

    }

}
