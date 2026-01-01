#!/bin/bash

# pro jistotu
rm -rf build

mkdir -p build
cd build
cmake .. && make && ./breakout
