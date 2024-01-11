cmake_minimum_required(VERSION 3.14)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS FALSE)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

add_compile_definitions(_POSIX_C_SOURCE=200809L)
add_compile_definitions(_FORTIFY_SOURCE=2)

add_compile_options(-Wall -Wextra -Wpedantic)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(
    rcsh
    VERSION 0.1.0
    LANGUAGES C
)

add_subdirectory(src)
