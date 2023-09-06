cmake_minimum_required(VERSION 3.22)

file(GLOB SCOT_PRIMAL "*.cpp")

add_library(primal SHARED ${SCOT_PRIMAL})

target_include_directories(primal PRIVATE
        "${CMAKE_SOURCE_DIR}/DOSE"
)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

file(GLOB INC "*.h")

install(TARGETS primal DESTINATION lib )

install(FILES ${INC} DESTINATION include )