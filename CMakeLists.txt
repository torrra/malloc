cmake_minimum_required(VERSION 3.22.1)

project(malloc)

set(SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(EXE_NAME mallocTest)

file(
    GLOB_RECURSE PROJECT_FILES
    ${INCLUDE_DIR}/*.h
    ${SOURCE_DIR}/*.c
    )


# Enable all compiler warnings
add_compile_options(-g -Wall -Wextra -Wshadow -Wpedantic) 

add_executable(EXE_NAME)

target_sources(EXE_NAME PRIVATE ${PROJECT_FILES})

target_include_directories(EXE_NAME PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)