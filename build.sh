#!/bin/sh

cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make -B -j 8