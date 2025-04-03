#!/bin/bash

set -e

SRC_FILE="test_device.cpp"
EXE_FILE="test_device.out"

# Load Intel oneAPI environment
source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1

# Compile the C++ file using DPC++
echo "Compiling $SRC_FILE..."
dpcpp "$SRC_FILE" -o "$EXE_FILE"

# Run the compiled executable
echo "Running $EXE_FILE..."
./"$EXE_FILE"

