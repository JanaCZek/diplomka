cmake_minimum_required(VERSION 3.0.0)

project(practice)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)

add_subdirectory(array-benchmark)
add_subdirectory(matrix-benchmark)
add_subdirectory(particles-benchmark)
add_subdirectory(update-benchmark)

add_subdirectory("libs\\benchmark" "benchmark")
add_subdirectory("libs\\googletest" "googletest")