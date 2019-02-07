#include <utility>

#ifndef ron_open_text_hpp
#define ron_open_text_hpp
#include "op.hpp"
#include "status.hpp"

namespace ron {

class TextFrame {
    std::string data_;

   public:
    typedef std::vector<TextFrame> Batch;

    TextFrame() : data_{} {}
    explicit TextFrame(std::string data) : data_{std::move(data)} {}
    // explicit TextFrame(const std::string&& data) : data_{data} {}
    TextFrame& operator=(const TextFrame& orig) = default;

    const std::string& data() const { return data_; }

    class Cursor {
        /** Frame data; the cursor does not own the memory */
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
        inline bool is(fsize_t idx, const Uuid& id) const {
            return has(idx, UUID) && uuid(idx) == id;
        }
        const slice_t data() const { return data_; }
        inline slice_t slice(frange_t range) const {
            return data().slice(range);
        }
        inline const Uuid& id() const { return op_.id(); }
        inline const Uuid& ref() const { return op_.ref(); }
        inline fsize_t size() const { return op_.size(); }
        inline ATOM type(fsize_t idx) const {
            assert(size() > idx);
            return op_.type(idx);
        }
        inline TERM term() const { return op_.term(); }
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
        Atom atom(fsize_t idx) const {  // TODO inherit Op! skip boilerplate
            assert(size() > idx);
            return op_.atom(idx);
        }
    };

    struct Builder {
        TERM term_;
        Uuid prev_;
        /** Frame data (builder owns the memory) */
        std::string data_;

        inline void Write(char c) { data_.push_back(c); }
        inline void Write(slice_t data) { data_.append(data.buf_, data.size_); }
        void WriteInt(int64_t value);
        void WriteFloat(double value);
        void WriteUuid(const Uuid& value);
        void WriteString(const std::string& value);

        void escape(std::string& escaped, const slice_t& unescaped);

        // terminates the op
        void WriteAtoms() { Write(TERM_PUNCT[term_]); }

        template <typename... Ts>
        void WriteAtoms(int64_t value, Ts... args) {
            Write(' ');
            WriteInt(value);
            WriteAtoms(args...);
        }

        template <typename... Ts>
        void WriteAtoms(Uuid value, Ts... args) {
            Write(value.is_ambiguous() ? ' ' : ATOM_PUNCT[UUID]);
            WriteUuid(value);
            WriteAtoms(args...);
        }

        template <typename... Ts>
        void WriteAtoms(double value, Ts... args) {
            Write(' ');
            WriteFloat(value);
            WriteAtoms(args...);
        }

        template <typename... Ts>
        void WriteAtoms(const std::string& value, Ts... args) {
            Write(ATOM_PUNCT[STRING]);
            WriteString(value);
            Write(ATOM_PUNCT[STRING]);
            WriteAtoms(args...);
        }

        void WriteSpec(const Uuid& id, const Uuid& ref) {
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

        void WriteValues(const Cursor& cur);

        template <typename Cursor2>
        void WriteValues(const Cursor2& cur);

       public:
        Builder() : term_{RAW}, prev_{Uuid::NIL}, data_{} {}

        /** A shortcut method, avoids re-serialization of atoms. */
        void AppendOp(const Cursor& cur);

        /** RON coding conversion (parsing, re-serialization) */
        template <typename Cursor2>
        void AppendOp(const Cursor& cur);

        // template <typename Cursor2>
        void AppendAmendedOp(const Cursor& cur, TERM newterm, const Uuid& newid,
                             const Uuid& newref);

        const TextFrame frame() const { return TextFrame{data_}; }

        const std::string& data() const { return data_; }

        bool empty() const { return data_.empty(); }

        /** A convenience API method to add an op with any number of atoms. */
        template <typename... Ts>
        void AppendNewOp(TERM term, const Uuid& id, const Uuid& ref,
                         Ts... args) {
            term_ = term;
            WriteSpec(id, ref);
            Write(' ');
            WriteAtoms(args...);
            Write('\n');
        }

        /** A convenience method to add all ops from the cursor. */
        template <typename Cur>
        void AppendAll(Cur& cur) {
            if (!cur.valid()) return;
            do {
                AppendOp(cur);
            } while (cur.Next());
        }

        /** A convenience method to add all ops from the frame. */
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

    Status Split(std::vector<TextFrame>& to);
};

}  // namespace ron

namespace std {

inline void swap(ron::TextFrame::Builder& builder, std::string& str) {
    swap(builder.data_, str);
}

inline void swap(ron::TextFrame& f, std::string& str) { f.swap(str); }

}  // namespace std

#endif
