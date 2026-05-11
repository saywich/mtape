include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    mtapelib-tests
    MTapeSorterTests.cpp
)

target_link_libraries(
    mtapelib-tests
    mtapelib
    GTest::gtest_main
)

target_include_directories(mtapelib-tests PUBLIC ${PROJECT_SOURCE_DIR}/src/include)

include(GoogleTest)

gtest_discover_tests(mtapelib-tests)
