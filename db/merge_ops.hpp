#ifndef RON_MERGE_OPS_HPP
#define RON_MERGE_OPS_HPP

#include <rocksdb/merge_operator.h>
#include "db/replica.hpp"

namespace ron {

//
template <typename Frame>
class ChainMergeOperator : public rocksdb::MergeOperator {
   public:
    typedef typename Frame::Batch Batch;
    typedef typename Frame::Builder Builder;
    typedef typename Frame::Cursor Cursor;

    bool FullMergeV2(const MergeOperationInput& merge_in,
                     MergeOperationOutput* merge_out) const override {
        Builder out;
        if (merge_in.existing_value) {
            Cursor ex{slice(merge_in.existing_value)};
            out.AppendAll(ex);
        }
        for (auto s : merge_in.operand_list) {
            Cursor nxt{slice(s)};
            out.AppendAll(nxt);
        }
        swap(out, merge_out->new_value);
        return true;
    }

    bool PartialMergeMulti(const rocksdb::Slice& key,
                           const std::deque<rocksdb::Slice>& operand_list,
                           std::string* new_value,
                           rocksdb::Logger* logger) const override {
        Builder out;
        for (auto s : operand_list) {
            Cursor nxt{slice(s)};
            out.AppendAll(nxt);
        }
        swap(out, *new_value);
        return true;
    }

    const char* Name() const override { return "chain"; }
};

};  // namespace ron

#endif  // RON_MERGE_OPS_HPP
