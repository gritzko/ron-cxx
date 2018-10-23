%%{

    machine OPEN_TEXT_FRAME;

    action spec_end { _default_spec(); }
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
    action op_end { fbreak; }

    # Base64 value
    DGT = [0-9a-zA-Z~_];

    # UUID type: name, hash, event or derived event 
    VERSION = [\$\%\+\-];

    # full int 
    WORD = DGT*;

    INT = ([\-+]? digit+ ) >int_start %int_end;
    
    # 64-bit (double) float 
    FLOAT = ( [\-+]? [0-9]+ "." [0-9]+ ([eE] [\-+]? digit+ )? ) >float_start %float_end;

    UUID = ( WORD ( (VERSION @version)? WORD )? ) >uuid_start %uuid_end;

    UTF8 = [^"'\n\r\\];

    STRING = UTF8* >string_start %string_end;

    OPTERM = space* [,;!?] @op_term;
    
    ATOM = space* (
            ("="|space) space* INT  |
            ("^"|space) space* FLOAT |
            ['] STRING ['] |
            (">"|space) space* UUID |
            space INT |
            space FLOAT |
            space UUID 
           ) ; 

    SPEC = '@' UUID ( ':' UUID )? %spec_end;
    NOSPEC = "" %spec_end;

    OP = space* ( SPEC ATOM* OPTERM? | NOSPEC ATOM* OPTERM ) %op_end;

    # optional frame terminator; mandatory in the streaming mode 
    DOT = "." ;

    OPEN_TEXT_FRAME = OP* DOT? ;

}%%
