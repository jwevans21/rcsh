add_library(
    rcsh_run
    rcsh_run.c rcsh_run_external.c
)

target_include_directories(
    rcsh_run
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    rcsh_run
    PRIVATE
    rcsh_builtin
)
target_link_libraries(
    rcsh_run
    PRIVATE
    rcsh_cmd
)
target_link_libraries(
    rcsh_run
    PRIVATE
    rcsh_ctx
)
target_link_libraries(
    rcsh_run
    PRIVATE
    rcsh_log
)
