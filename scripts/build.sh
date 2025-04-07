#!/bin/bash
# Create build directory if it doesn’t exist
mkdir -p build
# Configure CMake
cmake -S . -B build
# Build the project
cmake --build build