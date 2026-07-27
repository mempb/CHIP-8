cmake_minimum_required(VERSION 3.16)
project(CHIP-8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(CHIP-8
    src/main.cpp
    src/chip8screen.cpp
    src/mainwindow.cpp
    src/chip8.c
)

target_link_libraries(CHIP-8 PRIVATE Qt6::Widgets)