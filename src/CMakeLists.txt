add_executable(
    rcsh

    main.c
)

target_include_directories(
    rcsh
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_subdirectory(rcsh_builtin)
add_subdirectory(rcsh_cmd)
add_subdirectory(rcsh_ctx)
add_subdirectory(rcsh_log)
add_subdirectory(rcsh_run)
add_subdirectory(rcsh_str)

target_link_libraries(rcsh PRIVATE rcsh_cmd)
target_link_libraries(rcsh PRIVATE rcsh_ctx)
target_link_libraries(rcsh PRIVATE rcsh_log)
target_link_libraries(rcsh PRIVATE rcsh_run)

install(TARGETS rcsh DESTINATION bin)
