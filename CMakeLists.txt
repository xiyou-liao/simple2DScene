cmake_minimum_required(VERSION 4.0)
project(simple2DScene)

set(CMAKE_CXX_STANDARD 23)

add_executable(simple2DScene main.cpp)

target_include_directories(simple2DScene PRIVATE
        C:/raylib/include
)

target_link_directories(simple2DScene PRIVATE
        C:/raylib/lib
)

target_link_libraries(simple2DScene PRIVATE
        raylib
        opengl32
        gdi32
        winmm
)