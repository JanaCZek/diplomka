cmake_minimum_required(VERSION 3.0.0)

project(array-index-patterns)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} benchmark::benchmark)