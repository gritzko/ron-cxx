%%{

    machine TEXT_FRAME;
    alphtype unsigned char;

    include UUID "./uuid-grammar.rl";
    include UTF8 "./utf8-grammar.rl";

    action end_id {
        op_[0] = Uuid{variety, value, version, origin};
    }
    action end_ref {
        op_[1] = Uuid{variety, value, version, origin};
    }
    action begin_int { intb = p; }
    action end_int {
        Slice the_int(intb, p);
        if (the_int.size()>=19 && int_too_big(the_int)) { cs = 0; fbreak; }
        //op_.push_back(Atom::Integer(parse_int(the_int), body.range_of(the_int))); 
        // TODO atoms.emplace();
        op_.push_back(Atom{INT, body.range_of(the_int)});
        uuidb = nullptr; // sabotage uuid
    }
    action begin_string { strb = p; cp_size = 0; }
    action end_string { 
        Slice the_str{strb,p};
        //op_.push_back(Atom::String(body.range_of(the_str))); 
        Atom a = Atom{STRING, body.range_of(the_str)};
        a.value.cp_size = cp_size;
        op_.push_back(a);
    }
    action begin_float { floatb = p; }
    action end_float { 
        Slice the_float{floatb,p};
        if (the_float.size() > 24) { cs = 0; fbreak; }
        //op_.push_back(Atom::Float(parse_float(the_float), body.range_of(the_float))); 
        op_.push_back(Atom{FLOAT, body.range_of(the_float)});
    }
    action end_quoted_uuid {
        if (word_too_big(value) || word_too_big(origin)) { cs = 0; fbreak; }
        op_.push_back(Uuid{variety, value, version, origin}); 
    }
    action end_bare_uuid { 
        if (uuidb!=nullptr) { // " 123 " is an int, not an UUID
            if (word_too_big(value) || word_too_big(origin)) { cs = 0; fbreak; }
            op_.push_back(Uuid{variety, value, version, origin}); 
        }
    }
    action end_cp {
        cp_size++;
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
    action string_esc { cp = decode_esc(fc); }
    action string_uesc { cp = decode_hex_cp(Slice{p-4,4}); }

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
    UNIESC = "\\u" [0-9a-fA-F]{4} %string_uesc;
    ESC = "\\" [nrt\\b'/"] @string_esc;
    CHAR = CODEPOINT - ['\n\r\\];
    CP = (UNIESC|ESC|CHAR) %end_cp;
    STRING = ( CP* ) >begin_string %end_string;

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
