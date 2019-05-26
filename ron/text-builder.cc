#include <cinttypes>
#include "text.hpp"

namespace ron {

Result TextFrame::Builder::WriteValues(const Cursor& cur) {
    const Atoms& op = cur.op();
    for (fsize_t i = 2; i < op.size(); i++) {
        const Atom& atom = op[i];
        Write(' ');
        switch (atom.type()) {
            case INT:
                Write(cur.data(atom));
                break;
            case UUID:
                if (A2U(op[i]).is_ambiguous()) Write(ATOM_PUNCT[UUID]);
                WriteUuid(A2U(op[i]));
                break;
            case STRING:
                Write(ATOM_PUNCT[STRING]);
                Write(cur.data(atom));
                Write(ATOM_PUNCT[STRING]);
                break;
            case FLOAT:
                Write(cur.data(atom));
                break;
        }
    }
    return OK;
}

void TextFrame::Builder::WriteInt(int64_t value) {
    char tmp[20];
    int len = sprintf((char*)tmp, "%" PRId64, value);
    data_.append(tmp, static_cast<size_t>(len));
}

void TextFrame::Builder::WriteUuid(const Uuid value) {
    // FIXME escaping
    data_.append(value.str());
}

void TextFrame::Builder::WriteFloat(double value) {
    char tmp[20];
    int len = sprintf((char*)tmp, "%le", value);
    data_.append(tmp, static_cast<size_t>(len));
}

void TextFrame::Builder::WriteString(const String& value) {
    String esc;
    escape(esc, value);
    data_.append(esc);
}

void TextFrame::Builder::escape(String& to, const Slice& buf) {
    for (char i : buf) {
        switch (i) {
            case '\"':
                to.push_back(ESC);
                to.push_back('"');
                break;
            case '\'':
                to.push_back(ESC);
                to.push_back('\'');
                break;
            case '\\':
                to.push_back(ESC);
                to.push_back('\\');
                break;
            case '\b':
                to.push_back(ESC);
                to.push_back('b');
                break;
            case '\f':
                to.push_back(ESC);
                to.push_back('f');
                break;
            case '\n':
                to.push_back(ESC);
                to.push_back('n');
                break;
            case '\r':
                to.push_back(ESC);
                to.push_back('r');
                break;
            case '\t':
                to.push_back(ESC);
                to.push_back('t');
                break;
            default:
                to.push_back(i);
                break;
        }
    }
}

}  // namespace ron
