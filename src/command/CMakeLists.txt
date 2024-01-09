add_library(
    rcsh_command
    creation.c parse.c run.c external.c
)

target_include_directories(
    rcsh_command
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    rcsh_command
    rcsh_log
)
target_link_libraries(
    rcsh_command
    rcsh_builtin
)