cmake_minimum_required(VERSION 3.14)
project(ThreadMarker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Основная программа
add_executable(ThreadMarker 
    main.cpp 
    function.cpp
)

# Подключаем потоки (для std::thread)
find_package(Threads REQUIRED)
target_link_libraries(ThreadMarker PRIVATE Threads::Threads)

# Тесты
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(ThreadMarkerTests 
    tests.cpp 
    function.cpp
)

target_link_libraries(ThreadMarkerTests 
    GTest::gtest 
    GTest::gtest_main 
    Threads::Threads
)

add_test(NAME ThreadMarkerTests COMMAND ThreadMarkerTests)
