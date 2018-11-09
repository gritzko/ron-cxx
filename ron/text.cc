#include "ron/text.hpp"

using namespace std;

namespace ron {

    int64_t TextFrame::Cursor::integer(fsize_t idx) {
        slice_t range{frame_.data(), op_.atom(idx).range()};
        const char* i = range.begin();
        bool neg = false;
        if (*i=='-') {
            neg = true;
            i++;
        } else if (*i=='+') {
            i++;
        }
        int64_t ret = 0;
        while (i<range.end()) {
            ret *= 10;
            ret += *i - '0';
            i++;
        }
        return neg ? -ret : ret;
    }

    double TextFrame::Cursor::number(fsize_t idx) {
        slice_t range{frame_.data(), op_.atom(idx).range()};
        char fs[32]; // FIXME size limits
        strncpy(fs, range.buf_, range.size_);
        fs[range.size_] = 0;
        double ret;
        sscanf(fs, "%lf", &ret);
        return ret;
    }

    std::string TextFrame::Cursor::string(fsize_t idx) {
        slice_t range{frame_.data(), op_.atom(idx).range()};
        return range.str();
    }


}