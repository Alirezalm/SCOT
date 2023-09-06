cmake_minimum_required(VERSION 3.22)

file(GLOB SCOT_SOLVER "*.cpp")

add_library(solver SHARED ${SCOT_SOLVER})

target_include_directories(solver PRIVATE
        "${CMAKE_SOURCE_DIR}/DOSE"
        "$ENV{GUROBI_HOME}/include"
        "${CMAKE_SOURCE_DIR}/ExternLibs/spdlog"
)
target_link_directories(solver PUBLIC
        "${CMAKE_SOURCE_DIR}/Tasks"
        "${CMAKE_SOURCE_DIR}/Algorithms")

target_link_libraries(solver PUBLIC tasks)
target_link_libraries(solver PUBLIC algorithms)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
file(GLOB INC "*.h")

install(TARGETS solver DESTINATION lib )

install(FILES ${INC} DESTINATION include )