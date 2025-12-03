cmake_minimum_required(VERSION 3.14)
project(oop_lr6 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(runTests
    src/oop_lr6.cpp
    tests/test.cpp
)
target_link_libraries(runTests gtest_main)
include(GoogleTest)
gtest_discover_tests(runTests)

add_executable(mainApp
    src/oop_lr6.cpp
    main.cpp
)
