cmake_minimum_required(VERSION 3.22)

project(scot VERSION 1.0.0 DESCRIPTION "Distributed Sparse Convex Optimization Toolkit")

find_package(MPI REQUIRED)

set(CMAKE_CXX_FLAGS "-O2")

set(CMAKE_CXX_COMPILER mpicxx)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(DOSE)

add_subdirectory(src/Algorithm)

add_subdirectory(src/Tasks)

add_subdirectory(src/MIPSolver)

add_subdirectory(src/Model)

add_subdirectory(src/Solver)

add_subdirectory(src/PrimalSolver)

add_executable(${PROJECT_NAME}
        src/scot.cpp
        src/cli_utils.cpp
        src/cli_utils.h
)

target_include_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/DOSE"
        "${PROJECT_SOURCE_DIR}/ExternLibs"
        "${PROJECT_SOURCE_DIR}/ExternLibs/spdlog"
        "$ENV{GUROBI_HOME}/include")

target_link_directories(${PROJECT_NAME} PUBLIC
        "${PROJECT_SOURCE_DIR}/ExternLibs/spdlog/lib"
        "${PROJECT_SOURCE_DIR}/ExternLibs/fmt/lib"
        "$ENV{GUROBI_HOME}/lib"
        "${MPI_INCLUDE_PATH}")

target_link_libraries(${PROJECT_NAME} dose)
target_link_libraries(${PROJECT_NAME} algorithms)
target_link_libraries(${PROJECT_NAME} tasks)
target_link_libraries(${PROJECT_NAME} mip)
target_link_libraries(${PROJECT_NAME} model)
target_link_libraries(${PROJECT_NAME} solver)
target_link_libraries(${PROJECT_NAME} primal)

target_link_libraries(${PROJECT_NAME} spdlog.a)
target_link_libraries(${PROJECT_NAME}  fmt.a)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/scot_framework)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
install(TARGETS scot DESTINATION bin)