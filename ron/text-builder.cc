#include <cinttypes>
#include "text.hpp"

namespace ron {

void TextFrame::Builder::AddOp(const Op& op, const std::string& back_buf) {
  // TODO how do I know that the source frame is text, to avoid
  // re-serialization?
  data_.push_back(SPEC_PUNCT[SPEC::EVENT]);
  WriteUuid(op.id());
  data_.push_back(SPEC_PUNCT[SPEC::REF]);
  WriteUuid(op.ref());
  for (int i = 2; i < op.size(); i++) {
    const Value& a = op.value(i);
    switch (a.type()) {  // FIXME UUID is different
      case ATOM::INT:
        data_.push_back(ATOM_PUNCT[ATOM::INT]);
        WriteInt(a.int_value());
        break;
      case ATOM::UUID:
        data_.push_back(ATOM_PUNCT[ATOM::UUID]);
        WriteUuid(a.uuid_value());
        break;
      case ATOM::STRING:
        data_.push_back(ATOM_PUNCT[ATOM::STRING]);
        WriteString(a.string_value(back_buf));
        data_.push_back(ATOM_PUNCT[ATOM::STRING]);
        break;
      case ATOM::FLOAT:
        data_.push_back(ATOM_PUNCT[ATOM::FLOAT]);
        WriteFloat(a.float_value());
        break;
    }
  }
  data_.push_back(' ');
  data_.push_back(TERM_PUNCT[op.term_]);
  data_.push_back('\n');
}

void TextFrame::Builder::WriteInt(int64_t value) {
  char tmp[20];
  int len = sprintf(tmp, "%" PRId64, value);
  data_.append(tmp, (size_t)len);
}

void TextFrame::Builder::WriteUuid(const Uuid& value) {
  // FIXME escaping
  data_.append(value.str());
}

void TextFrame::Builder::WriteFloat(double value) {
  char tmp[20];
  int len = sprintf(tmp, "%le", value);
  data_.append(tmp, (size_t)len);
}

void TextFrame::Builder::WriteString(const std::string& value) {
  std::string esc;
  TextFrame::escape(esc, value);
  data_.append(esc);
}

void TextFrame::escape(std::string& to, const char* buf, fsize_t size) {
  while (size) {
    switch (*buf) {
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
        to.push_back(*buf);
        break;
    }
    buf++;
    size--;
  }
}

}  // namespace ron
