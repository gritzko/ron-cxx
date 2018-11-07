%%{

    machine UUID;

    action begin_uuid { uuid.SetTo(p); variety='0'; version='$'; origin=slice_t{}; }
    action variety { variety = *(p-1); }
    action begin_value { value.SetTo(p); }
    action end_value { value.ExtendUntil(p); }
    action version { version = fc; }
    action begin_origin { origin.SetTo(p); }
    action end_origin { origin.ExtendUntil(p); }
    action end_uuid { uuid.ExtendUntil(p); }

    # digits (base64, hex)
    DGT = [0-9a-zA-Z~_];
    HEX = [0-9A-F];

    # RON UUID variety (1st word flag bits)
    VARIETY = HEX "/" @variety;

    # RON UUID version char (2nd word flag bits)
    VERSION = [\$\%\+\-] @version;

    # 60+4 bit word (the most significant 4 bits are flags, the rest is payload)
    WORD = DGT+;

    # UUID value (the 1st word) 
    VALUE = WORD >begin_value %end_value;

    # UUID origin (the 2nd word)
    ORIGIN = WORD >begin_origin %end_origin;

    # RON UUID (128 bits, two 60+4 bit words) 
    UUID = ( VARIETY? VALUE ( VERSION ORIGIN )? ) >begin_uuid %end_uuid;
    # examples: lww, A/LED, 12345+origin, 1/0000000001+origin, some_hash%12345375868

}%%
