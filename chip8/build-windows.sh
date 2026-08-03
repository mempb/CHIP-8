#!/bin/bash
set -e

rm -rf build
cmake -G "MinGW Makefiles" -B build -S .
cmake --build build

echo "Build complete. Run with: ./build/CHIP-8.exe"