#ifndef ron_open_text_hpp
#define ron_open_text_hpp
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
                    if (a.value.cp != 0) {
                        a = Atom{a.value.cp, STRING_FLAGS};
                    }
                    return OK;
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
            data_.append(reinterpret_cast<StringCharRef>(data.begin()),
                         data.size());
        }
        void WriteInt(int64_t value);
        void WriteFloat(double value);
        void WriteUuid(Uuid value);
        void WriteString(const String& value);

        void escape(String& escaped, const Slice& unescaped);

        inline void WriteTerm(TERM term = REDUCED) {
            if (!span_size_) {
                return;
            }
            if (span_size_ > 1) {
                if (prev_2_.type() != STRING) {
                    Write('(');
                    WriteInt(span_size_);
                } else {
                    Write(ATOM_PUNCT[STRING]);
                }
                Write(')');
            }
            Write(TERM_PUNCT[term]);
            Write(NL);
            span_size_ = 0;
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
            prev_2_ = span_signature(cur);
            return OK;
        }

        template <typename Cursor2>
        static inline Atom span_signature(const Cursor2& cur) {
            const Atoms& op = cur.op();
            if (op.size() != 3) {
                return Uuid::NIL;
            }
            Atom atom = op[2];
            switch (atom.type()) {
                case INT:
                case FLOAT:
                    return static_cast<Atom>(Uuid::NIL);
                case UUID:
                    return atom;
                case STRING:
                    Atom a = cur.atom(2);  // parsed
                    return StringSize(a) == 1 ? Atom{0, STRING_FLAGS}
                                              : static_cast<Atom>(Uuid::NIL);
            }
        }

        template <class Cursor2>
        inline bool is_same_span(const Cursor2& cur) const {
            if (cur.ref() != prev_id_) {
                return false;
            }
            if (cur.id() != prev_id_.inc()) {
                return false;
            }
            return prev_2_ != Uuid::NIL && span_signature(cur) == prev_2_;
        }

        template <class Cursor2>
        inline Result ExtendSpan(const Cursor2& cur) {
            if (prev_2_.type() == STRING) {
                Atom a = cur.op()[2];
                if (span_size_ == 1) {
                    char ret[8];
                    fsize_t retc{0};
                    data_.pop_back();
                    while (data_.back() != ATOM_PUNCT[STRING]) {
                        ret[retc++] = data_.back();
                        data_.pop_back();
                    }
                    data_.pop_back();
                    Write('(');
                    Write(ATOM_PUNCT[STRING]);
                    for (fsize_t i = 0; i < retc; ++i) {
                        data_.push_back(ret[i]);
                    }
                }
                if (!a.value.cp) {
                    cur.NextCodepoint(a);
                }
                WriteCodepoint(a.value.cp);
            }
            ++span_size_;
            ++prev_id_;
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
            if (is_same_span(cur)) {
                return ExtendSpan(cur);
            } else {
                WriteTerm();
                WriteSpec(cur.id(), cur.ref());
                return WriteValues(cur);
            }
        }

        /** A shortcut method, avoids re-serialization of atoms. */
        Result AppendOp(const Cursor& cur) {
            if (is_same_span(cur)) {
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
