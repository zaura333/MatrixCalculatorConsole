cmake_minimum_required(VERSION 3.20)
project(MatrixCalculatorConsole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)          # -std=c++17 instead of gnu++17

# Sanitizers (AddressSanitizer + UBSan) - UBSan is GCC/Clang only:
#   cmake -S . -B build-san -DENABLE_SANITIZERS=ON
option(ENABLE_SANITIZERS "Build with AddressSanitizer and UBSan (GCC/Clang only)" OFF)
if(ENABLE_SANITIZERS)
    if(MSVC)
        message(WARNING "ENABLE_SANITIZERS: UBSan is not available in MSVC, option ignored. Use GCC/Clang (Linux, WSL).")
    else()
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address,undefined)
    endif()
endif()

# Library: Matrix.h is header-only, so only the exceptions file is compiled
add_library(matrix STATIC MatrixCalculatorConsole/MatrixExceptions.cpp)
target_include_directories(matrix PUBLIC MatrixCalculatorConsole)

if(MSVC)
    target_compile_options(matrix PUBLIC /W4 /permissive- /utf-8)
else()
    target_compile_options(matrix PUBLIC -Wall -Wextra -Wpedantic)
endif()

add_executable(MatrixCalculatorConsole MatrixCalculatorConsole/MatrixCalculatorConsole.cpp)
target_link_libraries(MatrixCalculatorConsole PRIVATE matrix)

enable_testing()
add_subdirectory(tests)
