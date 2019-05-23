#include <cinttypes>
#include "text.hpp"

namespace ron {

void TextFrame::Builder::WriteValues(const Cursor& cur) {
    const Op& op = cur.op();
    for (fsize_t i = 2; i < op.size(); i++) {
        const Atom& atom = op.atom(i);
        Write(' ');
        switch (atom.type()) {
            case INT:
                Write(cur.slice(atom.origin.range()));
                break;
            case UUID:
                if (op.uuid(i).is_ambiguous()) Write(ATOM_PUNCT[UUID]);
                WriteUuid(op.uuid(i));
                break;
            case STRING:
                Write(ATOM_PUNCT[STRING]);
                Write(cur.slice(atom.origin.range()));
                Write(ATOM_PUNCT[STRING]);
                break;
            case FLOAT:
                Write(cur.slice(atom.origin.range()));
                break;
        }
    }
}

template <typename Cursor2>
void TextFrame::Builder::WriteValues(const Cursor2& cur) {
    const Op& op = cur.op();
    Range range;
    for (fsize_t i = 2; i < op.size(); i++) {
        Write(' ');
        // FIXME(gritzko) parse the values!
        switch (op.type(i)) {
            case INT:
                WriteInt(op.atom(i).value.as_integer);
                break;
            case UUID:
                if (op.uuid(i).is_ambiguous()) Write(ATOM_PUNCT[UUID]);
                WriteUuid(op.uuid(i));
                break;
            case STRING:
                Write(ATOM_PUNCT[STRING]);
                range = op.atom(i).origin.range();
                WriteString(unescape(
                    cur.data().slice(range)));  // FIXME(gritzko) clearly a bug
                Write(ATOM_PUNCT[STRING]);
                break;
            case FLOAT:
                WriteFloat(op.atom(i).value.as_float);
                break;
        }
    }
}

// template <typename Cursor2>
void TextFrame::Builder::AppendAmendedOp(const Cursor& cur, TERM newterm,
                                         const Uuid& newid,
                                         const Uuid& newref) {
    const Op& op = cur.op();
    WriteSpec(newid, newref);
    WriteValues(cur);
    WriteTerm(newterm);
}

template <typename Cursor2>
void TextFrame::Builder::AppendOp(const Cursor& cur) {
    const Op& op = cur.op();
    WriteSpec(op.id(), op.ref());
    WriteValues(cur);
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
