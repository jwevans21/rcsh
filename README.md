# rcsh (Reduced Complexity Shell)

This is a basic shell implementation that is written in the C programming
language.

## Features/TODO

- [X] Basic command parsing
- [X] Basic builtin commands
    - [X] `cd` Change the current directory
    - [X] `exit` Used to exit the shell
    - [X] `status` Used to get the exit status of the last foreground command
- [X] Basic foreground commands
- [ ] Add CLI arguments
    - [ ] `-c` Run a command and exit
    - [ ] `-h` Print help message and exit
    - [ ] `-v` Print version and exit
    - [ ] `-V` Verbose mode (used for debugging the shell)
- [ ] Basic input/output redirection
- [ ] Background commands
- [ ] Basic signal handling
- [ ] More complex command parsing
    - [ ] Quoted strings
    - [ ] Environment variable substitution
    - [ ] Environment variable creation
- [ ] Additional builtin commands
    - [ ] `jobs` List the background processes
    - [ ] `fg` Move background process to foreground
    - [ ] `exec` Replace the current process with the new command

## Usage

To compile the shell, run the following command:

```bash
cmake -S . -B build
cmake --build build
```

To run the shell, run the following command:

```bash
./build/src/rcsh
```

Or you can install the shell by running the following command:

```bash
cmake --install build --prefix [PREFIX]
```

## Builtin Commands

- [cd](#cd) - Change the current directory
- [exit](#exit) - Gracefully exit the shell
- [status](#status) - Get the exit status of the last foreground command
- [help](#help) - Print help message
- [jobs](#jobs) - List the background processes
- [fg](#fg) - Move background process to foreground
- [exec](#exec) - Replace the current process with the new command

### `cd`

The `cd` command is used to change the current working directory. It takes zero
or one argument. If no argument is given, the users home directory is used. If
an argument is given, the current working directory is changed to the given
path. When 2 or more arguments are given, it takes the first argument and
ignores the rest.

### `exit`

The `exit` command is used to exit the shell. It takes zero or one argument. If
no argument is given, the shell exits with the exit status of the last command
that was run. If an argument is given, the shell exits with the given exit code.
When 2 or more arguments are given, it takes the first argument and ignores the
rest.

### `status`

The `status` command is used to get the exit status of the last foreground
command. It takes zero arguments. If any arguments are given, they are ignored.

### `help`

TODO

### `jobs`

TODO

### `fg`

TODO

## License

This project is meant as a personal learning project, but is licensed under the
GNU GPLv3 license. See the [LICENSE](LICENSE.md) file for more information.
