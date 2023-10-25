#!/usr/bin/env sh


clang-16 -O2 -fpass-plugin=cmake-debug-build/libSkeletonPass.so hw1/src/main.c -emit-llvm -S -o main.ll
