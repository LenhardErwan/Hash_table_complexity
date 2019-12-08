#!/bin/sh

mkdir bin 2>/dev/null
echo "Build..."
#-Wno-psabi it's for disable warning about parameter passing in vector in GCC 7.1
g++ -Wall -std=c++11 -Wno-psabi src/main.cpp -o bin/main.exe
echo "Done !"
