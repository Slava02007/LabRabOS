cmake_minimum_required(VERSION 3.14)
project(CMake_Lab2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Основная программа ---
add_executable(CMake_Lab2 "tests.cpp" "CMake_Lab2.cpp" "CMake_Lab2.h")

# --- GoogleTest ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# --- Тесты ---
add_executable(tests "tests.cpp" "CMake_Lab2.cpp" "CMake_Lab2.h")
target_link_libraries(tests PRIVATE GTest::gtest_main PRIVATE Kernel32.lib)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(tests)
