#!/bin/bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug 
cmake --build build 
mkdir -p output 
cp build/src/flux-nodes-ui output/flux-nodes-ui 
rm -rf output/python_stuff
cp -r python_stuff output/python_stuff  