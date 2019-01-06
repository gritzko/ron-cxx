#ifndef ron_open_text_hpp
#define ron_open_text_hpp
#include "op.hpp"

namespace ron {

class TextFrame {
    std::string data_;

   public:
    typedef std::vector<TextFrame> Batch;

    TextFrame() : data_{} {}
    explicit TextFrame(const std::string& data) : data_{data} {}
    explicit TextFrame(const std::string&& data) : data_{data} {}
    TextFrame& operator=(const TextFrame& orig) = default;

    const std::string& data() const { return data_; }

    class Cursor {
        // the cursor does not own the memory
        slice_t data_;
        Op op_;
        int pos_;
        int off_;
        int cs;
        Uuid prev_id_;

        static constexpr int RON_FULL_STOP = 255;
        static constexpr int SPEC_SIZE = 2;  // open RON

        static int64_t parse_int(slice_t data);
        static double parse_float(slice_t data);

       public:
        explicit Cursor(const slice_t& data)
            : data_{data},
              op_{TERM::RAW},
              cs{0},
              off_{0},
              pos_{-1},
              prev_id_{} {
            Next();
        }
        explicit Cursor(const std::string& str) : Cursor{slice_t{str}} {}
        explicit Cursor(const TextFrame& host) : Cursor{host.data_} {}
        const Op& op() const { return op_; }
        bool Next();
        inline bool valid() const { return cs != 0; }
        inline bool has(fsize_t idx, ATOM atype) const {
            return size() > idx && type(idx) == atype;
        }
        const slice_t data() const { return data_; }
        inline slice_t slice(frange_t range) const {
            return data().slice(range);
        }
        inline const Uuid& id() const { return op_.id(); }
        inline const Uuid& ref() const { return op_.ref(); }
        inline fsize_t size() const { return op_.size(); }
        inline ATOM type(fsize_t idx) const { return op_.type(idx); }
        static std::string unescape(const slice_t& data);
        std::string string(fsize_t idx) const {
            assert(type(idx) == STRING);
            // FIXME check metrics
            slice_t esc = data_.slice(op_.atom(idx).origin().range());
            return unescape(esc);
        }
        int64_t integer(fsize_t idx) const {
            assert(type(idx) == INT);
            return int64_t(op_.atom(idx).value());
        }
        double number(fsize_t idx) const {
            assert(type(idx) == FLOAT);
            return double(op_.atom(idx).value());
        }
        Uuid uuid(fsize_t idx) const {
            assert(type(idx) == UUID);
            return op_.uuid(idx);
        }
    };

    struct Builder {
        TERM term_;
        Uuid prev_;
        std::string data_;

        inline void Write(char c) { data_.push_back(c); }
        inline void Write(slice_t data) { data_.append(data.buf_, data.size_); }
        void WriteInt(int64_t value);
        void WriteFloat(double value);
        void WriteUuid(const Uuid& value);
        void WriteString(const std::string& value);

        void escape(std::string& escaped, const slice_t& unescaped);

       public:
        Builder() : term_{RAW}, prev_{Uuid::NIL}, data_{} {}

        // copy-as-strings
        void AppendOp(const Cursor& cur);

        // template <typename Cursor2>
        void AppendValues(const Cursor& cur);

        // RON coding conversion (parsing, re-serialization)
        // template <typename Cursor2>
        // void AppendOp(const Cursor& cur);

        // template <typename Cursor2>
        void AppendAmendedOp(const Cursor& cur, TERM newterm, const Uuid& newid,
                             const Uuid& newref);

        const TextFrame frame() const { return TextFrame{data_}; }

        const std::string& data() const { return data_; }

        bool empty() const { return data_.empty(); }

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

        void AppendSpec(const Uuid& id, const Uuid& ref) {
            bool seq_id = id == prev_.inc();
            if (!seq_id) {
                Write(SPEC_PUNCT[EVENT]);
                WriteUuid(id);
            }
            if (ref != prev_) {
                if (!seq_id) Write(' ');
                Write(SPEC_PUNCT[REF]);
                WriteUuid(ref);
            }
            prev_ = id;
        }

        template <typename... Ts>
        void AppendNewOp(TERM term, const Uuid& id, const Uuid& ref,
                         Ts... args) {
            term_ = term;
            AppendSpec(id, ref);
            Write(' ');
            AppendAtoms(args...);
            Write('\n');
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

    typedef std::vector<Cursor> Cursors;

    inline void swap(std::string& str) { std::swap(data_, str); }
};

}  // namespace ron

namespace std {

inline void swap(ron::TextFrame::Builder& builder, std::string& str) {
    swap(builder.data_, str);
}

inline void swap(ron::TextFrame& f, std::string& str) { f.swap(str); }

}  // namespace std

#endif
