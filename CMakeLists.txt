cmake_minimum_required(VERSION 3.20)
project(xl-formula VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Build options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_WEB_BINDINGS "Build Emscripten web bindings" OFF)
option(BUILD_RN_BINDINGS "Build React Native bindings" OFF)

# Include directories
include_directories(include)

# Core library
file(GLOB_RECURSE CORE_SOURCES 
    "cpp/core/*.cpp"
    "cpp/parser/*.cpp" 
    "cpp/engine/*.cpp"
    "cpp/functions/*.cpp"
)

file(GLOB_RECURSE CORE_HEADERS 
    "include/xl-formula/*.h"
    "include/xl-formula/*.hpp"
)

add_library(xl-formula STATIC ${CORE_SOURCES} ${CORE_HEADERS})
target_include_directories(xl-formula PUBLIC include)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Web bindings
if(BUILD_WEB_BINDINGS)
    add_subdirectory(bindings/web)
endif()

# React Native bindings
if(BUILD_RN_BINDINGS)
    add_subdirectory(bindings/react-native)
endif()

# Install targets
install(TARGETS xl-formula
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)