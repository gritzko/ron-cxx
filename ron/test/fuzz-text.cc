#include "../ron.hpp"

using namespace ron;
using namespace std;

using Frame = TextFrame;
using Cursor = typename Frame::Cursor;
using Builder = typename Frame::Builder;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    Slice data{Data, (fsize_t)Size};
    Cursor cur{data};
    Status ok;
    while ( (ok=cur.Next()) );
    if (!ok) return 0;
    Builder copyb{};
    Cursor c{data};
    while (c.Next()) {
        copyb.AppendOp(c);
    }
    Frame copy = copyb.Release();

    Cursor a{data};
    Cursor b = copy.cursor();
    assert(CompareWithCursors<Cursor>(a, b));

    return 0;
}
