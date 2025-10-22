cmake_minimum_required(VERSION 3.14)
project(BracketChecker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Google Test
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG main
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Включаем директорию с заголовками
include_directories(${PROJECT_SOURCE_DIR}/include)

# Исполняемый файл для тестов
add_executable(runTests
    src/oop_lr3.cpp
    tests/test.cpp
)
target_link_libraries(runTests gtest_main)

# Исполняемый файл для ручного запуска
add_executable(mainApp
    src/oop_lr3.cpp
    main.cpp
)
