#!/bin/sh

rm -rf build_x
mkdir build_x && cd build_x
cmake .. -G Xcode
