#include "text.hpp"

using namespace std;

namespace ron {

Result TextFrame::Cursor::ParseInteger(Atom& a) const {
    a.value.as_integer = 0;
    Slice range = data(a);
    CharRef i = range.begin();
    bool neg = false;
    if (*i == '-') {
        neg = true;
        i++;
    } else if (*i == '+') {
        i++;
    }
    int64_t& ret = a.value.as_integer;
    while (i < range.end()) {
        ret *= 10;
        ret += *i - '0';
        i++;
    }
    if (neg) ret = -ret;
    return OK;
}

Result TextFrame::Cursor::ParseFloat(Atom& a) const {
    a.value.as_float = 0;
    Slice range = data(a);
    char fs[32];  // FIXME size limits
    memcpy(fs, range.begin(), range.size());
    fs[range.size()] = 0;
    a.value.as_float = strtod(fs, nullptr);
    return OK;
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
