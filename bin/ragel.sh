#!/bin/bash

ragel -C -G2 -e ragel/uuid-parser.rl -o ron/uuid-parser.cc || exit 1
ragel -C -G2 -e ragel/text-parser.rl -o ron/text-parser.cc || exit 2
ragel -C -G2 -e ragel/cp-parser.rl -o ron/cp-parser.cc || exit 3
ragel -C -G2 -e ragel/string-parser.rl -o ron/string-parser.cc || exit 4
ragel -C -G2 -e ragel/utf8-parser.rl -o ron/utf8-parser.cc || exit 5
echo CODEGEN OK
