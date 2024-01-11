add_compile_definitions(_POSIX_C_SOURCE=200809L)

add_executable(
    rcsh

    main.c
)

target_include_directories(
    rcsh
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(rcsh_cmd)
add_subdirectory(rcsh_ctx)
add_subdirectory(rcsh_log)
add_subdirectory(rcsh_str)

target_link_libraries(rcsh PRIVATE rcsh_cmd)
target_link_libraries(rcsh PRIVATE rcsh_ctx)
target_link_libraries(rcsh PRIVATE rcsh_log)

install(TARGETS rcsh DESTINATION bin)