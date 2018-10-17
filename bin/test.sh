#!/bin/bash

if [ ! -d build ]; then
    mkdir build
fi
cd build
make clean
( cmake .. && make -j8 ) || exit 1
ls test-* | awk '{print "./" $1 " && \\"}' > all-tests.sh
echo "echo OK" >> all-tests.sh
chmod u+x ./all-tests.sh
./all-tests.sh

