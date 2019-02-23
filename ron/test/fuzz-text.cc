#include "ron/ron.hpp"

using namespace ron;
using namespace std;

using Frame = TextFrame;
using Cursor = typename Frame::Cursor;
using Builder = typename Frame::Builder;

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    slice_t data{Data, Size};
    Cursor cur{data};
    if (!cur.valid()) return 0;
    Status ok;
    while ( (ok=cur.Next()) );
    if (!ok) return 0;
    Builder copyb{};
    Cursor c{data};
    do {
        copyb.AppendOp(c);
    } while (c.Next());
    Frame copy = copyb.frame();

    Cursor a{data};
    Cursor b = copy.cursor();
    assert(CompareWithCursors<Cursor>(a, b));

    return 0;
}
