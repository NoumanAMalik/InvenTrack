#!/bin/zsh
# A script to test InvenTack.cpp

## First reset the database
./ResetDatabase.zsh

## Compile InvenTrack.cpp
g++ -std=c++20 -Wall -Wextra -Werror -l sqlite3 InvenTrack.cpp -o InvenTrack.exe

## Run Compiled .exe
./InvenTrack.exe -m ScAnIn -upc 1110