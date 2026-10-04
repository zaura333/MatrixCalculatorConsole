cmake_minimum_required(VERSION 3.20)
project(MatrixCalculatorConsole LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)          # -std=c++17 zamiast gnu++17

# Biblioteka: Matrix.h jest nag³ówkowy, wiêc wchodzi tylko plik z wyj¹tkami
add_library(matrix STATIC MatrixCalculatorConsole/MatrixExceptions.cpp)
target_include_directories(matrix PUBLIC MatrixCalculatorConsole)

if(MSVC)
    target_compile_options(matrix PUBLIC /W4 /permissive- /utf-8)
else()
    target_compile_options(matrix PUBLIC -Wall -Wextra -Wpedantic)
endif()

add_executable(MatrixCalculatorConsole MatrixCalculatorConsole/MatrixCalculatorConsole.cpp)
target_link_libraries(MatrixCalculatorConsole PRIVATE matrix)
