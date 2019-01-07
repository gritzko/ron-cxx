#include "text.hpp"

using namespace std;

namespace ron {

int64_t TextFrame::Cursor::parse_int(slice_t range) {
    const char* i = range.begin();
    bool neg = false;
    if (*i == '-') {
        neg = true;
        i++;
    } else if (*i == '+') {
        i++;
    }
    int64_t ret = 0;
    while (i < range.end()) {
        ret *= 10;
        ret += *i - '0';
        i++;
    }
    if (neg) ret = -ret;
    return ret;
}

double TextFrame::Cursor::parse_float(slice_t range) {
    char fs[32];  // FIXME size limits
    strncpy(fs, range.buf_, range.size_);
    fs[range.size_] = 0;
    double ret = strtod(fs, nullptr);
    return ret;
}

std::string TextFrame::Cursor::unescape(const slice_t& data) {
    std::string ret{};
    for (auto c = data.begin(); c < data.end(); c++) {
        if (*c != ESC) {
            ret.push_back(*c);
            continue;
        }
        char escape = *(c + 1), unesc = escape;
        switch (escape) {
            case 'b':
                unesc = '\b';
                break;
            case 'f':
                unesc = '\f';
                break;
            case 'n':
                unesc = '\n';
                break;
            case 'r':
                unesc = '\r';
                break;
            case 't':
                unesc = '\t';
                break;
            default:
                unesc = escape;
        }
        ret.push_back(unesc);
        c++;
    }
    return ret;
}

}  // namespace ron
