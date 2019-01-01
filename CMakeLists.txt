cmake_minimum_required(VERSION 3.13)

project(FmStarterDeckChecker
    VERSION 0.1
    DESCRIPTION "A command-line tool to check FM starter decks"
    LANGUAGES CXX)

# Set to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set warning flags
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

find_package(OpenMP REQUIRED)
add_subdirectory("extern/cxxopts")
add_executable(deck_check src/main.cpp src/options.cpp src/rng.cpp
    src/starter_deck.cpp)
target_link_libraries(deck_check PRIVATE OpenMP::OpenMP_CXX cxxopts)

include(CTest)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
