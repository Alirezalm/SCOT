cmake_minimum_required(VERSION 3.22)

file(GLOB SCOT_TASKS "*.cpp")

add_library(tasks SHARED ${SCOT_TASKS})

target_include_directories(tasks PRIVATE
        "${CMAKE_SOURCE_DIR}/DOSE"
        "$ENV{GUROBI_HOME}/include"
        "${CMAKE_SOURCE_DIR}/ExternLibs/spdlog")

target_link_directories(tasks PUBLIC "${CMAKE_SOURCE_DIR}/MIPSolver")

target_link_libraries(tasks PUBLIC mip)