cmake_minimum_required(VERSION 3.20)
project(velox VERSION 1.0.0 LANGUAGES CXX)

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
option(BUILD_FORMULAS "Build Velox Formulas library" ON)
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_WEB_BINDINGS "Build Emscripten web bindings" OFF)
option(BUILD_RN_BINDINGS "Build React Native bindings" OFF)

# Add formulas library
if(BUILD_FORMULAS)
    add_subdirectory(cpp/libraries/formulas)
endif()

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
    add_subdirectory(cpp/bindings/web)
endif()

# React Native bindings
if(BUILD_RN_BINDINGS)
    add_subdirectory(cpp/bindings/react-native)
endif()