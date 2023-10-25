cmake_minimum_required(VERSION 3.16)
project(llvm-hw LANGUAGES C CXX)


find_package(LLVM REQUIRED CONFIG)

add_library(TracePass MODULE
    # List your source files here.
    TracePass.cpp)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Use C++11 to compile our pass (i.e., supply -std=c++11).
target_compile_features(TracePass PRIVATE cxx_range_for cxx_auto_type)

# LLVM is (typically) built with no C++ RTTI. We need to match that;
# otherwise, we'll get linker errors about missing RTTI data.
set_target_properties(TracePass PROPERTIES
    COMPILE_FLAGS "-fno-rtti")

# Get proper shared-library behavior (where symbols are not necessarily
# resolved when the shared library is linked) on OS X.
if(APPLE)
    set_target_properties(TracePass PROPERTIES
        LINK_FLAGS "-undefined dynamic_lookup")
endif(APPLE)
