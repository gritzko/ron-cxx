# RON 2.1 reference implementation in C++

Replicated Object Notation (RON) is a format for distributed live data.
This Ragel/C++ implementation aims to be used as a rocksdb-based storage
engine, also as a library in higher-level languages, using bindings.
See http://replicated.cc for the protocol spec.

## Building

* ensure you have a C++ compiler, make, CMake
* mkdir build && cd build && cmake .. && make

SwarmDB depends on rocksdb, gflags, botan-2, snappy, but it downloads
and builds them on its own. zlib, libbz2 are taken from the system.

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
