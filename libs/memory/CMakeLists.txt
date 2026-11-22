add_library(memory STATIC
    src/memory-allocator.c
)

target_include_directories(memory
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/external/clogger
)

target_link_libraries(memory
    PUBLIC managers
)
