
cmake_minimum_required(VERSION 3.14)
project(Lab2.2 LANGUAGES CXX)

# Настройки компиляции
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Для MSVC включаем поддержку "горячей перезагрузки" (опционально)
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(arrayfunctions STATIC ArrayFunctions.cpp)
target_include_directories(arrayfunctions PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Lab2.2 main.cpp)
target_link_libraries(Lab2.2 PRIVATE arrayfunctions)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE arrayfunctions GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)
