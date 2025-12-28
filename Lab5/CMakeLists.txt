cmake_minimum_required(VERSION 3.10)
project(LabWork)

set(CMAKE_CXX_STANDARD 17)

add_executable(Server Server.cpp)

add_executable(Client Client.cpp)

add_executable(Tests Tests.cpp)

if(WIN32)
    target_link_libraries(Server)
    target_link_libraries(Client)
endif()