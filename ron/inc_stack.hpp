#ifndef RON_INC_STACK_HPP
#define RON_INC_STACK_HPP

#include <cstdint>
#include <vector>

template <typename value_t>
class inc_stack {
   public:
    using len_t = uint32_t;

    struct span_t {
        value_t value;
        len_t size;

        span_t(value_t v) : value{v}, size{1} {}
    };

    using spans_t = std::vector<span_t>;

    class const_iterator {
        using spans_iter = typename spans_t::const_iterator;
        spans_iter i_;
        len_t off_;
        const_iterator(spans_iter i, len_t offset) : i_{i}, off_{offset} {}
        friend class inc_stack;

       public:
        inline void operator++() {
            ++off_;
            if (off_ == i_->size) {
                off_ = 0;
                ++i_;
            }
        }
        inline value_t operator*() { return i_->value + off_; }
        inline bool operator==(const const_iterator& b) const {
            return i_ == b.i_ && off_ == b.off_;
        }
        inline bool operator!=(const const_iterator& b) const {
            return i_ != b.i_ || off_ != b.off_;
        }
        void operator+=(len_t advance) {
            while (advance > 0) {
                if (i_->size <= advance) {
                    advance -= i_->size;
                    ++i_;
                } else {
                    i_ += advance;
                    advance = 0;
                }
            }
        }
        len_t skip_span(len_t max_len) {
            len_t s = i_->size;
            if (off_ + max_len < i_->size) {
                off_ += max_len;
                return max_len;
            } else {
                len_t rest = i_->size - off_;
                ++i_;
                off_ = 0;
                return rest;
            }
        }
    };

   private:
    spans_t spans_;
    len_t size_;

   public:
    inc_stack() : spans_{}, size_{0} {}

    inline len_t size() const { return size_; }
    inline len_t span_size() const { return (len_t)spans_.size(); }
    inline bool empty() const { return size_ == 0; }
    inline const value_t front() const { return spans_.front().value; }
    inline const value_t back() const {
        const span_t& b = spans_.back();
        return b.value + (b.size - 1);
    }
    inline const span_t& front_span() const { return spans_.front(); }
    inline const span_t& back_span() const { return spans_.back(); }

    inline const_iterator begin() const {
        return const_iterator{spans_.begin(), 0};
    }
    inline const_iterator end() const {
        return const_iterator{spans_.end(), 0};
    }

    void push_back(value_t i) {
        ++size_;
        if (spans_.empty()) {
            spans_.emplace_back(i);
        } else if (spans_.back().value + spans_.back().size == i) {
            spans_.back().size++;
        } else {
            spans_.emplace_back(i);
        }
    }

    void pop_back() {
        assert(!empty());
        --size_;
        if (spans_.back().size == 1) {
            spans_.pop_back();
        } else {
            spans_.back().size--;
        }
    }
};

#endif
