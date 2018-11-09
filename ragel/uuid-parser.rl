#include "uuid.hpp"

namespace ron {

    void report(const char* pb, const char* p, const char* event) {
        std::cerr<<"at "<<(p-pb)<<" '"<<*p<<"' "<<event<<"\n";
    }

    Uuid::Uuid (slice_t data) {
        %% machine UUID;
        %% write data;

        const char* pb = data.buf_;
        const char* pe = data.buf_+data.size_;
        const char* p = data.buf_;
        const char* eof = pe;
        int cs = 0;

        slice_t value{}, origin{}, uuidb;
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
