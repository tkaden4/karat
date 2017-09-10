cmake_minimum_required(VERSION 2.8)

project(karat)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/out)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB KARAT_SOURCES ${PROJECT_SOURCE_DIR}/src/*.c)

add_executable(karat ${KARAT_SOURCES})
