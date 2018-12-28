#!/bin/bash

if [ ! -d build ]; then
    mkdir build
fi
cd build
#make clean
if [ ! -e Makefile ]; then
    cmake || exit 1
fi
make -j8 || exit 2
ls test??-* | awk '{print "echo " $1 " && ./" $1 " && \\"}' > all-tests.sh
echo "echo OK" >> all-tests.sh
chmod u+x ./all-tests.sh
./all-tests.sh

