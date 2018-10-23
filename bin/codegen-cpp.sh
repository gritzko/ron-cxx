#!/bin/bash

#cat sep2bits.txt | ./sep2bits.pl > $CPPINC/ron/bitsep.hpp && \
ragel -C -G2 -e ragel/uuid-parser.rl -o ron/uuid-parser.cc
ragel -C -G2 -e ragel/text-parser.rl -o ron/text-parser.cc
ragel -C -G2 -e ragel/open-parser.rl -o ron/open-text-ron-parser.cc
