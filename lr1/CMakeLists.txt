cmake_minimum_required(VERSION 3.14)
project(BracketTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Скачиваем Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Указываем путь к include
include_directories(${PROJECT_SOURCE_DIR}/include)

# Собираем исполняемый файл для тестов
add_executable(runTests
    src/oop_lr1.cpp
    tests/test.cpp
)

target_link_libraries(runTests gtest_main)
