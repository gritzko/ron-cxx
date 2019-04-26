#!/bin/bash

ragel -C -G2 -e ragel/uuid-parser.rl -o ron/uuid-parser.cc || exit 1
ragel -C -G2 -e ragel/text-parser.rl -o ron/text-parser.cc || exit 2
ragel -C -G2 -e ragel/cp-parser.rl -o ron/cp-parser.cc || exit 3
#if [ ! -d build ]; then
#    mkdir build | exit 4;
#fi
#cd build && cmake .. && make && make format && make test
echo
echo CODEGEN OK
