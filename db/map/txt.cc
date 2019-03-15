#include "../../rdt/rga.hpp"
#include "../map.hpp"

using namespace std;

namespace ron {

template <typename Frame>
Status TxtMapper<Frame>::Map(Builder& response, Cursor& query,
                             const VV& hili) const {
    Uuid id = query.id().event();
    Frame state;
    Status ok = host_->Get(state, id, RGA_RDT_ID);
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

template class TxtMapper<TextFrame>;

}  // namespace ron
