#!/bin/bash
# energy, path
energy=$1
path=$2
rm -rf New_Cuts
mkdir New_Cuts
cp Sample_code.cpp file_looper.cpp
root -b -q -l "file_looper.cpp($energy, \"$path\")"
rm -rf file_looper.cpp
