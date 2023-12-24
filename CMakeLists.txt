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
find_package(BISON)
find_package(FLEX)

# add_definitions(${LLVM_DEFINITIONS})

add_compile_options(-Wall -Werror -pedantic-errors)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# pure implementation

add_executable(orig src/main.c src/sim.cpp src/sim.h src/sim.hpp)

target_link_libraries(orig PRIVATE sfml-graphics)

target_compile_features(orig PRIVATE c_std_11)
target_compile_features(orig PRIVATE cxx_std_17)

# through LLVM

FLEX_TARGET(llvm-lexer src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.c
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/lexer.h)

BISON_TARGET(llvm-parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.c
        COMPILE_FLAGS -Wcounterexamples
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
        VERBOSE REPORT_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.txt)

ADD_FLEX_BISON_DEPENDENCY(llvm-lexer llvm-parser)

add_executable(llvm src/main.cpp src/sim.cpp src/sim.h src/sim.hpp src/ast.h src/ast.hpp src/ast.cpp
        ${BISON_llvm-parser_OUTPUTS} ${FLEX_llvm-lexer_OUTPUTS})

target_include_directories(llvm PRIVATE ${LLVM_INCLUDE_DIRS})

target_link_libraries(llvm PRIVATE sfml-graphics)
target_link_libraries(llvm PRIVATE LLVM)

target_compile_features(llvm PRIVATE cxx_std_17)

target_include_directories(llvm PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(llvm PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
