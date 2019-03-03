#!/bin/bash

ragel -C -G2 -e ragel/uuid-parser.rl -o ron/uuid-parser.cc
ragel -C -G2 -e ragel/text-parser.rl -o ron/text-parser.cc
ragel -C -G2 -e ragel/string-parser.rl -o ron/string-parser.cc
if [ ! -d build ]; then
    mkdir build;
fi
cd build && cmake .. && make && make format && make test
echo
echo CODEGEN OK
