#include <iostream>
#include <cassert>
#include "ron/ron.hpp"
#include "rdt/rdt.hpp"

using namespace ron;
using namespace std;

typedef RGArrayRDT<typename ron::TextFrame> RGA;

string despace (string& orig) {
    string ret;
    bool ows{false};
    for (char &i : orig) {
        if (i ==' ' || i =='\n' || i =='\r') continue;
        ret.push_back(i);
    }
    return ret;
}

void test_chain_merge () {
    string abc = "@1+A :rga! 'a', 'b', 'c',";
    string def = "@1000000004+B :1000000003+A 'D', 'E', 'F', ";
    string abcdef = Merge<TextFrame>(RGA_RDT, {abc, def});
    string correct = "@1+A :rga; 'a'; 'b'; 'c'; @1000000004+B 'D'; 'E'; 'F';";
    // FIXME punkt!!!
    assert(despace(abcdef)==despace(correct));
}

void test_sibling_merge () {
    string parent = "@1+A :rga!";
    string childA = "@1a+B :1+A 'b';";
    string childB = "@1b+C :1+A 'a';";
    string ab =  Merge<TextFrame>(RGA_RDT, {parent, childA, childB});
    string correct = "@1+A :rga; @1b+C 'a'; @1a+B :1+A 'b'; ";
    assert(despace(ab)==despace(correct));
}

int main (int argn, char** args) {
    test_chain_merge();
    test_sibling_merge();
    return 0;
}