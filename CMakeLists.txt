cmake_minimum_required(VERSION 3.20)
project(cryptoC LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

add_executable(cryptoC
	src/main.cpp
	src/crypto/hash.cpp
	src/core/block.cpp
	src/core/chain.cpp 
)
target_include_directories(cryptoC PRIVATE include ${SODIUM_INCLUDE_DIRS})
target_link_libraries(cryptoC PRIVATE ${SODIUM_LIBRARIES})
target_compile_options(cryptoC PRIVATE -Wall -Wextra -Wpedantic)

