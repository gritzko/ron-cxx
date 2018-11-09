%%{

    machine TEXT_FRAME;
    include UUID "./uuid-grammar.rl";

    action end_id {
        op_.SetId(Uuid{variety, value, version, origin});
    }
    action end_ref {
        op_.SetRef(Uuid{variety, value, version, origin});
    }
    action begin_int { intb.begin(p); }
    action end_int {
        intb.end(p);
        if (intb.size() > 21) { cs = 0; fbreak; }
        op_.AddAtom(Atom::Integer(body.range_of(intb))); 
        lastintb = intb.buf_;
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
        op_.AddAtom(Atom::Float(body.range_of(floatb))); 
    }
    action end_arrow_uuid {
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
    action end_bare_uuid { 
        if (!intb.same(uuidb))
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
    action op_term { 
        term = fc; 
        pos_++; 
        if (p<pe-1) fbreak;
    }

    # int64_t 
    SGN = [\-+];
    DIGITS = digit digit**;
    INT = (SGN? DIGITS ) >begin_int %end_int;
    
    # 64-bit (double) float TODO ISO syntax
    FRAC = "." DIGITS;
    EXP = [eE] SGN? DIGITS;
    FLOAT = ( SGN? DIGITS ( FRAC EXP? | EXP ) ) >begin_float %end_float;

    # a char TODO UTF8, escapes, \u escapes
    UTF8 = [^'\n\r\\];

    # JSON-ey string
    STRING = ( UTF8* ) >begin_string %end_string;

    # op term (header op, raw/reduced op, query op)
    OPTERM = [,;!?] @op_term;
    
    # value atom (payload) - int, float, string, UUID
    BARE_ATOM = INT | FLOAT | UUID %end_bare_uuid;
    QUOTED_ATOM =        
            "=" space* INT  |
            "^" space* FLOAT |
            ['] STRING ['] |
            ">" space* UUID %end_arrow_uuid ;    
    ATOM = QUOTED_ATOM | space BARE_ATOM ;

    # op's specifier, @id :ref
    SPEC = '@' UUID %end_id space* ( ':' UUID %end_ref )? ;
    ATOMS = ATOM (space* ATOM)* ;

    # RON op: an immutable unit of change
    OP = (SPEC|BARE_ATOM)? space* ATOMS? space* OPTERM ;

    # a frame terminator (mandatory in the streaming mode)
    DOT = ".\n" ;

    # RON frame (open text coding)
    TEXT_FRAME = (space* OP)* space* DOT? ;

}%%
