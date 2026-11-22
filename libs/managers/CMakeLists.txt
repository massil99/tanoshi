add_library(managers STATIC
    src/config.c
    src/log-manager.c
    src/master-manager.c
)

target_include_directories(managers
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/external/clogger
)

target_link_libraries(managers
    PUBLIC memory
    PUBLIC hashing
)
