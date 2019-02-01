#include "map.hpp"

namespace ron {

    template<typename Frame>
    Status TxtMapper<Frame>::Map(Builder& response, Cursor& query, const VV& hili) const {
        Frame state;
        //CT ct{state.cursor()};
        string text;
        // now, walk em both

        // 0. 2-pass algo
        CT{};
        // 1. build ct
        // 2. reverse: pick rm-ranges (heap, start-pos-len, all fsize_t)
        // 3. walk the frame, make the string

        // 2. walk the frame again, add non-rm data, color it

        // iterate
        //  insert?
        //    append letter (?!!! - variable length, string metrics)
        //    append to ct
        //  remove?
        //    see depth
        //    unroll, mark
        return Status::NOT_IMPLEMENTED;
    }

    template class OpMapper<TextFrame>;

}
