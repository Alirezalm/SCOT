cmake_minimum_required(VERSION 3.22)

file(GLOB SCOT_ALGORITHMS "*.cpp")

add_library(algorithms SHARED ${SCOT_ALGORITHMS})

target_include_directories(algorithms PRIVATE "${CMAKE_SOURCE_DIR}/DOSE")
target_include_directories(algorithms PRIVATE "${CMAKE_SOURCE_DIR}/ExternLibs/spdlog")