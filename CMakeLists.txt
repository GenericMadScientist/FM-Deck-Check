cmake_minimum_required(VERSION 3.13)

project(FmStarterDeckChecker
    VERSION 0.1
    DESCRIPTION "A command-line tool to check FM starter decks"
    LANGUAGES CXX)

add_executable(deck_check src/main.cpp src/square.cpp)

include(CTest)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
