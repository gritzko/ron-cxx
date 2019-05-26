#include "testutil.hpp"
#include <unistd.h>
#include "../../ron/ron.hpp"
#include "../replica.hpp"
#include "../map.hpp"
#include "../fs.hpp"
#define DEBUG 1

using namespace ron;
using namespace std;

using Frame = TextFrame;
using Cursor = Frame::Cursor;
using Builder = typename Frame::Builder;
using Store = RocksDBStore<Frame>;
using TestReplica = Replica<Store>;
using Commit = typename TestReplica::Commit;
using Txt = TxtMapper<Commit>;


TEST(Mappers, Enums) {
    ASSERT_NE(uuid2map(Uuid{"op"}), uuid2map(Uuid{"obj"}));
}

TEST(Mappers, TxtRead) {
    TmpDir tmp;
    int a = 1;
    tmp.cd("MappersTxtRead");
    TestReplica replica;
    ASSERT_TRUE(IsOK(TestReplica::CreateReplica()));
    ASSERT_TRUE(IsOK(replica.Open()));
    Commit commit{replica};
    ASSERT_TRUE(IsOK(commit.NewObject(RGA_FORM_UUID)));
    Uuid id = commit.tip();
    Txt txt;
    Builder re;
    Frame q = OneOp<Frame>(id, TXT_FORM_UUID);
    Cursor qc{q};
    Frame w = OneOp<Frame>(id, TXT_FORM_UUID, "test");
    Cursor wc{w};
    ASSERT_TRUE(wc.valid());
//    ASSERT_TRUE(IsOK( txt.WriteState(re, wc, commit)) );
//    ASSERT_TRUE(IsOK( txt.Read(re, qc, commit) ));

    Frame correct = OneOp<Frame>(id, TXT_FORM_UUID, "test");
    Frame response = re.Release();
    cerr<<response.data()<<"\n";
    cerr<<correct.data();
    // NOT READY YET ASSERT_TRUE(IsOK( CompareFrames(correct, response) ));

}



int main (int argc, char** args) {

    if (getenv("TRACE")) {
        Key::trace_by_key = true;
    }

    ::testing::InitGoogleTest(&argc, args);
    return RUN_ALL_TESTS();
}
