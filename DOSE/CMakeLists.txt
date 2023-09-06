cmake_minimum_required(VERSION 3.22)

file(GLOB DOSE_SRC "src/*.cpp")

add_library(dose SHARED ${DOSE_SRC})