set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/googletest" "extern/googletest")

macro(package_add_test TESTNAME)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} PRIVATE gtest gmock gtest_main)
    add_test(NAME ${TESTNAME} COMMAND ${TESTNAME})
    set_target_properties(${TESTNAME} PROPERTIES FOLDER tests)
endmacro()

set(SRC "${PROJECT_SOURCE_DIR}/src")

package_add_test(rng_unittest rng_unittest.cpp "${SRC}/rng.cpp")
target_include_directories(rng_unittest PRIVATE "${SRC}")

package_add_test(deck_filter_unittest
    deck_filter_unittest.cpp
    "${SRC}/rng.cpp"
    "${SRC}/deck_filter.cpp")
target_link_libraries(deck_filter_unittest PRIVATE OpenMP::OpenMP_CXX)
target_include_directories(deck_filter_unittest PRIVATE "${SRC}")
