%%{

    machine UUID;

    action start_uuid {
        hlf = VALUE;
    }

    action int60_dgt { // std::cerr << "digit " << fc << '\n';
        if (dgt>9) {
            fbreak;
        }
        atoms[atm][hlf].put6(9-dgt, ABC[fc]);
        dgt++;
    }

    action start_full_int {
        atoms[atm][hlf].zero_payload();
    }

    action start_value {
    }

    action start_origin {
        dgt = 0;
        hlf = ORIGIN;
    }

    action end_value {
    }

    action end_origin {
    }

    action variety {
        atoms[atm][VALUE].set_flags(atoms[atm][VALUE].get6(9));
        atoms[atm][VALUE].zero_payload();
        dgt--;
    }

    action uuid_sep {
        hlf = ORIGIN;
        atoms[atm][ORIGIN].zero_flags();
        atoms[atm][ORIGIN].set_flags(ABC[fc]);
    }

    action end_name {
        atoms[atm][ORIGIN].zero();
        atoms[atm][ORIGIN].set_flags(UUID::NAME);
    }

    # Base64 value
    DGT = [0-9a-zA-Z~_] @int60_dgt;
    # UUID type: name, hash, event or derived event 
    VERSION = [\$\%\+\-] @uuid_sep;
    # full int 
    INT = DGT* >start_full_int;

    # first half of an UUID 
    VALUE = (DGT >start_value ([\/] @variety)? DGT*) %end_value >start_full_int;
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
