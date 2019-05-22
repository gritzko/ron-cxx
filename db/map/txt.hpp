#include "../../rdt/rga.hpp"
#include "../map.hpp"
#include "dmp_diff.hpp"

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
    response.AppendNewOp(id.derived(), TXT_MAP_ID, text);
    query.Next(); // consume the query
    return Status::OK;
}

template <typename Commit>
Status TxtMapper<Commit>::ReadVersion(Builder &response, Cursor &query, Commit &branch){
    Uuid id = query.id();
    Uuid version = query.uuid(3);
    Frame snapshot;
    IFOK(branch.GetObjectVersion(snapshot, id, RGA_FORM_UUID, version));
    String string;
    frame2string(string, snapshot);
    response.AppendNewOp(id, TXT_MAP_ID, string);
    return Status::OK;
}

template <typename Commit>
Status TxtMapper<Commit>::ReadPatch(Builder &response, Cursor &query, Commit &branch){
    // W E A V E
    Booleans ts;
    Frame last_ver;
    VV vv;
    Codepoints weave;
    // 1. get log
    // 2. get ignore vv
    IFOK(ScanRGA(ts, last_ver, vv));
    IFOK(ScanRGA(ts, last_ver));
    fsize_t len;
    Codepoints ins;
    // we have two visibility maps
    // scan both, serialize changes
    response.WriteUuid(RM_UUID);
    response.WriteInt(len);
    response.WriteString(ins);
    return Status::NOT_IMPLEMENTED;
}

template <typename Commit>
Status TxtMapper<Commit>::WriteDiffs(const Ids& ids, const Diffs& dmp, Commit& branch) {
    Builder patch;
    Uuid now = branch.Now();
    auto i = ids.begin();
    fsize_t k, l;
    // FIXME out-of-bounds checks
    for(auto d=dmp.begin(); d!=dmp.end(); ++d) {
        switch (d->operation) {
            case Operation::EQUAL:
                i += d->text.size();
                break;
            case Operation::INSERT:
                for(auto i=d->text.from; i!=d->text.till; ++i) {
                    //patch.AppendNewOp(now.inc(), now, *i);
                    ++now;
                }
                break;
            case Operation::DELETE:
                k = d->text.size();
                while (k) {
                    l = i.skip_span(k);
                    i += l;
                    Uuid prev = *i;
                    while (l--) {
                        patch.AppendNewOp(now, prev, RM_UUID);
                        prev = now;
                        ++now;
                    }
                }
                // scan spans, delete
                break;
        }
    }
    Frame edits = patch.Release();
    Cursor pc{edits};
    Builder re;
    return branch.WriteNewEvents(re, pc);
}

template<class Commit>
Status TxtMapper<Commit>::RGA2Codepoints (Codepoints& text, Ids& ids, const Frame& frame, const Booleans& tombs) {
    Cursor c{frame};
    auto i = tombs.begin();
    while (c.valid()) {
        if (!*i) {
            // FIXME if any
            // FIXME perf?
            StringIterator s{c.string_slice(2)};
            text.push_back(*s);
        }
        ++i;
        c.Next();
    }
    return Status::OK;
}

template <typename Commit>
Status TxtMapper<Commit>::WriteState(Builder& response, Cursor& query, Commit& branch) {
    Codepoints new_state;
    Codepoints old_text, new_text;
    for(StringIterator cpi{query.string_slice(2)}; cpi; ++cpi)
        new_text.push_back(*cpi);
    Booleans tombs;
    Frame old;
    Ids ids;
    IFOK(branch.GetObject(old, query.id(), RGA_FORM_UUID));
    IFOK(ScanRGA(tombs, old));
    IFOK(RGA2Codepoints(new_text, ids, old, tombs));
    DMP dmp{old_text, new_text};
    return WriteDiffs(ids, dmp.diffs(), branch);
}


template <typename Commit>
Status TxtMapper<Commit>::WritePatch(Builder& response, Cursor& query, Commit& branch) {
    Diffs diff;
    for(fsize_t i = 3; i<query.size(); i+=2) {
        if (!query.has(i, UUID)) {
            return Status::BADARGS.comment("must be eq/rm/un");
        }
        Uuid command = query.uuid(i);
        if (command==EQ_UUID) {
            diff.emplace_back();
        } else if (command==RM_UUID) {
            diff.emplace_back();
        } else if (command==IN_UUID) {
            diff.emplace_back();
        }
    }
    // parse the patch
    return WriteDiffs(diff, branch);
}

const Uuid SINCE_COMMAND{"since"};
const Uuid PATCH_COMMAND{"patch"};
const Uuid HILI_COMMAND{"hili"};

template <typename Commit>
Status TxtMapper<Commit>::Write(Builder& response, Cursor& query, Commit& branch) {
    if (query.size()<3) {
        return Status::BADARGS.comment("no arguments in a txt write");
    }
    if (query.has(2, STRING)) {
        return WriteState(response, query, branch);
    }
    if (!query.has(2, UUID)) {
        return Status::BADARGS.comment("unknown txt write op pattern");
    }
    Uuid command = query.uuid(2);
    if (command==SINCE_COMMAND) {
        return Status::NOT_IMPLEMENTED;
    } else if (command==HILI_COMMAND) {
        return Status::NOT_IMPLEMENTED;
    } else {
        return Status::BADARGS.comment("unknown txt write command");
    }
}


}  // namespace ron
