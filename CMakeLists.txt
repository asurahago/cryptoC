cmake_minimum_required(VERSION 3.20)
project(cryptoC LANGUAGES CXX VERSION 0.3.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)

include(FetchContent)
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG        v5.0.0
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(ftxui)

add_executable(cryptoC
    src/main.cpp
    src/crypto/hash.cpp
    src/core/block.cpp
    src/core/chain.cpp
    src/core/wallet.cpp
    src/core/mempool.cpp
)
target_include_directories(cryptoC PRIVATE include ${SODIUM_INCLUDE_DIRS})
target_link_libraries(cryptoC PRIVATE
    ${SODIUM_LIBRARIES}
    ftxui::screen
    ftxui::dom
    ftxui::component
)
target_compile_options(cryptoC PRIVATE -Wall -Wextra -Wpedantic)