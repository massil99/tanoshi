cmake_minimum_required(VERSION 3.3)

set(PROJECT_NAME "game_engine")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \
    -g \
    -Wall \
    -Werror \
")

project(${PROJECT_NAME} C)

file(GLOB
    SOURCE_FILES
    src/*
)

add_executable(
    ${PROJECT_NAME}
    ${SOURCE_FILES}
)

