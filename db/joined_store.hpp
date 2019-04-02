#include "../rdt/rdt.hpp"
#include "../ron/ron.hpp"
#include "key.hpp"

namespace ron {

template <class StoreA, class StoreB>
class JoinedStore {
    StoreA& a_;
    StoreB& b_;

   public:
    using Frame = typename StoreA::Frame;
    using Record = typename StoreA::Record;
    using Records = typename StoreA::Records;
    using Frames = std::vector<Frame>;
    using Cursor = typename Frame::Cursor;
    using Cursors = typename Frame::Cursors;
    using Builder = typename Frame::Builder;

    JoinedStore(StoreA& a, StoreB& b) : a_{a}, b_{b} {}

    Status Write(Key key, const Frame& data) { return a_.Write(key, data); }

    Status Read(Key key, Frame& into) {
        Frame a, b;
        Status ok;
        ok = a_.Read(key, a);
        if (!ok) {
            return ok;
        }
        ok = b_.Read(key, b);
        if (!ok) {
            return ok;
        }
        if (a.empty()) {
            std::swap(b, into);
        } else if (b.empty()) {
            std::swap(b, into);
        } else {
            ok = MergeFrames(into, Frames{a, b});
        }
        return ok;
    }

    Status Write(const Records& batch) { return a_.Write(batch); }

    class Iterator {
        typename StoreA::Iterator ai_;
        typename StoreB::Iterator bi_;
        Key at_;
        Frame merged_;

        inline void pick() {
            merged_.Clear();
            at_ = std::min(ai_.key(), bi_.key());
        }

       public:
        Iterator(JoinedStore& host) : ai_{host.a_}, bi_{host.b_} {}

        Status Next() {
            if (ai_.key() == at_) {
                Status ok = ai_.Next();
                if (!ok) {
                    return ok;
                }
            }
            if (bi_.key() == at_) {
                Status ok = bi_.Next();
                if (!ok) {
                    return ok;
                }
            }
            pick();
            return Status::OK;
        }

        Status SeekTo(Key key, bool prev = false) {
            Status ok;
            ok = ai_.SeekTo(key, prev);
            if (!ok) {
                return ok;
            }
            ok = bi_.SeekTo(key, prev);
            if (!ok) {
                return ok;
            }
            pick();
            return Status::OK;
        }

        Cursor value() {
            if (ai_.key() != at_) {
                return bi_.value();
            }
            if (bi_.key() != at_) {
                return ai_.value();
            }
            // have to merge then
            Cursors inputs;
            inputs.push_back(ai_.value());
            inputs.push_back(bi_.value());
            Status ok = MergeCursors<Frame>(merged_, inputs);
            // errors?!
            return Cursor{merged_};
        }

        inline Key key() const { return at_; }
    };

    Status Close() {
        Status oka = a_.Close();
        Status okb = b_.Close();
        return oka ? okb : oka;
    }
};

}  // namespace ron
