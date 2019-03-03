#include "uuid.hpp"

namespace ron {

    Uuid::Uuid (Slice data) {
        using iterator = const unsigned char *;
        %% machine UUID;
        %% write data;

        iterator pb = (iterator)data.buf_;
        iterator pe = pb+data.size_;
        iterator p = pb;
        iterator eof = pe;
        int cs = 0;

        Slice value{}, origin{}, uuidb;
        char variety{'0'}, version{'$'};

        %%{ 

            include UUID "./uuid-grammar.rl";
            main := UUID ;

            write init;
            write exec;
        }%%

        if (cs && value.size()<=Word::MAX_BASE64_SIZE && origin.size()<=Word::MAX_BASE64_SIZE) {
            words_.first = Word{ABC[variety], value};
            words_.second = Word{ABC[version], origin};
        } else {
            *this = FATAL;
        }

    }

}
