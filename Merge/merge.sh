#!/bin/bash

# Loop over centralities 1-9
for cen in {1..9}; do
    perl combineResults_simple.pl $cen
    root -b -q myCombine.C++
    rm -rf myCombine*  
done

