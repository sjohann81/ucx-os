#!/bin/sh
 
# Simple script to rename .obj to .o in current directory
for file in *.obj; do
    [ -e "$file" ] || continue  # Skip if no .obj files exist
    mv -- "$file" "${file%.obj}.o"
done