cmake_minimum_required(VERSION 3.0.0)

project(particles-benchmark)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF)

add_subdirectory("D:\\Projekty\\diplomka\\libs\\benchmark" "benchmark")
add_subdirectory("D:\\Projekty\\diplomka\\libs\\googletest" "googletest")

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} benchmark::benchmark)
target_link_libraries(${PROJECT_NAME} GTest::gtest_main)