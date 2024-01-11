add_library(
    rcsh_str
    rcsh_str.c
)
target_include_directories(
    rcsh_str
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# target_link_libraries(
#     rcsh_str
#     PUBLIC
#     rcsh_log
# )