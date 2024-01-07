cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(
    rcsh
    VERSION 0.1.0
    LANGUAGES C
)

add_subdirectory(src)
