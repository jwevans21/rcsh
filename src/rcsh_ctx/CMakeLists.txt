add_library(
    rcsh_ctx
    rcsh_ctx.c
)
target_include_directories(
    rcsh_ctx
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(
    rcsh_ctx
    PRIVATE
    rcsh_log
)
