#!/bin/bash

if [ -z $PRJ ]; then
    PRJ=$PWD
fi
echo Using the working dir $PRJ

if [ -z $SAN ]; then
    SAN=address
fi
echo Using sanitizer: $SAN
export CXXFLAGS="$CXXFLAGS -fsanitize=$SAN"

BUILDDIR=$PRJ/build

if [ ! -d $PRJ ]; then
    echo no working dir found
    exit -2
fi

if [ -d $BUILDDIR ]; then
    exit -1
else
    mkdir $BUILDDIR || exit 6
fi

if [[ -z MAKEFLAGS && -e /proc/cpuinfo ]]; then
    export MAKEFLAGS=-j$(($(grep ^cpu\\scores /proc/cpuinfo | uniq |  awk '{print $4}') - 0))
fi

{

cd $BUILDDIR
env
git pull || exit 7
cmake -DCMAKE_BUILD_TYPE=Debug .. || exit 1
make -j10 VERBOSE=1 || exit 2

for i in `seq 1 100`; do
    make test || exit 3
done

clang++ -g -O1 -fsanitize="fuzzer,$SAN" -I.. ../ron/test/fuzz-text.cc -I../deps/include ../ron/*.cc -o fuzz-text || exit 4

./fuzz-text -workers=20 -jobs=10 -max_total_time=600 -dict=../test/dict.ron ~/data/fuzz-text ../test/rdt/ || exit 5

} > $BUILDDIR/ci.log 2>&1

rm -rf $BUILDDIR
