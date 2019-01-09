#include "map.hpp"

namespace ron {

    template<typename Frame>
    Status CSVMapper<Frame>::Map(Builder& response, Cursor& query, const VV& hili) const {
        if (query.id()!=CSV_MAPPER_ID) return Status::BAD_STATE;
        Uuid obj = query.ref(); // TODO version - Replica API
        Frame mx;
        Status ok = host_->Get(mx, obj, MX_UUID);
        if (!ok) return ok;
        // output, comma-separated, check strings
        Cursor i = mx.cursor();
        if (!i.valid() || i.ref()!=MX_UUID)
            return Status::NOT_FOUND.comment("not a mx object");
        mx_t values;
        fsize_t maxcol{0}, maxrow{0};
        while (i.Next()) {
            mxidx_t at = readmxidx(i);
            if (at==MX_IDX_MAX) continue;
            if (i.size()<2+2+1) continue;
            values[at] = i.atom(4);
            if (at.first>maxcol) maxcol = at.first;
            if (at.second>maxrow) maxrow = at.second;
        }
        std::string csv;
        for(fsize_t r=0; r<=maxrow; r++) {
            for(fsize_t c=0; c<=maxcol; c++) {
                const Atom x = values[mxidx_t{r,c}];
                if ((*(Uuid*)&x)==Uuid::NIL) { // FIXME atom -> pair !!! less boilerplate
                } if (x.type()==INT) {
                    csv.append(std::to_string((int64_t)x.value()));
                } else {
                    csv.append("NOTIMPL");
                }
                csv.push_back(',');
            }
            csv.push_back('\n');
        }
        return Status::OK;
    }

    template<typename Frame>
    Status CSVMapper<Frame>::Write(Builder& patch, Cursor& query) const {
        mx_t values{}, changes{};
        // fetch the state
        // scan, write changes (cancel too)
        // now(), write the patch
        return Status::NOT_IMPLEMENTED;
    }


}