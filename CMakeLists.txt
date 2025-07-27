cmake_minimum_required(VERSION 3.16)
project(wallpaper LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(wallpaper main.cpp)

target_link_libraries(wallpaper
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    X11
)
