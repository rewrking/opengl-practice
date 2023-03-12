cmake_minimum_required(VERSION 3.16)

project(glad-s VERSION 1.0.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(TARGET_NAME glad-s)
file(GLOB_RECURSE SOURCES src/*.c)

add_library(${TARGET_NAME} STATIC ${SOURCES})

target_include_directories(${TARGET_NAME} PRIVATE src/ include/)
