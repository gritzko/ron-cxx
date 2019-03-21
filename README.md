# RON 2.1 reference implementation in C++

Replicated Object Notation (RON) is a format for distributed live data.
See http://replicated.cc for the protocol spec.

This is a Ragel/C++ implementation of:

* Replicated Object Notation (parsers/builders),
* Replicated Data Types (RON based CRDTs),
* libswarmdb.so, an embedded syncable database (RocksDB-based),
* swarmdb, a standalone RON-native database.

Still in the works / planned / envisioned:

* libswarmdb bindings for higher-level languages
    - [ ] Java
    - [ ] node.js
    - [ ] Python
    - [ ] Go
    - [ ] ?Haskell
- [ ] WebAssembly (in-memory) lib,
- [ ] other backends.

## Building

* ensure you have
    * a C++ compiler,
    * make,
    * CMake,
    * git
* mkdir build && cd build
* cmake ..
* make

Static dependencies are fetched from the net and built by cmake:

* rocksdb,
* gflags,
* botan-2,
* snappy.

Dymanic dependencies are taken from the system:

* zlib,
* libbz2.

## New 2.1 features

* RON-open two-UUID notation
* strict references (every object is a causal tree)
* merge/gc separation (thanks @archagon)
* multiple encodings (text, binary, JSON, CBOR)
* chain-based compression

## Contents

* bin/ - utility scripts
* ragel/ - [Ragel](http://www.colm.net/open-source/ragel/) grammars, parsers
* ron/ - RON basic primitives, encodings
* rdt/ - replicated data types, merge, gc
* db/ - RocksDB based implementation
* deps/ - libs, dependencies
* ...

## Contributing

Code conventions as per [Google Style Guide](https://google.github.io/styleguide/cppguide.html).
Preferably, use `bin/commit.sh` for commits.
