cmake_minimum_required(VERSION 3.22)

set(LIB_NAME allocator)

file(GLOB_RECURSE PROJECT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)



if (LINUX)
# Enable all compiler warnings
    add_library(${LIB_NAME} STATIC)

    target_compile_options(${LIB_NAME} PRIVATE -g -Wall -Wextra -Wshadow -Wpedantic) 

    target_sources(${LIB_NAME} PRIVATE ${PROJECT_FILES})

    target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

    set(ALLOCATOR_LIB ${LIB_NAME} PARENT_SCOPE)


else()

    message("[allocator lib] Platform not supported")

endif()

