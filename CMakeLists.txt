cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(epoll_demo)
add_executable(${PROJECT_NAME} "main.cpp")
