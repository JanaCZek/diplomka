cmake_minimum_required(VERSION 3.0.0)

project(array-benchmark)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)

add_subdirectory("D:\\benchmark" "benchmark")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} benchmark::benchmark)