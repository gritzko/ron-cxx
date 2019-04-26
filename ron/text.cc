#include "text.hpp"

using namespace std;

namespace ron {

int64_t TextFrame::Cursor::parse_int(Slice range) {
    CharRef i = range.begin();
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

double TextFrame::Cursor::parse_float(Slice range) {
    char fs[32];  // FIXME size limits
    memcpy(fs, range.buf_, range.size_);
    fs[range.size_] = 0;
    double ret = strtod(fs, nullptr);
    return ret;
}

String TextFrame::unescape(const Slice& data) {
    String ret{};
    for (auto c = data.begin(); c < data.end(); c++) {
        if (*c != ESC) {
            ret.push_back(*c);
            continue;
        }
        char escape = *(c + 1);
        ret.push_back(decode_esc(escape));
        c++;
    }
    return ret;
}

Status TextFrame::Split(std::vector<TextFrame>& to) {
    const String& d = data_;
    const size_t s = d.size();
    size_t pos = 0;
    do {
        size_t at = d.find(FRAME_TERM, pos);
        if (at == String::npos) {
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

bool TextFrame::Cursor::int_too_big(const Slice& data) {
    static const char* MAXINTSTR = "9223372036854775807";
    if (data.size() < 19) {
        return false;
    }
    CharRef mem = data.begin();
    size_t sz = data.size();
    if (*mem == '-' || *mem == '+') {
        ++mem;
        --sz;
    }
    if (sz > 19) {
        return true;
    }
    int res = memcmp(mem, MAXINTSTR, sz);
    return res > 0;
}

inline char TextFrame::decode_esc(char esc) {
    switch (esc) {
        case 'a':
            return '\a';
        case 'b':
            return '\b';
        case 'f':
            return '\f';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        default:
            return esc;
    }
}

Codepoint TextFrame::decode_hex_cp(Slice data) {
    Codepoint ret = 0;
    while (!data.empty()) {
        ret <<= 4;
        int8_t i = ABC16[*data];
        if (i == -1) return 0;
        ret |= i;
        ++data;
    }
    return ret;
}

}  // namespace ron
