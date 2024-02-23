cmake_minimum_required(VERSION 3.3)

set(PROJECT_NAME "tanoshi")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

project(${PROJECT_NAME} C)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    -g \
    -Wall \
    -Werror \
")

file(GLOB
    SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/managers/*
    ${CMAKE_SOURCE_DIR}/src/memory/*
)

if(TARGET_BUILD STREQUAL "debug")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
        -O0 \
    ")

    list(APPEND SOURCE_FILES 
        ${CMAKE_SOURCE_DIR}/src/main.c
    )

    add_executable(
        ${PROJECT_NAME}
        ${SOURCE_FILES}
    )
elseif(TARGET_BUILD STREQUAL "test")

    list(APPEND SOURCE_FILES 
        ${CMAKE_SOURCE_DIR}/test/test.c
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

