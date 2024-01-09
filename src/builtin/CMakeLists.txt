add_library(
    rcsh_builtin
    cd.c exit.c status.c
)

target_include_directories(
    rcsh_builtin
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    rcsh_builtin
    rcsh_log
)
target_link_libraries(
    rcsh_builtin
    rcsh_command
)