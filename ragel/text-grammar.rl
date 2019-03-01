%%{

    machine TEXT_FRAME;
    alphtype unsigned char;

    include UUID "./uuid-grammar.rl";
    include UTF8 "./utf8-grammar.rl";

    action end_id {
        op_.SetId(Uuid{variety, value, version, origin});
    }
    action end_ref {
        op_.SetRef(Uuid{variety, value, version, origin});
    }
    action begin_int { intb.begin(p); }
    action end_int {
        intb.end(p);
        if (intb.size()>=19 && int_too_big(intb)) { cs = 0; fbreak; }
        op_.AddAtom(Atom::Integer(parse_int(intb), body.range_of(intb))); 
        lastintb = (iterator)intb.buf_;
    }
    action begin_string { strb.begin(p); }
    action end_string { 
        strb.end(p);
        op_.AddAtom(Atom::String(body.range_of(strb))); 
    }
    action begin_float { floatb.begin(p); }
    action end_float { 
        floatb.end(p);
        if (floatb.size() > 24) { cs = 0; fbreak; }
        op_.AddAtom(Atom::Float(parse_float(floatb), body.range_of(floatb))); 
    }
    action end_quoted_uuid {
        if (word_too_big(value) || word_too_big(origin)) { cs = 0; fbreak; }
        op_.AddAtom(Uuid{variety, value, version, origin}); 
    }
    action end_bare_uuid { 
        if (!intb.same(uuidb)) {
            if (word_too_big(value) || word_too_big(origin)) { cs = 0; fbreak; }
            op_.AddAtom(Uuid{variety, value, version, origin}); 
        }
    }
    action begin_span {}
    action end_span {}
    action op_term { 
        term = fc; 
        pos_++; 
        if (p<pe-1) fbreak;
    }
    action newline {
        if (fc=='\n') {
            line++;
            lineb = p;
        }
    }

    WS = [ \r\n\t] @newline;

    # int64_t 
    SGN = [\-+];
    DIGITS = digit+;
    INT = (SGN? DIGITS ) >begin_int %end_int;
    
    # 64-bit (double) float TODO ISO syntax
    FRAC = "." DIGITS;
    EXP = [eE] SGN? DIGITS;
    FLOAT = ( SGN? DIGITS ( FRAC EXP? | EXP ) ) >begin_float %end_float;

    # a char TODO UTF8, escapes, \u escapes
    # UTF8 = TODO;

    # JSON-ey string
    UNIESC = "\\u" [0-9a-fA-F]{4};
    ESC = "\\" [nrt\\b'/"];
    CHAR = CODEPOINT - ['\n\r\\];
    STRING = ( (UNIESC|ESC|CHAR)* ) >begin_string %end_string;

    # op term (header op, raw/reduced op, query op)
    OPTERM = [,;!?] @op_term;
    
    # value atom (payload) - int, float, string, UUID
    BARE_ATOM = INT | FLOAT | UUID %end_bare_uuid;
    QUOTED_ATOM =        
            "=" WS* INT  |
            "^" WS* FLOAT |
            ['] STRING ['] |
            ">" WS* UUID %end_quoted_uuid;
    ATOM = QUOTED_ATOM | WS BARE_ATOM ;

    # op's specifier, @id :ref
    SPEC = '@' UUID %end_id WS* ( ':' UUID %end_ref )? ;
    ATOMS = ATOM (WS* ATOM)* ;

    # op spans
    SPAN = ( [(] WS* ([']STRING['])? WS* digit+ WS* [)] );

    # RON op: an immutable unit of change
    OP = (SPEC|BARE_ATOM)? WS* ATOMS? WS* SPAN? WS* OPTERM ;

    # a frame terminator (mandatory in the streaming mode)
    DOT = ".\n" ;

    # RON frame (open text coding)
    TEXT_FRAME = (WS* OP)* WS* DOT? ;

}%%
