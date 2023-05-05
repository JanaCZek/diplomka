cmake_minimum_required(VERSION 3.0.0)

project(false-sharing)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} benchmark::benchmark)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)
endif()