cmake_minimum_required(VERSION 3.14)
project(ThreadMarker)

set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0600)
endif()

add_executable(ThreadMarker 
    main.cpp 
    function.cpp
)

if(WIN32)
    target_link_libraries(ThreadMarker kernel32 user32 gdi32 winspool shell32 ole32 oleaut32 uuid comdlg32 advapi32)
endif()

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

if(WIN32)
    target_link_libraries(ThreadMarkerTests 
        GTest::gtest 
        GTest::gtest_main 
        kernel32 user32 gdi32 winspool shell32 ole32 oleaut32 uuid comdlg32 advapi32)
else()
    target_link_libraries(ThreadMarkerTests 
        GTest::gtest 
        GTest::gtest_main 
        pthread)
endif()

add_test(NAME ThreadMarkerTests COMMAND ThreadMarkerTests)