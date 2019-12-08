@echo off

mkdir bin 2>NUL
echo Build...
g++ -Wall -std=c++11 src\main.cpp -o bin\main.exe
echo Done !