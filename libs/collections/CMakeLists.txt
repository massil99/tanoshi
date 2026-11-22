add_library(collections STATIC
    src/collections.c
)

target_include_directories(collections
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/external/clogger
)

target_link_libraries(collections
    PUBLIC managers
)
