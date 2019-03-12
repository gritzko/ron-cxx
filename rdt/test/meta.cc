#include <iostream>
#include <cassert>
#include "ron/ron.hpp"
#include "rdt/rdt.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

typedef MetaRDT<typename ron::TextFrame> TextMeta;
using Frame = TextFrame;

int main (int argn, char** args) {
    TextMeta reducer;
    typedef TextFrame::Cursors Cursors;
    typedef TextFrame::Cursor Cursor;
    string f1 = "@now :12345+origin; @pubkey 'ABCDEF...';";
    string f2 = "@now :34567+origin;";
    string f3 = "@now :23456+origin;";
    Cursors inputs{
        Cursor{f1},
        Cursor{f2},
        Cursor{f3},
    };
    Frame now;
    assert(MergeCursors<Frame>(now, META_RDT, inputs));
    assert(now.data()=="@now :34567+origin;\n@pubkey 'ABCDEF...';\n");
}
