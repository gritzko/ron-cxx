#!/bin/bash

ragel -C -G2 -e ragel/uuid-parser.rl -o ron/uuid-parser.cc
ragel -C -G2 -e ragel/text-parser.rl -o ron/text-parser.cc
