cmake_minimum_required(VERSION 3.10)

project(practica
  LANGUAGES CXX
)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${practica_SOURCE_DIR}/bin)

set(CMAKE_CXX_STANDARD 20)
add_executable(main main.cpp src/slib.cpp src/ioputter.cpp)
target_include_directories(main PUBLIC include/)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include(CodeCoverage)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})

add_subdirectory(tests)
enable_testing()
add_test(TestPart1 {PROJECT_SOURCE_DIR}/bin/all_tests)