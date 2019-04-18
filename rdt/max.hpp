#ifndef rdt_max_hpp
#define rdt_max_hpp

#include <unordered_map>
#include "../ron/form.hpp"
#include "merge.hpp"

namespace ron {

template <class Frame>
class MaxRDT {
    using Builder = typename Frame::Builder;
    using Cursor = typename Frame::Cursor;
    using Cursors = typename Frame::Cursors;

    static bool Less(const Cursor &a, const Cursor &b) {
        if (a.size() == 2) {
            if (b.size() > 2) return true;
            return b.id() > a.id();
        }
        if (b.size() == 2) return false;
        return a.atom(2).words_ < b.atom(2).words_;
    }

   public:
    Status Merge(typename Frame::Builder &output, Cursors &inputs) const {
        if (inputs.empty()) return Status::OK;
        if (inputs[0].valid() && inputs[0].ref() == MAX_FORM_UUID) {
            output.AppendOp(inputs[0]);
            inputs[0].Next();
        }
        // TODO  sanity: all ops ref the root
        Cursor max = inputs.back();
        inputs.back().Next();
        for (int i = inputs.size() - 1; i >= 0; --i) {
            while (inputs[i].valid()) {
                if (Less(max, inputs[i])) {
                    max = inputs[i];
                }
                inputs[i].Next();
            }
        }
        output.AppendOp(max);
        output.EndChunk();
        return Status::OK;
    }
};

}  // namespace ron

#endif
