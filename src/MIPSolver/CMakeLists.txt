cmake_minimum_required(VERSION 3.22)

file(GLOB SCOT_MIP_SOLVER "*.cpp")

add_library(mip SHARED ${SCOT_MIP_SOLVER})

target_include_directories(mip PRIVATE
        "${CMAKE_SOURCE_DIR}/DOSE"
        "$ENV{GUROBI_HOME}/include"
        "${CMAKE_SOURCE_DIR}/ExternLibs/spdlog"
)
target_link_directories(mip PUBLIC "$ENV{GUROBI_HOME}/lib")

target_link_libraries(mip gurobi_c++)
target_link_libraries(mip gurobi100)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

file(GLOB INC "*.h")

install(TARGETS mip DESTINATION lib )

install(FILES ${INC} DESTINATION include )