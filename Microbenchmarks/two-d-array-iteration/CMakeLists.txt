cmake_minimum_required(VERSION 3.0.0)

project(two-d-array-iteration)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} benchmark::benchmark)
target_link_libraries(${PROJECT_NAME} Tracy::TracyClient)