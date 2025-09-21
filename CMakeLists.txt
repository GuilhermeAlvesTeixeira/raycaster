cmake_minimum_required(VERSION 3.25)
project(raycaster)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SFML 2.5 COMPONENTS graphics window system audio network REQUIRED)

find_package(OpenGL REQUIRED)

file(GLOB IMGUI_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui/*.cpp"
)

file(GLOB IMGUI_SFML_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/imgui-sfml/*.cpp"
)

set(SOURCES
        main.cpp
        map.cpp
        player.cpp
        renderer.cpp
        editor.cpp
        ${IMGUI_SOURCES}
        ${IMGUI_SFML_SOURCES}
        resources.cpp
        resources.h
)

add_executable(raycaster ${SOURCES})

target_include_directories(raycaster PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/imgui-sfml
)

target_link_libraries(raycaster PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        sfml-network
        ${OPENGL_LIBRARIES}
)

target_compile_definitions(raycaster PRIVATE SFML_STATIC)
