#!/bin/bash
rm -rf release
mkdir build release
cd build
cmake -DCMAKE_BUILD_TYPE=debug -DCMAKE_INSTALL_PREFIX=../release ..
make -j 4
make install
make test
