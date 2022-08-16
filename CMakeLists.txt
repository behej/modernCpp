cmake_minimum_required(VERSION 3.5)

project(modernCpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(hello main.cpp)
add_executable(constructors constructors.cpp)
