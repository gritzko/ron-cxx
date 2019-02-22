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

   private:
    using spans_t = std::vector<span_t>;
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
