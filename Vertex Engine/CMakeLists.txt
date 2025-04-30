cmake_minimum_required(VERSION 3.10)

project("Vertex Engine")


set(CMAKE_CXX_STANDARD 17)

add_executable("Vertex Engine" StartUp.cpp)

find_package(box2d CONFIG REQUIRED)
target_link_libraries("Vertex Engine" PRIVATE box2d::box2d)