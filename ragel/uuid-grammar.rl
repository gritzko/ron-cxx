%%{

    machine UUID;

    action start_uuid {
        hlf = 0;
    }

    action int60_dgt {
        if (dgt>9) {
            fbreak;
        }
        word(atm,hlf).put6(9-dgt, ABC[fc]);
        dgt++;
    }

    action start_full_int {
        //word(atm, hlf).zero_payload();
    }

    action start_value {
    }

    action start_origin {
        dgt = 0;
        hlf = 1;
    }

    action end_value {
    }

    action end_origin {
    }

    action variety {
        word(atm, 0).set_flags(word(atm, 0).get6(9));
        word(atm, hlf).zero_payload();
        dgt--;
    }

    action uuid_sep {
        hlf = 1;
        word(atm, 1).zero_flags();
        word(atm, 1).set_flags(ABC[fc]);
    }

    action end_name {
        word(atm, 1).zero();
        word(atm, 1).set_flags(UUID::NAME);
    }

    # Base64 value
    DGT = [0-9a-zA-Z~_] @int60_dgt;
    # UUID type: name, hash, event or derived event 
    VERSION = [\$\%\+\-] @uuid_sep;
    # full int 
    INT = DGT* >start_full_int;

    # first half of an UUID 
    VALUE = (DGT >start_value ([\/] @variety)? DGT*) %end_value;
    #VALUE = INT >start_value %end_value ;
    # second half of an UUID 
    ORIGIN = INT >start_origin %end_origin ;
    # global name UUID, e.g. "lww" (aka transcendent constant) 
    NAME = VALUE >start_value %end_value %end_name;

    # RON 128 bit UUID 
    UUID =  ( VALUE ( VERSION ORIGIN? )? | VERSION ORIGIN? )
            >start_uuid
           ;

    # main := UUID;

}%%
