include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tanoshi_test
    collections_test.cpp
    main_test.cpp
)

target_include_directories(tanoshi_test
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/external/clogger
)

target_link_libraries(tanoshi_test
    PUBLIC
        GTest::gtest_main
    PRIVATE
        collections
        memory
        managers
        hashing
)

include(GoogleTest)
gtest_discover_tests(tanoshi_test)
