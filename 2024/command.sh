#!/usr/bin/env bash
set -e

g++-10 -std=c++17 -Wall -Wpedantic -Wextra -Wpessimizing-move -g $1 && ./a.out