#include <utility>

#ifndef ron_open_text_hpp
#define ron_open_text_hpp
#include "op.hpp"
#include "slice.hpp"
#include "status.hpp"

namespace ron {

class TextFrame {
    String data_;

   public:
    typedef std::vector<TextFrame> Batch;

    TextFrame() : data_{} {}
    explicit TextFrame(String data) : data_{std::move(data)} {}
    explicit TextFrame(Slice data)
        : data_{(const char*)data.begin(), data.size()} {}

    void operator=(const TextFrame& orig) { data_ = orig.data_; }

    const String& data() const { return data_; }

    /** Primary value parser: STRINGs.
     *  Puts the codepoint to atom.value().AsCodepoint();
     *  advances the range start at atom.origin().AsSize(LEAST_SIGNIFICANT) */
    static inline bool ParseCodepoint(Atom& atom, Slice data) {
        return ParseEscapedUtf8Codepoint(
            atom.value().codepoint_[LEAST_SIGNIFICANT], atom.origin().range_,
            data.begin());
    }

    /** Primary value parser: UUIDs */
    static inline bool ParseUuid(Atom& atom, Slice data) { return true; }

    /** Primary value parser: FLOATs;
     *  puts the value to atom.value().AsFloat() */
    static inline bool ParseNumber(Atom& atom, Slice data);

    /** Primary value parser: INTs */
    static inline bool ParseInteger(Atom& atom, Slice data);

    static String ParseToUtf8(Atom& atom, Slice data) {
        String ret;
        /*Codepoint cp;
        while (!atom.origin().range_.empty() && ParseEscapedUtf8Codepoint(cp,
        atom, data)) { push_back_utf8(ret, cp);
        }*/
        return ret;
        // empty:  from==(till&FSIZE_BITS)   ++from   =>  & is optimized
    }

    static Codepoints ParseToCodepoints(Atom& atom, Slice data) {
        Codepoints ret;
        Codepoint& cp = atom.value().AsCodepoint();
        while (ParseCodepoint(atom, data)) {
            ret.push_back(cp);
        }
        return ret;
    }

    /** Parses a codepoint (escaped UTF8), saves to cp, consumes the range. */
    static inline bool ParseEscapedUtf8Codepoint(Codepoint& cp, Range& range,
                                                 CharRef buf) {
        // shortcut ASCII
        // dive for complex things
        return false;
    }

    //    KILLL THIS!!!
    static String unescape(const Slice& data);
    static inline String string(Slice data, const Atom& a) {
        Slice esc = data.slice(a.origin().range());
        return unescape(esc);
    }
    static inline int64_t integer(Slice data, const Atom& a) {
        return static_cast<int64_t>(a.value());
    }
    static inline double number(Slice data, const Atom& a) {
        return static_cast<double>(a.value());
    }
    inline String string(const Atom& a) { return string(data_, a); }
    inline int64_t integer(const Atom& a) { return integer(data_, a); }
    inline double number(const Atom& a) { return number(data_, a); }
    //    END OF KILL

    //  P A R S I N G

    class Cursor {
        /** Frame data; the cursor does not own the memory */
        Slice data_;
        Op op_;
        int pos_;
        fsize_t at_;
        fsize_t off_;
        int cs;
        Uuid prev_id_;
        fsize_t line_;

        static constexpr int RON_FULL_STOP = 255;
        static constexpr int SPEC_SIZE = 2;  // open RON

        static int64_t parse_int(Slice data);
        static double parse_float(Slice data);

       public:
        explicit Cursor(const Slice data, bool advance = true)
            : data_{data},
              op_{TERM::RAW},
              pos_{-1},
              at_{0},
              off_{0},
              cs{0},
              prev_id_{},
              line_{1} {
            if (advance) {
                Next();
            }
        }
        explicit Cursor(const String& str) : Cursor{Slice{str}} {}
        explicit Cursor(const TextFrame& host, bool advance = true)
            : Cursor{host.data_, advance} {}
        Cursor(const Cursor& b) = default;
        const Op& op() const { return op_; }
        Status Next();
        void Trim(const Cursor& b) { data_.Resize(b.at_); }
        Status SkipChain() {
            Uuid i;
            Status ok;
            do {
                i = id();
                ok = Next();
            } while (ok && ref() == i);
            return ok;
        }
        inline bool valid() const { return cs != 0; }
        inline bool has(fsize_t idx, ATOM atype) const {
            return size() > idx && type(idx) == atype;
        }
        inline bool is(fsize_t idx, const Uuid& id) const {
            return has(idx, UUID) && uuid(idx) == id;
        }
        const Slice data() const { return data_; }
        const Slice at_data() const {
            return data_.slice(Range::FroTo(at_, off_));
        }
        inline Slice slice(Range range) const { return data().slice(range); }
        inline const Uuid& id() const { return op_.id(); }
        inline const Uuid& ref() const { return op_.ref(); }
        inline fsize_t size() const { return op_.size(); }
        inline ATOM type(fsize_t idx) const {
            assert(size() > idx);
            return op_.type(idx);
        }
        inline TERM term() const { return op_.term(); }
        String string(fsize_t idx) const {
            assert(type(idx) == STRING);
            // FIXME check metrics
            return TextFrame::string(data_, atom(idx));
        }
        inline Slice string_slice(fsize_t idx) const {
            return data_.slice(atom(idx).origin().range());
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
        static bool int_too_big(const Slice& data);
        static inline bool word_too_big(const Slice data) {
            return data.size() > Word::MAX_BASE64_SIZE;
        }
    };

    //  S E R I A L I Z A T I O N

    class Builder {
        Uuid prev_;
        bool unterm_;
        /** Frame data (builder owns the memory) */
        String data_;

        inline void Write(char c) { data_.push_back(c); }
        inline void Write(Slice data) {
            data_.append((String::value_type*)data.begin(), data.size());
        }
        void WriteInt(int64_t value);
        void WriteFloat(double value);
        void WriteUuid(const Uuid value);
        void WriteString(const String& value);

        void escape(String& escaped, const Slice& unescaped);

        // terminates the op
        void WriteAtoms() {}

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
        void WriteAtoms(const String& value, Ts... args) {
            Write(ATOM_PUNCT[STRING]);
            WriteString(value);
            Write(ATOM_PUNCT[STRING]);
            WriteAtoms(args...);
        }

        inline void WriteTerm(TERM term = REDUCED) {
            if (unterm_) {
                Write(TERM_PUNCT[term]);
                Write(NL);
                unterm_ = false;
            }
        }

        void WriteSpec(Uuid id, Uuid ref) {
            WriteTerm();
            unterm_ = true;
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
        Builder() : prev_{Uuid::NIL}, unterm_{false}, data_{} {}

        /** A shortcut method, avoids re-serialization of atoms. */
        void AppendOp(const Cursor& cur) {
            const Op& op = cur.op();
            WriteSpec(op.id(), op.ref());
            WriteValues(cur);
        }

        /** RON coding conversion (parsing, re-serialization) */
        template <typename Cursor2>
        void AppendOp(const Cursor& cur);

        // template <typename Cursor2>
        void AppendAmendedOp(const Cursor& cur, TERM newterm, const Uuid& newid,
                             const Uuid& newref);

        /**  */
        inline void EndChunk(TERM term = RAW) {
            assert(term != REDUCED);
            unterm_ = true;  // empty chunks are OK
            WriteTerm(term);
        }

        void Release(String& to) {
            if (unterm_) {
                EndChunk();
            }
            std::swap(data_, to);
            data_.clear();
        }

        void Release(TextFrame& to) { Release(to.data_); }

        TextFrame Release() {
            TextFrame ret;
            Release(ret);
            return ret;
        }

        const String& data() const { return data_; }

        bool empty() const { return data_.empty(); }

        /** A convenience API method to add an op with any number of atoms. */
        template <typename... Ts>
        void AppendNewOp(Uuid id, Uuid ref, Ts... args) {
            WriteSpec(id, ref);
            Write(' ');
            WriteAtoms(args...);
        }

        /** A convenience method to add all ops from the cursor. */
        template <typename Cur>
        void AppendAll(Cur& cur) {
            if (!cur.valid()) {
                return;
            }
            do {
                AppendOp(cur);
                WriteTerm(cur.term());
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

    using Cursors = std::vector<Cursor>;

    inline void swap(String& str) { std::swap(data_, str); }

    Status Split(std::vector<TextFrame>& to);

    inline void Clear() { data_.clear(); }

    inline bool empty() const { return data_.empty(); }

    // V A L U E  D E C O D E R S

    // std::u16string utf16string(Atom range) const;

    static Codepoint decode_hex_cp(Slice data);

    inline static char decode_esc(char esc);

    struct StringIterator {
        Slice data_;
        Codepoint cp_;

       public:
        StringIterator(Slice str) : data_{str}, cp_{0} { Next(); }
        inline Codepoint operator*() const { return cp_; }
        bool Next();
        inline void operator++() { Next(); }
        inline bool valid() const { return cp_ != 0; }
        inline operator bool() const { return valid(); }
    };
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
