#!/bin/bash
# energy, path
arg1=$1
arg2=$2
rm -rf New_Cuts
mkdir New_Cuts
cp Sample_code.cpp file_looper.cpp
root -b -q "file_looper.cpp($arg1, \"$arg2\")"
rm -rf file_looper.cpp
