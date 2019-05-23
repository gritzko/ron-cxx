#ifndef RON_VV_HPP
#define RON_VV_HPP
#include <unordered_map>
#include "uuid.hpp"

namespace ron {

class VV {
    std::unordered_map<Word, Word> vv_;

   public:
    inline Uuid get(Word origin) const {
        auto i = vv_.find(origin);
        return i == vv_.end() ? Uuid{Word{}, origin}
                              : Uuid{i->second, i->first};
    }
    inline bool covers(Uuid point) const {
        return get(point.origin).value >= point.value;
    }
    inline bool empty() const { return vv_.empty(); }
};

const VV EMPTY_VV{};

}  // namespace ron

#endif  // RON_VV_HPP
