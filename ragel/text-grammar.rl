%%{

    machine FRAME;
    include UUID "./uuid-grammar.rl";

    action spec_start {  // std::cerr<<"spec_start"<<std::endl;
        atoms.resize(SPEC_SIZE);
    }

    action redef_uuid {  // std::cerr<<"redef_uuid"<<std::endl;
        if (atm>0) {
            atoms[atm] = atoms[atm-1];
        }
    }

    action spec_uuid_start {  // std::cerr<<"spec_uuid_start"<<std::endl;
        n = (int)(ABC[fc]);
        if (!(op_.coding()&RON::CLOSED)) {
            n-=2;
            if (n<0) {
                fnext *RON_error;
                fbreak;
            }
        }
	    dgt = 0;
        if (n < atm) { 
            // parse #op1#op2#op3 without Ragel state explosion
            fnext *RON_start;
            position++;
            p--;
            fbreak;
        } else { 
            // next UUID
            atm = n;
        }
    }

    action spec_uuid_end {  // std::cerr<<"spec_uuid_end "<<std::endl;
        atm++;
    }

    action atom_start {  // std::cerr<<"atom_start"<<std::endl;
	    dgt = 0;
        atoms.push_back(Atom{});
        aso = p-be+1;
        atoms[atm][ORIGIN].put2(31, VARIANT::RON_ATOM);
        atoms[atm][ORIGIN].put30(1, aso);
    }
    action atom_end {  // std::cerr<<"atom_end"<<std::endl;
        if (atoms[atm].variant()==VARIANT::RON_ATOM) {
            atoms[atm][ORIGIN].put30(0, (p-be)-aso-1);
        }
        atm++;
    }

    action int_atom_start {  // std::cerr<<"int_atom_start "<<(p-pb)<<std::endl;
        atoms[atm][ORIGIN].put2(30, ATOM::INT);
    }
    action int_atom_end {  // std::cerr<<"int_atom_end "<<(p-pb)<<std::endl;
    }

    action float_atom_start {  // std::cerr<<"float_atom_start"<<std::endl;
        atoms[atm][ORIGIN].put2(30, ATOM::FLOAT);
    }
    action float_atom_end {  // std::cerr<<"float_atom_end"<<std::endl;
    }

    action string_atom_start {  // std::cerr<<"string_atom_start"<<std::endl;
	    atoms[atm][ORIGIN].put2(30, ATOM::STRING);
    }
    action string_atom_end {  // std::cerr<<"string_atom_end"<<std::endl;
    }

    action uuid_atom_start {  // std::cerr<<"uuid_atom_start"<<std::endl;
        atoms[atm] = atoms[atm-1];
    }

    action atoms_start {  // std::cerr<<"atoms_start"<<std::endl;
        atm = SPEC_SIZE;
        dgt = 0;
    }
    action atoms {  // std::cerr<<"atoms"<<std::endl;
    }

    action opterm {  // std::cerr<<"opterm"<<std::endl;
        term = (enum TERM) (ABC[fc]);
    }

    action op_start {  // std::cerr<<"op_start"<<std::endl;
        if (p>pb && position!=-1) {
            // one op is done, so stop parsing for now
            // make sure the parser restarts with the next op
            p--;
            fnext *RON_start;
            fbreak;
        } else {
            //op_idx++;
            if (term!=TERM::RAW) {
                term = TERM::REDUCED;
            }
        }
    }

    action op_end {  // std::cerr<<"op_end"<<std::endl;
        position++;
    }

    action spec_end {  // std::cerr<<"spec_end"<<std::endl;
    }

    action frame_end {  // std::cerr<<"frame_end"<<std::endl;
        fnext *RON_FULL_STOP;
        fbreak;
    }

    # one of op spec UUIDs: type, object, event id or a reference 
    REDEF = "`" @redef_uuid;
    QUANT = [*#@:] @spec_uuid_start ;
    SPEC_UUID = QUANT space* REDEF? (UUID space*)? %spec_uuid_end ;

    # 64-bit signed integer 
    INT_ATOM = ([\-+]? digit+ ) >int_atom_start %int_atom_end;

    # 64-bit (double) float 
    FLOAT_ATOM = ( [\-+]? [0-9]+ "." [0-9]+ ([eE] [\-+]? digit+ )? ) >float_atom_start %float_atom_end;

    UUID_ATOM = UUID >uuid_atom_start ;

    # JSON-escaped string 
    UNIESC = "\\u" [0-9a-fA-F]{4};
    ESC = "\\" [^\n\r];
    CHAR = [^"'\n\r\\];
    STRING_ATOM = ( (UNIESC|ESC|CHAR)* ) >string_atom_start %string_atom_end;

    # an atom (int, float, string or UUID) 
    ATOM = (
            "=" space* INT_ATOM  |
            "^" space* FLOAT_ATOM |
            ['] STRING_ATOM ['] |
            ">" space* UUID_ATOM
            ) >atom_start %atom_end space*;
    # op value - an atom, an atom tuple, or empty 
    ATOMS = ATOM+ %atoms >atoms_start;

    # an optional op terminator (reduced, raw, header, query)
    OPTERM = [,;!?] @opterm space*;

    # a RON op "specifier" (four UUIDs for its type, object, event, and ref)
    SPEC = SPEC_UUID+ >spec_start %spec_end ;

    # a RON op
    # op types: (0) raw op (1) reduced op (2) frame header (3) query header 
    OP = space* ( SPEC ATOMS? OPTERM? | ATOMS OPTERM? | OPTERM) $2 %1 >op_start %op_end;

    # optional frame terminator; mandatory in the streaming mode 
    DOT = "." @frame_end;

    # RON frame, including multiframes (those have more headers inside) 
    FRAME = OP* DOT? ;

}%%
