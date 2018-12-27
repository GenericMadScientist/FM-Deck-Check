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

# Set optimisation flags
if(MSVC)
    add_compile_options(/O2)
else()
    add_compile_options(-O3)
endif()

add_executable(deck_check src/main.cpp src/starter_deck.cpp)

include(CTest)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
