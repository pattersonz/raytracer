#!/bin/bash
mkdir -p build
cd build
cmake ..
make
rm ../XMLS/*.png
for file in ../XMLS/*
do
  ./render/Render -i"$file" -w250 -h250 --rayTracer -r1
done
clear
echo "COMPLETE
Images are in the XMLS folder"
