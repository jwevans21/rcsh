add_library(
    rcsh_builtin
    rcsh_exit.c rcsh_status.c rcsh_cd.c
)

target_include_directories(
    rcsh_builtin
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    rcsh_builtin
    PRIVATE
    rcsh_cmd
)
target_link_libraries(
    rcsh_builtin
    PRIVATE
    rcsh_ctx
)
target_link_libraries(
    rcsh_builtin
    PRIVATE
    rcsh_log
)