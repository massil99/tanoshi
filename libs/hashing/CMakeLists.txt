add_library(hashing STATIC
    src/hash.c
)

target_include_directories(hashing
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)
