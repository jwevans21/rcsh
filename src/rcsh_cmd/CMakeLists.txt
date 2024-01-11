add_library(
    rcsh_cmd
    rcsh_cmd_args.c rcsh_cmd_create.c rcsh_cmd_from.c rcsh_cmd_parse.c
)
target_include_directories(
    rcsh_cmd
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    rcsh_cmd
    PRIVATE
    rcsh_log
)

target_link_libraries(
    rcsh_cmd
    PRIVATE
    rcsh_str
)
