#!/bin/bash

# Auto format all project header and source files with
# clang-format 

cd ..

echo "Formatting: root"
meson format -i meson.build

for dir in */; do
    echo "Formatting: $dir"
    clang-format -i "$dir"*.c "$dir"*.h 2>/dev/null
    meson format -i meson.build
done

echo "Done"
