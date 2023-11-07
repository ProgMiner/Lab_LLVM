cmake_minimum_required(VERSION 3.16)
project(llvm-hw LANGUAGES C CXX)

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)

FetchContent_MakeAvailable(SFML)

add_executable(llvm-hw src/main.c src/sim.cpp src/sim.h src/sim.hpp)

target_link_libraries(llvm-hw PRIVATE sfml-graphics)

target_compile_features(llvm-hw PRIVATE c_std_11)
target_compile_features(llvm-hw PRIVATE cxx_std_17)

target_compile_options(llvm-hw PRIVATE -fsanitize=address)
target_link_options(llvm-hw PRIVATE -fsanitize=address)
