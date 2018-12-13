#ifndef ron_frames_hpp
#define ron_frames_hpp

#include <cstring>
#include <vector>
#include "op.hpp"
#include "status.hpp"
#include "uuid.hpp"

namespace ron {

template <typename Frame>
std::vector<typename Frame::Cursor> cursors(const std::vector<Frame>& frames) {
    std::vector<typename Frame::Cursor> ret;
    for (int i = 0; i < frames.size(); i++) ret.push_back(frames[i].cursor());
    return ret;
}

}  // namespace ron
#endif
