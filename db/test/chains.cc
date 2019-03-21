#include <unistd.h>
#include "../../ron/ron.hpp"
#include "../replica.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;
typedef TextFrame Frame;
typedef Frame::Cursor Cursor;
typedef Replica<Frame> TextReplica;

void test_db_yarn_root () {
}


/*   FIXME   >>>> RecvChain !!!!
void test_chain_breaks () {
    Frame headf{"@1gA9cq+gritzko :lww; 'a' 1; 'b' 2.0;"};
    Frame::Cursor headc = headf.cursor();
    OpMeta headm;
    assert(headm.ScanChain(headc));
    assert(headm.object==Uuid{"1gA9cq+gritzko"});
    assert(headm.at==Uuid{"1gA9cq0002+gritzko"});
    assert(headm.ScanChain("@1gA9cz+notgritzko :1gA9cq0002+gritzko;")==Status::CHAINBREAK);
    assert(headm.ScanChain("@1gA9cq+gritzko :1gA9cq0002+gritzko;")==Status::REPEAT);
    assert(headm.ScanChain("@1gA9cq0003+gritzko :1gA8k+notgritzko;")==Status::CHAINBREAK);
    assert(headm.ScanChain("@obj 12345+obj! @1gA9cq0004+gritzko :1gA9cq0003+gritzko ;")==Status::TREEBREAK);
    assert(headm.ScanChain("@1gA9cq0004+gritzko :1gA8k0003+gritzko 'd' 4;")==Status::TREEGAP);
    assert(headm.ScanChain("@1gA9cq0003+gritzko :1gA9cq0002+gritzko 'c' 'three';"));
    assert(headm.ScanChain("@prev 1gA9cq0004+gritzko! @1gA9cq0005+gritzko :1gA8k0003+gritzko 'e' 5;")==Status::YARNGAP);
    assert(headm.ScanChain("@1gA9cq0004+gritzko :1gA9cq0003+gritzko 'd' 4;"));
    assert(headm.object==Uuid{"1gA9cq+gritzko"});
    assert(headm.at==Uuid{"1gA9cq0004+gritzko"});
}
 */

void test_keys () {
    string idstr = "1gA9cz+gritzko";
    Uuid id{idstr};
    Key chain{id, CHAIN_RAW_FORM};
    assert(chain.form()==CHAIN_RAW_FORM);
    Uuid id2 = chain.id();
    assert(id2==id);
    assert(id2.str()==idstr);
    Key derived{id.derived(), CHAIN_RAW_FORM};
    assert(derived.id()==id);
}

int main (int argc, const char** args) {
    test_keys();
    //test_chain_breaks();
    test_db_yarn_root();
}
