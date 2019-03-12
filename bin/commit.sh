#!/bin/bash

DIR=`date +%H:%M:%S_%d-%m-%Y`

if [ ! -d .git ]; then
    echo please launch from the project\'s root
    echo 1
fi

git pull || exit 9

git diff $@ || exit 10

pause

git add $@ || exit 4

git stash push --keep-index -m "$DIR" | grep WIP
STASH=$?
if [ $STASH ]; then
    echo stashed $DIR
    git stash show
    pause
fi

mkdir $DIR || exit 2

cd $DIR

cmake .. || exit 3

make || exit 5
export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH:/lib:/usr/lib:/usr/local/lib
make test || exit 7

make format

cd ..

git add $@ || exit 11

git commit -S || exit 6

#git push || exit 8

if [ $STASH ]; then
    git stash pop
fi

rm -rf $DIR
