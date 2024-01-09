add_executable(
    rcsh
    main.c cli_args.c shell_loop.c
)

target_include_directories(rcsh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

add_subdirectory(log)
add_subdirectory(command)
add_subdirectory(builtin)
add_subdirectory(context)

target_link_libraries(rcsh PRIVATE rcsh_builtin)
target_link_libraries(rcsh PRIVATE rcsh_command)
target_link_libraries(rcsh PRIVATE rcsh_log)
target_link_libraries(rcsh PRIVATE rcsh_shell_ctx)

install(TARGETS rcsh DESTINATION bin)