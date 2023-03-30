cmake_minimum_required(VERSION 3.0.0)

project(aos-soa)

set(TRACY_ENABLE ON)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} benchmark::benchmark)
target_link_libraries(${PROJECT_NAME} Tracy::TracyClient)