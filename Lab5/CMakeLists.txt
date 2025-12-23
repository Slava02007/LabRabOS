cmake_minimum_required(VERSION 3.10)
project(LabWork)

set(CMAKE_CXX_STANDARD 17)

# Сервер
add_executable(Server Server.cpp)

# Клиент
add_executable(Client Client.cpp)

# Указываем, что для Windows нужно линковать соответствующие библиотеки
if(WIN32)
    target_link_libraries(Server)
    target_link_libraries(Client)
endif()