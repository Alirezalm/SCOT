cmake_minimum_required(VERSION 3.22)

file(GLOB DOSE_SRC "src/*.cpp")

add_library(dose SHARED ${DOSE_SRC})

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

file(GLOB INC "include/*.h")
install(TARGETS dose DESTINATION lib )

#install(FILES ${INC} DESTINATION DOSE/include )