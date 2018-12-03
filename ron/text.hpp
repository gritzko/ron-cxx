#ifndef ron_open_text_hpp
#define ron_open_text_hpp
#include "op.hpp"

namespace ron {

class TextFrame {

    const std::string data_;

public:

    typedef std::vector<TextFrame> Batch;

    TextFrame() : data_{} {}
    explicit TextFrame(const std::string& data) : data_{data} {}
    explicit TextFrame(const std::string&& data) : data_{data} {}

    const std::string& data() const { return data_; }

    class Cursor {
        const TextFrame& frame_;
        Op op_;
        int pos_;
        int off_;
        int cs;
        Uuid prev_id_;

        static constexpr int RON_FULL_STOP = 255;
        static constexpr int SPEC_SIZE = 2; // open RON

        typedef const char* iter;

    public:
        explicit Cursor(const TextFrame& host) :
            frame_{host}, op_{RON::TEXT_OPEN, TERM::RAW}, 
            cs{0}, off_{0}, pos_{-1}, prev_id_{}
            {
                Next();
            }
        const Op& op() const { return op_; }
        const TextFrame& frame() const { return frame_; }
        bool Next();
        inline bool valid() const { return cs!=0; }
        const std::string& data() const { return frame_.data(); }

        int64_t integer(fsize_t idx);
        double number(fsize_t idx);
        std::string string(fsize_t idx) const;
        Uuid uuid(fsize_t idx);
    };

    class Builder {
        Op op_;
        std::string data_;

        void WriteInt(int64_t value);
        void WriteFloat(double value);
        void WriteUuid(const Uuid& value);
        void WriteString(const std::string& value);

    public:

        Builder() : data_{}, op_{RON::TEXT_OPEN, TERM::RAW} {}

        void AddOp(const Op& op, const std::string& back_buf);

        void AddAll(Cursor& cur) {
            // TODO
        }
        void AddAll(const TextFrame& frame) {
            Cursor cur = frame.cursor();
            AddAll(cur);
        }

        const TextFrame frame() const { return TextFrame{data_}; }

        bool empty() const { return data_.empty(); }

    };

    Cursor cursor() const {
        return Cursor{*this};
    }

    static constexpr char ESC = '\\';

    static void unescape (std::string& to, const char* buf, fsize_t size);

    static void escape (std::string& to, const char* buf, fsize_t size);

    static inline void escape (std::string& to, const std::string& str) {
        escape(to, str.data(), (fsize_t)str.size());
    }

    static inline void unescape (std::string& to, const std::string& str) {
        unescape(to, str.data(), (fsize_t)str.size());
    }

};

}

#endif
