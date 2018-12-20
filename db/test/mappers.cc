#include <unistd.h>
#include "ron/ron.hpp"
#include "db/replica.hpp"
using namespace ron;
using namespace std;
typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Replica<Frame> TextReplica;


void test_db_json_mapper () {
}

int main (int argc, const char** args) {
    test_db_json_mapper();
}