#!/bin/bash


# buildディレクトリがなければ作成
if [ ! -d build ]; then
  mkdir build
fi

cmake --build build -DCMAKE_TOOLCHAIN_FILE=riscv32-toolchain.cmake --target all -- -j 4
cmake --build build --target run
