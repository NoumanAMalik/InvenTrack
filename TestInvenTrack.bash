#!/bin/bash
# A script to test InvenTack.cpp

## First reset the database
./ResetDatabase.bash

## Compile InvenTrack.cpp
g++ -std=c++20 -Wall -Wextra -Werror -l sqlite3 InvenTrack.cpp -o InvenTrack.exe

## Run Compiled .exe
# ./InvenTrack.exe -q 10 -m ScAnIn -u 1110
./InvenTrack.exe -m addproduct