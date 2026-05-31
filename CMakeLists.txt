cmake_minimum_required(VERSION 3.14)
project(Papildoma)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(main main.cpp)

add_executable(testai testai.cpp)

target_link_libraries(testai
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testai)