#!/bin/bash
#this file is a script that allows the user to compile opencv programs
# Copyright (c) 2016, SnapVision
# All rights reserved.

echo "compiling $1"
if [[ $1 == *.c ]]
then
  gcc -ggdb `pkg-config --cflags opencv` -o `basename $1 .c` $1 `pkg-config --libs opencv`;
elif [[ $1 == *.cpp ]]
then
  g++ -ggdb `pkg-config --cflags opencv` -o `basename $1 .cpp` $1 `pkg-config --libs opencv` -std=c++11 -fopenmp;
else
  echo "Please compile only .c or .cpp files"
fi
echo "Output file => ${1%.*}"