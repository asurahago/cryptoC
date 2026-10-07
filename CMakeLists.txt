cmake_minimum_required(VERSION 3.20)
project(cryptoC LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(cryptoC
	src/main.cpp
)
target_include_directories(cryptoC PRIVATE include)
target_compile_options(cryptoC PRIVATE -Wall -Wextra -Wpedantic)

