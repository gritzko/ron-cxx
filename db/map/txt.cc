#include "../../rdt/rga.hpp"
#include "../map.hpp"

using namespace std;

namespace ron {

template <typename Frame>
Status TxtMapper<Frame>::Read(Builder& response, Cursor& query, Uuid branch) {
    Uuid id = query.id().event();
    Frame state;
    Status ok = store_.Get(Key{id, RGA_RDT_FORM}, state);
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
    response.AppendNewOp(RAW, id, TXT_MAP_ID, text);
    return Status::OK;
}

template <typename Frame>
Status TxtMapper<Frame>::Write(Records& save, Cursor& query, Uuid branch) {
    return Status::NOT_IMPLEMENTED;
}

template class TxtMapper<TextFrame>;

}  // namespace ron
