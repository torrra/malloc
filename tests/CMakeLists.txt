cmake_minimum_required(VERSION 3.22)

set(EXE_NAME mallocTest)

file(GLOB_RECURSE PROJECT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)


if (LINUX)

    add_executable(${EXE_NAME})

    # Enable all compiler warnings
    target_compile_options(${EXE_NAME} PRIVATE -g -Wall -Wextra -Wshadow -Wpedantic) 

    target_sources(${EXE_NAME} PRIVATE ${PROJECT_FILES})

    target_include_directories(${EXE_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    target_link_libraries(${EXE_NAME} PRIVATE ${ALLOCATOR_LIB})

endif()