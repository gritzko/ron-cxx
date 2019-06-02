#ifndef ron_open_text_hpp
#define ron_open_text_hpp
#include <iostream>
#include <utility>

#include "encdec.hpp"
#include "slice.hpp"
#include "status.hpp"
#include "uuid.hpp"

namespace ron {

class TextFrame {
    String data_;

   public:
    TextFrame() : data_{} {}
    explicit TextFrame(String data) : data_{std::move(data)} {}
    explicit TextFrame(Slice data) : TextFrame{data.str()} {}

    void operator=(const TextFrame& orig) { data_ = orig.data_; }

    const String& data() const { return data_; }

    //    KILLL THIS!!!
    static String unescape(const Slice& data);
    static inline String string(Slice data, const Atom& a) {
        Slice esc = data.slice(a.origin.range());
        return unescape(esc);
    }
    //    END OF KILL

    //  P A R S I N G

    class Cursor {
        /** Frame data; the cursor does not own the memory */
        Slice data_;           // 16 remaining data, base for ranges
        Atoms op_;             // 24
        TERM term_;            // 1
        uint8_t ragel_state_;  // 1
        fsize_t line_;         // 4
        fsize_t span_size_;    // 4
                               // 50 bytes total

        static constexpr int RON_FULL_STOP = 255;
        static constexpr int SPEC_SIZE = 2;  // open RON

        Result ParseFloat(Atom& atom) const;
        Result ParseInteger(Atom& atom) const;
        inline Result ParseAtom(Atom& a) const {
            switch (a.type()) {
                case INT:
                    return ParseInteger(a);
                case FLOAT:
                    return ParseFloat(a);
                case STRING:
                case UUID:
                    return OK;
            }
        }
        static bool int_too_big(const Slice& data);
        static inline bool word_too_big(const Slice data) {
            return data.size() > Word::MAX_BASE64_SIZE;
        }

       public:
        explicit Cursor(const Slice data)
            : data_{data},
              op_{TERM::RAW},
              ragel_state_{0},
              line_{1},
              span_size_{0} {}

        explicit Cursor(const String& str) : Cursor{Slice{str}} {}
        explicit Cursor(const TextFrame& host) : Cursor{host.data_} {}
        Cursor(const Cursor& b) = default;

        /** Returns the raw atom vector (values likely unparsed, use atom(i) for
         * parsed). */
        const Atoms& op() const { return op_; }

        /** Parses the next op. Populates the atom vector (see op(), atom(i)).
         */
        Status Next();

        /** Parses a codepoint (escaped UTF8), saves to atom.value.cp,
         *  consumes the origin range, decreases atom.value.cp_size
         *  (full parser). */
        Result NextCodepoint(Atom& c) const;

        /** Returns whether the last op was parsed successfully.  */
        inline bool valid() const { return ragel_state_ != 0; }

        /** The remaining unparsd data. Use data().reset() for the full buffer.
         */
        Slice& data() { return data_; }

        inline Slice data(Atom a) const {
            assert(a.safe_origin().as_range.valid());
            return Slice{data_.data(), a.safe_origin().as_range};
        }

        /** The current op's id. */
        inline const Uuid id() const {
            return static_cast<Uuid>(atom(OP_ID_IDX));
        }

        /** The current op's reference id. */
        inline const Uuid ref() const {
            return static_cast<Uuid>(atom(OP_REF_IDX));
        }

        /** The op's term is basically a form of punctuation in a frame.
            See enum TERM. */
        inline TERM term() const { return term_; }

        /** Returns an atom (0 id, 1 ref, 2... are values). Values are parsed.
         */
        inline Atom atom(fsize_t idx = 2) const {
            assert(op().size() > idx);
            Atom ret = op_[idx];
            ParseAtom(ret);
            return ret;
        }

        using Comparator = bool (*)(const Cursor& a, const Cursor& b);
    };

    //  S E R I A L I Z A T I O N

    class Builder {
        Uuid prev_id_;
        Atom prev_2_;
        fsize_t span_size_;

        /** Frame data (builder owns the memory) */
        String data_;

        inline void Write(char c) { data_.push_back(c); }
        inline void WriteCodepoint(Codepoint cp) { utf8esc_append(data_, cp); }
        inline void Write(Slice data) {
            data_.append((String::value_type*)data.begin(), data.size());
        }
        void WriteInt(int64_t value);
        void WriteFloat(double value);
        void WriteUuid(Uuid value);
        void WriteString(const String& value);

        void escape(String& escaped, const Slice& unescaped);

        inline void WriteTerm(TERM term = REDUCED) {
            if (span_size_) {
                if (span_size_ > 1) {
                    Write('(');
                    WriteInt(span_size_);
                    Write(')');
                }
                Write(TERM_PUNCT[term]);
                Write(NL);
                span_size_ = 0;
            }
        }

        void WriteSpec(Uuid id, Uuid ref) {
            WriteTerm();
            span_size_ = 1;
            bool seq_id = id == prev_id_.inc();
            if (!seq_id) {
                Write(SPEC_PUNCT[EVENT]);
                WriteUuid(id);
            }
            if (ref != prev_id_) {
                if (!seq_id) Write(' ');
                Write(SPEC_PUNCT[REF]);
                WriteUuid(ref);
            }
            prev_id_ = id;
        }

        Result WriteValues(const Cursor& cur);

        /** The op must be *parsed*. */
        template <typename Cursor2>
        Result WriteValues(const Cursor2& cur) {
            const Atoms& op = cur.op();
            for (fsize_t i = 2; i < op.size(); i++) {
                Atom a = op[i];
                Write(' ');
                switch (a.type()) {
                    case INT:
                        WriteInt(a.value.as_integer);
                        break;
                    case UUID:
                        if (A2U(a).is_ambiguous()) Write(ATOM_PUNCT[UUID]);
                        WriteUuid(A2U(a));
                        break;
                    case STRING:
                        Write(ATOM_PUNCT[STRING]);
                        while (a.value.cp) {
                            WriteCodepoint(a.value.cp);
                            cur.NextCodepoint(a);
                        }
                        Write(ATOM_PUNCT[STRING]);
                        break;
                    case FLOAT:
                        WriteFloat(a.value.as_float);
                        break;
                }
            }
            if (op.size() == 3 && op[2].type() != STRING) {
                prev_2_ = op[2];
            } else {
                prev_2_ = Uuid::FATAL;
            }  // FIXME 2
            return OK;
        }

        template <class Cursor2>
        inline bool same_span(const Cursor2& cur) {
            const Atoms& op = cur.op();
            return span_size_ > 0 && op.size() == 3 && op[2] == prev_2_;
        }

        template <class Cursor2>
        inline Result ExtendSpan(const Cursor2& cur) {
            ++span_size_;
            // maybe append a char
            return OK;
        }

       public:
        Builder()
            : prev_id_{Uuid::NIL},
              prev_2_{Uuid::FATAL},
              span_size_{0},
              data_{} {}

        //  A P I   M E T H O D S

        template <class Cursor2>
        Result AppendOp(const Cursor2& cur) {
            if (same_span(cur)) {
                return ExtendSpan(cur);
            } else {
                WriteTerm();
                WriteSpec(cur.id(), cur.ref());
                return WriteValues(cur);
            }
        }

        /** A shortcut method, avoids re-serialization of atoms. */
        Result AppendOp(const Cursor& cur) {
            if (same_span(cur)) {
                return ExtendSpan(cur);
            } else {
                WriteTerm();
                const Atoms& op = cur.op();
                WriteSpec(A2U(op[OP_ID_IDX]), A2U(op[OP_REF_IDX]));
                return WriteValues(cur);
            }
        }

        /**  */
        inline Result EndChunk(TERM term = RAW) {
            assert(term != REDUCED);
            // empty chunks?
            WriteTerm(term);
            return OK;
        }

        Result EndFrame() {
            Write(FRAME_TERM);
            return OK;
        }

        Result Release(String& to) {
            if (span_size_) {
                EndChunk();
            }
            std::swap(data_, to);
            data_.clear();
            return OK;
        }

        Result Release(TextFrame& to) { return Release(to.data_); }

        TextFrame Release() {
            TextFrame ret;
            Release(ret);
            return ret;
        }

        const String& data() const { return data_; }

        bool empty() const { return data_.empty(); }
    };

    Cursor cursor() const { return Cursor{*this}; }

    static constexpr char ESC = '\\';

    using Cursors = std::vector<Cursor>;

    inline void swap(String& str) { std::swap(data_, str); }

    Status Split(std::vector<TextFrame>& to);

    inline void Clear() { data_.clear(); }

    inline bool empty() const { return data_.empty(); }

    // V A L U E  D E C O D E R S

    // std::u16string utf16string(Atom range) const;

    static Codepoint decode_hex_cp(Slice data);

    inline static char decode_esc(char esc);
};

inline char TextFrame::decode_esc(char esc) {
    switch (esc) {
        case 'a':
            return '\a';
        case 'b':
            return '\b';
        case 'f':
            return '\f';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'v':
            return '\v';
        default:
            return esc;
    }
}
}  // namespace ron

namespace std {

inline void swap(ron::TextFrame::Builder& builder, ron::String& str) {
    builder.Release(str);
}

inline void swap(ron::TextFrame& f, ron::String& str) { f.swap(str); }

}  // namespace std

#endif
