# Replicated Object Notation

This lib contains RON per se, builders, parsers, global constants, etc

## TODO

- [ ] Op to support incremental string building
- [ ] Use Result instead of Status where possible
- [ ] rework the Merkle hashing code
    - [ ] make the hashing code pluggable (SHA2, SHA3, multihashes)
    - [ ] use the implementation's Base64 for hashes
    - [ ] yarn cross-hashing by inclusion ops only
- [ ] add the binary RON coding

## Files

```
const.hpp                Protocol constants
cp-parser.cc             Ragel-generated UTF8(esc) codepoint parser
encdec.hpp               Base64, Unicode encode/decode routines (TODO use a lib)
form.hpp                 Defines global name UUIDs (e.g. RDT names)
forms.txt
frames.hpp               function templates for RON frame manipulation
hash.cc                  SHA2 wrapper
hash.hpp                 ...
inc_stack.hpp            a stack of mostly-sequential values
op.hpp                   a single "homeless" op, mimics the Cursor interface
opmeta.hpp               op-in-an-object metadata
portable_endian.hpp      endianness utilities
ron.hpp                  an aggregate lib header
slice.hpp                Slice: non-owning memory slice, also basic types
status.cc                func completion status (we use no exceptions here)
status.hpp               ...
test                     tests (unit, fuzz)
text-builder.cc          Textual RON - frame builder
text.cc                  Textual RON - misc functions
text.hpp                 Textual RON - header
text-parser.cc           Textual RON - Ragel generated parser
utf8-parser.cc           Pure UTF8 parser (no escapes)
uuid.cc                  RON UUID
uuid.hpp                 ...
uuid-parser.cc           RON UUID - Ragel generated parser
vv.hpp                   version vector
```
