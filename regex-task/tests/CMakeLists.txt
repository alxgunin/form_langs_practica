cmake_minimum_required(VERSION 3.10)

project(all-tests)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include(CodeCoverage)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(all_tests run_tests.cpp TestBase.cpp ../src/slib.cpp ../src/ioputter.cpp)
target_include_directories(all_tests PUBLIC ../include)
target_link_libraries(all_tests ${GTEST_LIBRARIES} Threads::Threads)

setup_target_for_coverage_lcov(
    NAME nfa_coverage
    EXECUTABLE ${practica_SOURCE_DIR}/bin/all_tests
    BASE_DIRECTORY ${practica_SOURCE_DIR}
)

SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage -fsanitize=address,undefined")