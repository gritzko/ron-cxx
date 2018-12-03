#include "text.hpp"

using namespace std;

namespace ron {

int64_t TextFrame::Cursor::integer(fsize_t idx) {
  const slice_t range{frame_.data(), op_.atom(idx).range()};
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
  return neg ? -ret : ret;
}

double TextFrame::Cursor::number(fsize_t idx) {
  slice_t range{frame_.data(), op_.atom(idx).range()};
  char fs[32];  // FIXME size limits
  strncpy(fs, range.buf_, range.size_);
  fs[range.size_] = 0;
  double ret;
  sscanf(fs, "%lf", &ret);
  return ret;
}

std::string TextFrame::Cursor::string(fsize_t idx) const {
  slice_t range{frame_.data(), op_.atom(idx).range()};
  std::string ret{};
  for (auto c = range.begin(); c < range.end(); c++) {
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
    }
    ret.push_back(unesc);
    c++;
  }
  return ret;
}

}  // namespace ron
