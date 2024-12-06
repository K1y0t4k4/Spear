cmake_minimum_required(VERSION 3.5)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static-libgcc")

project(agent)
aux_source_directory(./src SRC_DIR)
include_directories(
    ./inc
    ./dependency
)
add_executable(${PROJECT_NAME} ${SRC_DIR})