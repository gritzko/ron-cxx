#include "uuid.hpp"

namespace ron {

    Uuid::Uuid (Slice data) noexcept {

        %% machine UUID;
        %% write data;

        CharRef pb = data.buf_;
        CharRef pe = pb+data.size_;
        CharRef p = pb;
        CharRef eof = pe;
        CharRef uuidb{p}, wordb{p};
        int cs = 0;
        Slice value{}, origin{};
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
