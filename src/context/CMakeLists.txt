add_library(
    rcsh_shell_ctx
    shell_ctx.c
)

target_include_directories(
    rcsh_shell_ctx
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)