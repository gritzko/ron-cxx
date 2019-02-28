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

Status TextFrame::Split(std::vector<TextFrame>& to) {
    const string& d = data_;
    const size_t s = d.size();
    size_t pos = 0;
    do {
        size_t at = d.find(".\n", pos);
        if (at == string::npos) {
            at = s;
        } else {
            at += 2;
        }
        to.emplace_back(d.substr(pos, at - pos));
        pos = at;
        while (pos < s && isspace(d[pos])) pos++;
    } while (pos < s);
    return Status::OK;
}

bool TextFrame::Cursor::int_too_big(const slice_t& data) {
    static const char* MAXINTSTR = "9223372036854775808";
    if (data.size() < 19) {
        return false;
    }
    if (data.size() > 20) {
        return true;
    }
    const char* mem = data.begin();
    size_t sz = data.size();
    if (*mem == '-' || *mem == '+') {
        ++mem;
        --sz;
    }
    return memcmp(mem, MAXINTSTR, sz) > 0;
}

}  // namespace ron
