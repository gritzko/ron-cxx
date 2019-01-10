#include <unistd.h>
#include "ron/ron.hpp"
#include "db/replica.hpp"
#include "db/map/const.hpp"
using namespace ron;
using namespace std;
typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Replica<Frame> TextReplica;


void test_enums () {
    assert(uuid2map(Uuid{"op"}) != uuid2map(Uuid{"obj"}));
}

void test_db_json_mapper () {
}

int main (int argc, const char** args) {
    test_enums();
    test_db_json_mapper();
}