#!/usr/bin/env bash
set -e

g++-10 -std=c++17 -Wall -Wpedantic -Wextra -O3 -I/home/dhairya/projects/advent-of-code/3party/nlohmann/include/ $1 && ./a.out