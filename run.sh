#!/usr/bin/env sh

export CC="$(which clang-16)"
export CXX="$(which clang++-16)"


cmake -B cmake-debug-build && cmake --build cmake-debug-build || exit $?

"$CC" -O2 \
    -fpass-plugin=cmake-debug-build/libTracePass.so \
    hw1/src/main.c \
    -emit-llvm -S \
    -o main.ll || exit $?


cd ./hw1 || exit $?
cmake -B cmake-debug-build && cmake --build cmake-debug-build || exit $?

cd ..
./hw1/cmake-debug-build/llvm-hw
