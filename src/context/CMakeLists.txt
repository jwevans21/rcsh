add_library(
    rcsh_shell_ctx
    shell_ctx.c
)

target_include_directories(
    rcsh_shell_ctx
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    rcsh_shell_ctx
    PUBLIC
    rcsh_log
)