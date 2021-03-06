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
                if (A2U(atom).is_ambiguous()) {
                    Write(ATOM_PUNCT[UUID]);
                }
                WriteUuid(A2U(atom));
                break;
            case STRING:
                Write(ATOM_PUNCT[STRING]);
                if (atom.value.cp) {  // a char
                    WriteCodepoint(atom.value.cp);
                } else {  // a string
                    Write(cur.data(atom));
                }
                Write(ATOM_PUNCT[STRING]);
                break;
            case FLOAT:
                Write(cur.data(atom));
                break;
        }
    }
    prev_2_ = span_signature(cur);
    return OK;
}

void TextFrame::Builder::WriteInt(int64_t value) {
    char tmp[20];
    int len = sprintf(tmp, "%" PRId64, value);
    data_.append(tmp, static_cast<size_t>(len));
}

void TextFrame::Builder::WriteUuid(const Uuid value) {
    data_.append(value.str());
}

void TextFrame::Builder::WriteFloat(double value) {
    char tmp[32];
    // floats are a pain; esp converting binary<->text
    // our only interest here is to preserve the hashes,
    // so binary->text->binary must not change the value;
    // text->binary->text we don't care about (see test/text.cc)
    // 17 is DBL_DECIMAL_DIG, enough to express 64 bit ISO floats
    int len = sprintf(tmp, "%.17G", value);
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
