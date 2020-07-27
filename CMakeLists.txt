cmake_minimum_required(VERSION 3.5)

project(orb LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(client
        client.cpp
        interface.hh
        interfaceSK.cc)


target_link_libraries(client
        -lomnithread
        -lomniORB4)


add_executable(server
        server.cpp
        interface.hh
        interfaceSK.cc)


target_link_libraries(server
        -lomnithread
        -lomniORB4)