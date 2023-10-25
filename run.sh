#!/usr/bin/env sh


cmake -B cmake-debug-build && cmake --build cmake-debug-build || exit $?

clang-16 -O2 \
    -fpass-plugin=cmake-debug-build/libTracePass.so \
    hw1/src/main.c \
    -emit-llvm -S \
    -o "$(mktemp)"
