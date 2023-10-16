cmake_minimum_required(VERSION 3.10)

project(all-tests)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(all_tests run_tests.cpp TestBase.cpp ../src/slib.cpp ../src/ioputter.cpp)
target_include_directories(all_tests PUBLIC ../include)
target_link_libraries(all_tests ${GTEST_LIBRARIES} Threads::Threads)

include(CodeCoverage)
include(${practica_SOURCE_DIR}/cmake/CodeCoverage.cmake)
SETUP_TARGET_FOR_COVERAGE(
    test_coverage
    ../bin/all_tests
    results
)
SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
