#include <iostream>
#include <cassert>
#include "ron/ron.hpp"
#include "rdt/rdt.hpp"

using namespace ron;
using namespace std;

typedef MetaRDT<typename ron::TextFrame> TextMeta;


int main (int argn, char** args) {
    TextMeta reducer;
    typedef TextFrame::Cursors Cursors;
    typedef TextFrame::Cursor Cursor;
    Cursors inputs{
        Cursor{"@now :12345+origin; @pubkey 'ABCDEF...';"},
        Cursor{"@now :34567+origin;"},
        Cursor{"@now :23456+origin;"},
    };
    string now = Merge<TextFrame>(META, inputs);
    assert(now=="@now :34567+origin;\n@pubkey 'ABCDEF...';\n");
}