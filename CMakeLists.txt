cmake_minimum_required(VERSION 3.22.1)

project(directories)

set(SOURCE_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(
    GLOB_RECURSE PROJECT_FILES
    ${INCLUDE_DIR}/*.h
    ${SOURCE_DIR}/*.c
    )
# Enable all compiler warnings
add_compile_options(-g -Wall -Wextra -Wshadow -Wpedantic) # -Wpedantic to display errors caused by minor mistakes in code

add_executable(exe)

target_sources(exe PRIVATE ${PROJECT_FILES})

target_include_directories(exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)