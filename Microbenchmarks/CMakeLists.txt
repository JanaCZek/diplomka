cmake_minimum_required(VERSION 3.0.0)

project(microbenchmarks)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)
set(TRACY_ENABLE ON)

add_subdirectory(two-d-array-iteration)
add_subdirectory(matrix-multiplication-variations)
add_subdirectory(loop-carried-dependence)
add_subdirectory(loop-unrolling)
add_subdirectory(cache-associativity-limit)
add_subdirectory(array-index-patterns)

add_subdirectory("libs\\benchmark" "benchmark")
add_subdirectory("libs\\tracy" "tracy")