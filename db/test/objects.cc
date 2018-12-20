#include <unistd.h>
#include "ron/ron.hpp"
#include "db/replica.hpp"
using namespace ron;
using namespace std;
typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Replica<Frame> TextReplica;


void test_db_lww_object_merge () {
    TextReplica db{};
    string path = Uuid::HybridTime(time(nullptr)).str();
    assert(db.Create(path));
    assert(db.Receive("@1gIBSU+gritzko :lww ;"));
    assert(db.Receive("@1gIBSU0001+gritzko :1gIBSU+gritzko 'one' 1 ;"));
    string obj;
    assert(db.ReceiveRespond(obj, "@1gIBSU+gritzko :lww ?"));
    assert(obj == "@1gIBSU+gritzko :lww!\n 'one' 1,");
    assert(db.Close());
    rmdir(path.c_str());
}

void test_db_rga_object_merge () {
}

int main (int argc, const char** args) {
    test_db_lww_object_merge();
    test_db_rga_object_merge();
}