#!/bin/bash

echo this configures the project for a clang++/libc++ build

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_CXX_FLAGS="-I/usr/local/include/c++/v1 -stdlib=libc++" -DCMAKE_EXE_LINKER_FLAGS="-lc++ -lc++abi -lm -lc" -G "Unix Makefiles" ..

