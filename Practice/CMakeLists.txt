cmake_minimum_required(VERSION 3.0.0)

project(practice)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(TRACY_ENABLE ON)
#set(CMAKE_CXX_FLAGS "/O2")
#set(CMAKE_CXX_FLAGS_DEBUG "/O2")
#set(CMAKE_CXX_FLAGS_RELEASE "/O2")

add_subdirectory(array-benchmark)
add_subdirectory(matrix-benchmark)
add_subdirectory(particles-benchmark)
add_subdirectory(update-benchmark)

add_subdirectory("libs\\benchmark" "benchmark")
add_subdirectory("libs\\tracy" "tracy")