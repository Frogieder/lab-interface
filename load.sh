#!/usr/bin/env sh

echo "Size of the file is" "$(du ./cmake-build-debug/src/lab_interface.uf2 -h | cut -f 1)"
picotool load -v ./cmake-build-debug/src/lab_interface.uf2 -f