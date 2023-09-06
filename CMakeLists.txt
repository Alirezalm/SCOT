cmake_minimum_required(VERSION 3.22)

project(scot VERSION 1.0.0 DESCRIPTION "Distributed Sparse Convex Optimization Toolkit")

find_package(MPI REQUIRED)

set(CMAKE_CXX_FLAGS "-march=native -O2")

set(CMAKE_CXX_COMPILER mpicxx)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(DOSE)

add_subdirectory(src/Algorithm)

add_subdirectory(src/Tasks)

add_executable(${PROJECT_NAME}
        src/scot.cpp
        src/Model/IObjective.h
        src/Solver/Structs.h
        src/Model/LogRegObjectiveFunction.cpp
        src/Model/LogRegObjectiveFunction.h
        src/Model/LinRegObjectiveFunction.cpp
        src/Model/LinRegObjectiveFunction.h
        src/Model/SparsityConstraint.cpp
        src/Model/SparsityConstraint.h
        src/Solver/Enums.h
        src/Model/Model.h
        src/Model/Model.cpp
        src/Solver/Environment.h
        src/PrimalSolver/InlpSolver.h
        src/PrimalSolver/DistributedOperatorSplittingEngine.cpp
        src/PrimalSolver/DistributedOperatorSplittingEngine.h
        src/Solver/ScotSolver.cpp
        src/Solver/ScotSolver.h
        src/Solver/Iteration.cpp
        src/Solver/Iteration.h
        src/Solver/Results.cpp
        src/Solver/Results.h
        src/MIPSolver/IMIPSolver.h
        src/MIPSolver/MipSolverGurobiMultipleTree.cpp
        src/MIPSolver/MipSolverGurobiMultipleTree.h
        src/Solver/Utils.cpp src/Solver/Utils.h
        src/Solver/MessagePassingInterface.cpp
        src/Solver/MessagePassingInterface.h
        src/Solver/Logger.cpp
        src/Solver/Logger.h
        src/Solver/Report.cpp
        src/Solver/Report.h
        src/MIPSolver/MipSolverGurobiSingleTree.cpp
        src/MIPSolver/MipSolverGurobiSingleTree.h
        src/Solver/settings.h
        src/Solver/Timer.cpp
        src/Solver/Timer.h)

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
target_link_libraries(${PROJECT_NAME}  ${MPI_CXX_LIBRARIES})
target_link_libraries(${PROJECT_NAME} spdlog.a)
target_link_libraries(${PROJECT_NAME}  fmt.a)
target_link_libraries(${PROJECT_NAME} gurobi_c++)
target_link_libraries(${PROJECT_NAME} gurobi100)