#ifndef CPP_MERGE_OPS_HPP
#define CPP_MERGE_OPS_HPP
#import <rocksdb/merge_operator.h>

namespace ron {

    //
    template<typename Frame>
    class ChainMergeOperator : public rocksdb::MergeOperator {

    public:
        typedef typename Frame::Batch Batch;
        typedef typename Frame::Builder Builder;
        typedef typename Frame::Cursor Cursor;


        virtual bool FullMergeV2(const MergeOperationInput& merge_in,
                                 MergeOperationOutput* merge_out) const {
            Builder out;
            if (merge_in.existing_value) {
                out.addAll(Cursor{merge_in.existing_value});
            }
            for(auto s : merge_in.operand_list) {
                out.addAll(Cursor{s});
            }
            swap(out.data_, merge_out->new_value);
            return true;
        }

        virtual bool PartialMergeMulti(const rocksdb::Slice& key,
                                       const std::deque<rocksdb::Slice>& operand_list,
                                       std::string* new_value, rocksdb::Logger* logger) const {
            Builder out;
            for(auto s : operand_list) {
                out.addAll(Cursor{s});
            }
            swap(out, *new_value);
            return true;
        }


    };


};

#endif //CPP_MERGE_OPS_HPP
