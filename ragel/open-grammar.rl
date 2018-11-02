%%{

    machine OPEN_TEXT_FRAME;

    action spec_end { _parse_spec(); }
    action int_start { int_start = p; }
    action int_end { _parse_int(int_start, p); }
    action uuid_start { uuid_start = p; version_at = pb; }
    action version { version_at = p; }
    action uuid_end { _parse_uuid(uuid_start, version_at, p); }
    action string_start { string_start = p; }
    action string_end { _parse_string(string_start, p); }
    action op_term { _set_term(fc); }
    action float_start { float_start = p; }
    action float_end { _parse_float(float_start, p); }
    action op_end { _parse_op(); if (p<pe-1) { fhold; fbreak; } }

    # Base64 value
    DGT = [0-9a-zA-Z~_];

    # UUID type: name, hash, event or derived event 
    VERSION = [\$\%\+\-];

    # a RON word (60+4 bits, half of a UUID)
    WORD = DGT+;

    # int64_t 
    INT = ([\-+]? digit++ ) >int_start %int_end;
    
    # 64-bit (double) float TODO ISO syntax 
    FLOAT = ( [\-+]? [0-9]+ "." [0-9]+ ([eE] [\-+]? digit+ )? ) >float_start %float_end;

    # RON UUID
    UUID = ( WORD ( (VERSION @version)? WORD )? ) >uuid_start %uuid_end;

    # a char TODO UTF8, escapes, \u escapes
    UTF8 = [^"'\n\r\\];

    # JSON-ey string
    STRING = ( UTF8* ) >string_start %string_end;

    # op term (header op, raw/reduced op, query op)
    OPTERM = space* [,;!?] @op_term;
    
    # value atom (payload) - int, float, string, UUID
    BARE_ATOM = INT | FLOAT | UUID;
    ATOM = space* (
            ("="|space) space* INT  |
            ("^"|space) space* FLOAT |
            ['] STRING ['] |
            (">"|space) space* UUID |
            space BARE_ATOM 
           ) ; 

    # op's specifier, @id :ref
    SPEC = '@' UUID ( ':' UUID )? %spec_end;

    # RON op: an immutable unit of change
    OP = space* ( SPEC ATOM* <: OPTERM? | (BARE_ATOM|ATOM) ATOM* <: OPTERM? ) %op_end ;

    # optional frame terminator; mandatory in the streaming mode 
    DOT = ".\n" ;

    # RON frame (open text coding)
    OPEN_TEXT_FRAME = OP* DOT? ;

}%%
