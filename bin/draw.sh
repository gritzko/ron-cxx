#!/bin/bash

echo UUID grammar
ragel -V -p ragel/uuid-parser.rl > tmp/uuid.dot
dot -otmp/uuid.svg -Tsvg tmp/uuid.dot

echo text grammar
ragel -V -p ragel/text-parser.rl > tmp/text.dot
dot -otmp/text.svg -Tsvg tmp/text.dot

