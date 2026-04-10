#!/bin/bash

# Auto format all project header and source files with
# clang-format 

for dir in */; do
    echo "Formatting: $dir"
    clang-format -i "$dir"*.c "$dir"*.h 2>/dev/null
done
