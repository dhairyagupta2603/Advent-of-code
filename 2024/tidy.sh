#!/usr/bin/env bash
set -e

clang-tidy --extra-arg=-std=c++17 $1