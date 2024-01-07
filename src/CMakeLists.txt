add_executable(
    rcsh
    main.c command.c builtin.c external.c
)

target_include_directories(rcsh PUBLIC include/)

install(TARGETS rcsh DESTINATION bin)