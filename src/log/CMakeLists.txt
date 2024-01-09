add_library(
    rcsh_log
    log.c
)

target_include_directories(
    rcsh_log
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)