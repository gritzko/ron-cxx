#!/bin/bash

#cat sep2bits.txt | ./sep2bits.pl > $CPPINC/ron/bitsep.hpp && \
ragel -C -G2 -e ragel/uuid-parser.rl -o ron/uuid-parser.cc
#ragel -C -G2 -e $RAGELPATH/parser.rl -o $CPPSRC/parser.cpp
