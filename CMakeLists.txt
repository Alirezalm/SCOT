cmake_minimum_required(VERSION 3.22)
project(scot VERSION 1.0.0 DESCRIPTION "Distributed Sparse Convex Optimization Toolkit")
find_package(MPI REQUIRED)
set(CMAKE_CXX_FLAGS "-march=native -O2")

set(CMAKE_CXX_COMPILER mpicxx)
set(CMAKE_CXX_STANDARD 17)

include_directories(DOSE)
include_directories(DOSE/include)
include_directories(ExternLibs/spdlog)
include_directories(/home/alireza/gurobi952/linux64/include)
link_directories(/home/alireza/gurobi952/linux64/lib ) #todo : must be changed
add_executable(${PROJECT_NAME}
        src/scot.cpp
        DOSE/include/DOSE.h
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
        src/Solver/MainSolver.cpp
        src/Solver/MainSolver.h
        src/Algorithm/IAlgorithm.h
        src/Algorithm/Dipoa.cpp
        src/Algorithm/Dipoa.h
        src/Tasks/TaskBase.h
        src/Solver/Iteration.cpp
        src/Solver/Iteration.h
        src/Solver/Results.cpp
        src/Solver/Results.h
        src/Tasks/TaskDistributedNlpSolution.cpp
        src/Tasks/TaskDistributedNlpSolution.h
        src/Tasks/TaskBase.cpp
        src/Tasks/TaskManager.cpp
        src/Tasks/TaskManager.h
        src/Tasks/TaskInitializeNewIteration.cpp
        src/Tasks/TaskInitializeNewIteration.h
        src/MIPSolver/IMIPSolver.h
        src/MIPSolver/MipSolverGurobiMultipleTree.cpp
        src/MIPSolver/MipSolverGurobiMultipleTree.h
        src/Tasks/TaskCreateMultipleTreeDualProblem.cpp
        src/Tasks/TaskCreateMultipleTreeDualProblem.h
        src/Tasks/TaskInitializeMultipleTreeDualSolver.cpp
        src/Tasks/TaskInitializeMultipleTreeDualSolver.h
        src/Tasks/TaskAddLinearOuterApproximation.cpp
        src/Tasks/TaskAddLinearOuterApproximation.h
        src/Solver/Utils.cpp src/Solver/Utils.h
        src/Tasks/TaskSolveDualProblem.cpp
        src/Tasks/TaskSolveDualProblem.h
        src/Tasks/TaskAddDualSolution.cpp
        src/Tasks/TaskAddDualSolution.h
        src/Solver/MessagePassingInterface.cpp
        src/Solver/MessagePassingInterface.h
        src/Tasks/TaskGatherLocalLinearOuterApproximations.cpp
        src/Tasks/TaskGatherLocalLinearOuterApproximations.h
        src/Tasks/TaskBcastDualSolution.cpp
        src/Tasks/TaskBcastDualSolution.h
        src/Solver/Logger.cpp
        src/Solver/Logger.h
        src/Solver/Report.cpp
        src/Solver/Report.h
        src/Tasks/TaskPrintIterationInfo.cpp
        src/Tasks/TaskPrintIterationInfo.h
        src/Tasks/TaskComputeObjectiveGap.cpp
        src/Tasks/TaskComputeObjectiveGap.h
        src/Tasks/TaskCheckTerminationGap.cpp
        src/Tasks/TaskCheckTerminationGap.h
        src/Tasks/TaskGatherLocalQuadraticOuterApproximations.cpp
        src/Tasks/TaskGatherLocalQuadraticOuterApproximations.h
        src/Tasks/TaskAddQuadraticOuterApproximation.cpp
        src/Tasks/TaskAddQuadraticOuterApproximation.h
        src/Tasks/TaskCheckSocEvent.cpp
        src/Tasks/TaskCheckSocEvent.h
        src/MIPSolver/MipSolverGurobiSingleTree.cpp
        src/MIPSolver/MipSolverGurobiSingleTree.h
        src/Algorithm/Dihoa.cpp
        src/Algorithm/Dihoa.h
        src/Tasks/TaskInitializeSingleTreeDualSolver.cpp
        src/Tasks/TaskInitializeSingleTreeDualSolver.h
        src/Tasks/TaskSolveSingleTreeDualProblem.cpp
        src/Tasks/TaskSolveSingleTreeDualProblem.h
        src/Tasks/TaskCheckHybridEvent.cpp
        src/Tasks/TaskCheckHybridEvent.h src/Solver/settings.h src/Solver/Timer.cpp src/Solver/Timer.h src/Tasks/TaskCheckDuration.cpp src/Tasks/TaskCheckDuration.h)

target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/DOSE/lib/libDOSE.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/ExternLibs/fmt/lib/libfmt.a)
target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/ExternLibs/spdlog/lib/libspdlog.a)
target_link_libraries(${PROJECT_NAME} gurobi_c++)
target_link_libraries(${PROJECT_NAME} gurobi95)