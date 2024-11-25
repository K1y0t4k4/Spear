cmake_minimum_required(VERSION 3.5)
set(CMAKE_EXPORT_COMPILE_COMMANDS OFF)
project(agent)
aux_source_directory(./src SRC_DIR)
include_directories(
    ./inc
    ./dependency
)
add_compile_options(-std=c++20)
set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++ -static-libgcc")
add_executable(${PROJECT_NAME} ${SRC_DIR})