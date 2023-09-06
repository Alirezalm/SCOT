cmake_minimum_required(VERSION 3.22)

file(GLOB SCOT_MODEL "*.cpp")

add_library(model SHARED ${SCOT_MODEL})

target_include_directories(model PRIVATE
        "${CMAKE_SOURCE_DIR}/DOSE"
        "$ENV{GUROBI_HOME}/include"
        "${CMAKE_SOURCE_DIR}/ExternLibs/spdlog"
)