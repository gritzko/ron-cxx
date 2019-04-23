#include "../../rdt/rga.hpp"
#include "../map.hpp"

using namespace std;

namespace ron {

template <typename Commit>
Status TxtMapper<Commit>::Read(Builder& response, Cursor& query, Commit& branch) {
    Uuid id = query.id().event();
    Frame state;
    IFOK( branch.Read(Key{id, RGA_RDT_FORM}, state) );
    vector<bool> tombs;
    ScanRGA<Frame>(tombs, state);
    // now, walk em both
    String text;
    Cursor c = state.cursor();
    fsize_t pos = 0;
    while (c.valid()) {
        if (!tombs[pos]) {
            text.append(c.string(2));
        }
        c.Next();
        pos++;
    }
    response.AppendNewOp(id, TXT_MAP_ID, text);
    query.Next(); // consume the query
    return Status::OK;
}

template <typename Commit>
Status TxtMapper<Commit>::Write(Builder& response, Cursor& query, Commit& branch) {
    return Status::NOT_IMPLEMENTED;
}


}  // namespace ron
