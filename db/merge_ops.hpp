#ifndef RON_MERGE_OPS_HPP
#define RON_MERGE_OPS_HPP

#include <rocksdb/merge_operator.h>
#include "db/replica.hpp"

namespace ron {

//
template <typename Frame>
class RDTMergeOperator : public rocksdb::MergeOperator {
    MasterRDT<Frame> reducer_;

   public:
    typedef typename Frame::Batch Batch;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;
    typedef typename Frame::Cursors Cursors;

    RDTMergeOperator() : reducer_{} {}

    Status MergeRDT(Builder& out, const Uuid& rdt, const Cursors& inputs) {
        return reducer_.Merge(out, rdt, inputs);
    }

    bool FullMergeV2(const MergeOperationInput& merge_in,
                     MergeOperationOutput* merge_out) const override {
        Builder out;
        Cursors inputs{};
        Key key{merge_in.key};
        RDT rdt = key.rdt();

        if (merge_in.existing_value) {
            inputs.push_back(Cursor{slice(merge_in.existing_value)});
            // ? read @rdt
        }
        for (auto s : merge_in.operand_list) {
            inputs.push_back(Cursor{slice(s)});
        }

        Status ok = reducer_.Merge(out, rdt, inputs);

        if (!ok) return false;
        swap(out, merge_out->new_value);
        return true;
    }

    bool PartialMergeMulti(const rocksdb::Slice& dbkey,
                           const std::deque<rocksdb::Slice>& operand_list,
                           std::string* new_value,
                           rocksdb::Logger* logger) const override {
        Key key{dbkey};
        RDT rdt = key.rdt();
        Builder out;
        Cursors inputs{};  // operand_list.size() + 1
        for (auto s : operand_list) {
            inputs.push_back(Cursor{slice(s)});
            // read rdt?
        }

        Status ok = reducer_.Merge(out, rdt, inputs);

        if (!ok) return false;
        swap(out, *new_value);
        return true;
    }

    const char* Name() const override { return "rdt"; }
};

};  // namespace ron

#endif  // RON_MERGE_OPS_HPP
