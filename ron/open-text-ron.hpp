#ifndef ron_text_hpp
#define ron_text_hpp
#include "op.hpp"

namespace ron {

class OpenTextFrame {

    const std::string data_;

public:

    typedef std::vector<OpenTextFrame> Batch;

    OpenTextFrame() : data_{} {}
    explicit OpenTextFrame(const std::string& data) : data_{data} {}
    explicit OpenTextFrame(const std::string&& data) : data_{data} {}

    const std::string& data() const { return data_; }

    class Cursor {
        const OpenTextFrame& frame_;
        Op op_;
        int pos_;
        int off_;
        int cs;

        static constexpr int RON_FULL_STOP = 255;
        static constexpr int SPEC_SIZE = 2; // open RON

        typedef std::string::const_iterator iter;

        bool _parse_int (iter from, iter till) ;
        bool _parse_float (iter from, iter till) ;
        bool _parse_uuid (iter from, iter version_at, iter till) ;
        bool _parse_string (iter from, iter till) ;
        bool _set_term (char term) ; 
        bool _default_spec () ;

    public:
        explicit Cursor(const OpenTextFrame& host) :
            frame_{host}, op_{RON::TEXT_OPEN, TERM::RAW}, 
            cs{0}, off_{0}, pos_{-1} 
            {
                Next();
            }
        const Op& op() const { return op_; }
        const OpenTextFrame& frame() const { return frame_; }
        bool Next();
        const std::string& data() const { return frame_.data(); }
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

        void AddAll(Cursor& cur, const std::string& back_buf);

        const OpenTextFrame frame() const { return OpenTextFrame{data_}; }

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
