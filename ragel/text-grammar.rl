%%{

    machine TEXT_FRAME;
    include UUID "./uuid-grammar.rl";

    action end_id {
        std::cerr<<op_.size()<<"id "<<uuid.str()<<'\n';
        op_.SetId(Uuid::Parse(variety, value, version, origin));
    }
    action end_ref {
        std::cerr<<op_.size()<<"ref "<<uuid.str()<<'\n';
        op_.SetRef(Uuid::Parse(variety, value, version, origin));
    }
    action begin_int { intb = p; }
    action end_int { 
        std::cerr<<"I"<<intb<<"\n"; 
        op_.AddAtom(Atom::Integer(intb-pb, p-intb)); 
        uuid.buf_ = 0; // bust the uuid
    }
    action begin_string { strb = p; }
    action end_string { std::cerr<<"S\n"; op_.AddAtom(Atom::String(strb-pb, p-strb)); }
    action op_term { term = fc; pos_++; if (p<pe-1) { fbreak; } }
    action begin_float { floatb = p; }
    action end_float { std::cerr<<"F\n"; op_.AddAtom(Atom::Float(floatb-pb, p-floatb)); }
    action end_op {  }
    action end_arrow_uuid {
        op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
    }
    action end_bare_uuid { 
        if (uuid.buf_)
            op_.AddAtom(Uuid::Parse(variety, value, version, origin)); 
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
    OP = (SPEC|BARE_ATOM)? space* ATOMS? space* OPTERM %end_op ;

    # optional frame terminator; mandatory in the streaming mode 
    DOT = "." ;

    # RON frame (open text coding)
    TEXT_FRAME = (space* OP)* space* DOT? ;

}%%
