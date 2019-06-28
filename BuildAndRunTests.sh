#!/bin/sh

cmake --build ./build/ --target all
cd build
ctest
