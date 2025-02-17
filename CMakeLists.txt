cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "tanoshi")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

message("BUILD TYPE = ${CMAKE_BUILD_TYPE}")
message("BUILD TYPE = ${TARGET_BUILD}")
message("BUILD TYPE = ${TEST_TARGET}")
project(${PROJECT_NAME} C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    -Wall \
    -Werror \
")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} \
    -g\
")

file(GLOB
    SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/managers/*
    ${CMAKE_SOURCE_DIR}/src/memory/*
    ${CMAKE_SOURCE_DIR}/src/collections.c
    ${CMAKE_SOURCE_DIR}/src/hash.c
)

if(TARGET_BUILD STREQUAL "debug")
    list(APPEND SOURCE_FILES 
        ${CMAKE_SOURCE_DIR}/src/main.c
    )

    add_executable(
        ${PROJECT_NAME}
        ${SOURCE_FILES}
    )

    target_link_libraries(
        ${PROJECT_NAME}
    )

elseif(TARGET_BUILD STREQUAL "test")
    list(APPEND SOURCE_FILES 
        ${CMAKE_SOURCE_DIR}/test/${TEST_TARGET}.c
    )

    include(FetchContent)
    FetchContent_Declare(
        Tau
        URL https://github.com/jasmcaus/tau/archive/dev.zip
    )
    
    FetchContent_MakeAvailable(Tau)
    enable_testing()

    add_executable(
        "${PROJECT_NAME}_test"
        ${SOURCE_FILES}
    )
    
    target_link_libraries(
        "${PROJECT_NAME}_test"
        Tau
    )
endif()
