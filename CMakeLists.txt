cmake_minimum_required(VERSION 3.16)
project(llvm-hw LANGUAGES C CXX)

set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)

FetchContent_MakeAvailable(SFML)

find_package(LLVM REQUIRED CONFIG)
# add_definitions(${LLVM_DEFINITIONS})

# pure implementation

add_executable(orig src/main.c src/sim.cpp src/sim.h src/sim.hpp)

target_link_libraries(orig PRIVATE sfml-graphics)

target_compile_features(orig PRIVATE c_std_11)
target_compile_features(orig PRIVATE cxx_std_17)

target_compile_options(orig PRIVATE -fsanitize=address)
target_link_options(orig PRIVATE -fsanitize=address)

# through LLVM

add_executable(llvm src/main.cpp src/sim.cpp src/sim.h src/sim.hpp)

target_include_directories(llvm PRIVATE ${LLVM_INCLUDE_DIRS})

target_link_libraries(llvm PRIVATE sfml-graphics)
target_link_libraries(llvm PRIVATE LLVM)

target_compile_features(llvm PRIVATE cxx_std_17)

target_compile_options(llvm PRIVATE -fsanitize=address)
target_link_options(llvm PRIVATE -fsanitize=address)
