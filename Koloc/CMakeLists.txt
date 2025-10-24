cmake_minimum_required(VERSION 3.16)
project(KolocProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к исходникам (если все файлы рядом с CMakeLists.txt)
set(SOURCES
    main.cpp
    Koloc.cpp
)

# ------------------------------
# Основное приложение
# ------------------------------
add_executable(KolocApp ${SOURCES})
target_include_directories(KolocApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ------------------------------
# GoogleTest (через FetchContent)
# ------------------------------
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ------------------------------
# Unit-тесты
# ------------------------------
add_executable(KolocTests tests.cpp Koloc.cpp)
target_include_directories(KolocTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(KolocTests GTest::gtest GTest::gtest_main)

include(GoogleTest)
enable_testing()
gtest_discover_tests(KolocTests)

# ------------------------------
# Красивый вывод бинарников
# ------------------------------
set_target_properties(KolocApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(KolocTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
