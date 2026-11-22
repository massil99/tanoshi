cmake_minimum_required(VERSION 4.2)
project(Tanoshi 
    VERSION 1.0
    LANGUAGES C CXX
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY, ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS, "ON")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    -Wall \
    -Werror \
    -g \
")

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.c
)

add_subdirectory(libs/collections)
add_subdirectory(libs/memory)
add_subdirectory(libs/managers)
add_subdirectory(libs/hashing)
add_subdirectory(test)

add_executable(Tanoshi 
    ${SOURCES}
    apps/cli/src/main.c
)

target_link_libraries(Tanoshi
    PRIVATE
        collections
        memory
        managers
        hashing
        m
)

target_include_directories(Tanoshi
    PRIVATE
        external/clogger
        include/
        libs/collections/include
        libs/memory/include
        libs/managers/include
        libs/hashing/include
)

