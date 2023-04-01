cmake_minimum_required(VERSION 3.0.0)

project(loop-unrolling)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} benchmark::benchmark)