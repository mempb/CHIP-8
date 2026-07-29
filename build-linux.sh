#!/bin/bash
set -e

rm -rf build
mkdir build
cd build
cmake ..
cmake --build .

echo "Build complete. Run with: ./build/CHIP-8"