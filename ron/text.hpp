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
        static constexpr int SPEC_SIZE = 2;  // open RON

       public:
        explicit Cursor(const TextFrame& host)
            : frame_{host},
              op_{TERM::RAW},
              cs{0},
              off_{0},
              pos_{-1},
              prev_id_{} {
            Next();
        }
        const Op& op() const { return op_; }
        const TextFrame& frame() const { return frame_; }
        bool Next();
        inline bool valid() const { return cs != 0; }
        const std::string& data() const { return frame_.data(); }
        inline slice_t slice(frange_t range) const {
            return slice_t{data().data() + range.first, range.second};
        }

        int64_t parse_int(fsize_t idx);
        double parse_float(fsize_t idx);
        void parse(fsize_t idx) {
            switch (op().type(idx)) {
                case INT:
                    parse_int(idx);
                    break;
                case FLOAT:
                    parse_float(idx);
                    break;
                default:
                    break;
            }
        }
        std::string unescape(const slice_t& data) const;
        inline std::string parse_string(fsize_t idx) const {
            const Atom& atom = op().atom(idx);
            assert(atom.type() == STRING);
            return unescape(slice(atom.origin().range()));
        }
        inline Uuid parse_uuid(fsize_t idx) {
            assert(op_.atom(idx).type() == UUID);
            return op_.uuid(idx);
        }
    };

    class Builder {
        TERM term_;
        Uuid prev_;
        std::string data_;

        inline void Write(char c) { data_.push_back(c); }
        inline void Write(slice_t data) { data_.append(data.buf_, data.size_); }
        void WriteInt(int64_t value);
        void WriteFloat(double value);
        void WriteUuid(const Uuid& value);
        void WriteString(const std::string& value);

       public:
        Builder() : term_{RAW}, prev_{Uuid::ZERO}, data_{} {}

        // copy-as-strings
        void AppendOp(const Cursor& cur);

        // RON coding conversion (parsing, re-serialization)
        template <typename Cursor2>
        void AppendOp(const Cursor2& cur);

        const TextFrame frame() const { return TextFrame{data_}; }

        bool empty() const { return data_.empty(); }

        void escape(std::string& escaped, const slice_t& unescaped);

        //  B E A U T I F Y I N G   T E M P L A T E S

        // terminates the op
        void AppendAtoms() { Write(TERM_PUNCT[term_]); }

        template <typename... Ts>
        void AppendAtoms(int64_t value, Ts... args) {
            Write(' ');
            WriteInt(value);
            AppendAtoms(args...);
        }

        template <typename... Ts>
        void AppendAtoms(Uuid value, Ts... args) {
            Write(value.is_ambiguous() ? ' ' : ATOM_PUNCT[UUID]);
            WriteUuid(value);
            AppendAtoms(args...);
        }

        template <typename... Ts>
        void AppendAtoms(double value, Ts... args) {
            Write(' ');
            WriteFloat(value);
            AppendAtoms(args...);
        }

        template <typename... Ts>
        void AppendAtoms(const std::string& value, Ts... args) {
            Write(ATOM_PUNCT[STRING]);
            WriteString(value);
            Write(ATOM_PUNCT[STRING]);
            AppendAtoms(args...);
        }

        template <typename... Ts>
        void AppendNewOp(TERM term, const Uuid& id, const Uuid& ref,
                         Ts... args) {
            term_ = term;
            Write(SPEC_PUNCT[EVENT]);
            WriteUuid(id);
            Write(' ');
            Write(SPEC_PUNCT[REF]);
            WriteUuid(ref);
            AppendAtoms(args...);
        }

        template <typename Cur>
        void AppendAll(Cur& cur) {
            if (!cur.valid()) return;
            do {
                AppendOp(cur);
            } while (cur.Next());
        }

        template <typename Frame2>
        void AppendFrame(const Frame2& frame) {
            auto cur = frame.cursor();
            AppendAll(cur);
        }
    };

    Cursor cursor() const { return Cursor{*this}; }

    static constexpr char ESC = '\\';
};

}  // namespace ron

#endif
