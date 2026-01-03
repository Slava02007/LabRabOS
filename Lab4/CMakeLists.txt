cmake_minimum_required(VERSION 3.10)
project(MessageQueue)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Receiver Receiver.cpp)
add_executable(Sender Sender.cpp)
add_executable(Tests tests.cpp)
