cmake_minimum_required(VERSION 3.16)
project(raycaster)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(raycaster main.cpp)

target_link_libraries(raycaster sfml-graphics sfml-window sfml-system)
