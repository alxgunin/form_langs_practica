cmake_minimum_required(VERSION 3.20)

project(all-tests)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include(CodeCoverage)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(all_tests run_tests.cpp TestBase.cpp ../src/slib.cpp ../src/ioputter.cpp)
target_include_directories(all_tests PUBLIC ../include)
target_link_libraries(all_tests ${GTEST_LIBRARIES} Threads::Threads)

setup_target_for_coverage(coverage_report all_tests coverage)
SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")