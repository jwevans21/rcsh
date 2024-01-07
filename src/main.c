#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include <command.h>

#include <rcsh_debug.h>

int shell_loop(int argc, char *argv[]);

const char *const USAGE =
    "Usage: %s [OPTION]... [FILE]\n"
    "\n"
    "Options:\n"
    "  -v\t\tverbose mode use more v's to increase level (e.g. -vvv)\n"
    "  -c COMMAND\texecute command\n"
    "  -h\t\tdisplay this help and exit\n"
    "  -V\t\tdisplay version information and exit\n";

const char *const VERSION =
    "%s 0.0.1\n"
    "\n"
    "Author: Jacob Evans <jacob@jwevans.dev>\n"
    "License: GPL-3.0-or-later\n"
    "\n"
    "For source code and bug reports see <https://github.com/jwevans21/rcsh>\n"
    "\n";

static int parse_command_line_options(int argc, char *argv[], int *leave,
                                      int *verbose, int *cmd, char **cmd_str) {
  *verbose = 0;
  *cmd = 0;
  *cmd_str = NULL;

  int opt, option_index;
  static struct option long_options[] = {
      {"version", no_argument, 0, 'V'},
      {"verbose", no_argument, 0, 'v'},
      {"command", required_argument, 0, 'c'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0},
  };

  while ((opt = getopt_long(argc, argv, "Vvc:h?", long_options,
                            &option_index)) != -1) {
    switch (opt) {
    case 0:
      switch (option_index) {
      case 0:
        fprintf(stdout, VERSION, argv[0]);
        *leave = 1;
        return 0;
      case 1:
        verbose += 1;
        break;
      case 2:
        *cmd = 1;
        *cmd_str = optarg;
        break;
      case 3:
        fprintf(stderr, USAGE, argv[0]);
        *leave = 1;
        return 0;
      default:
        fprintf(stderr, USAGE, argv[0]);
        *leave = 1;
        return 1;
      }
      break;
    case 'V':
      fprintf(stdout, VERSION, argv[0]);
      return 0;
    case 'v':
      verbose += 1;
      break;
    case 'c':
      *cmd = 1;
      *cmd_str = optarg;
      break;
    case 'h':
    case '?':
      fprintf(stderr, USAGE, argv[0]);
      *leave = 1;
      return 0;
    default:
      fprintf(stderr, USAGE, argv[0]);
      *leave = 1;
      return 1;
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  int verbose = 0, cmd = 0;
  char *cmd_str = NULL;

  int leave = 0;
  int ret = parse_command_line_options(argc, argv, &leave, &verbose, &cmd,
                                       &cmd_str);
  
  if (leave) {
    return ret;
  }

  if (verbose > 0) {
    debug_level = verbose;
  }

  if (cmd != 1) {
    return shell_loop(argc, argv);
  } else {
    command_t *cmd = command_new();
    if (command_parse_string(cmd, cmd_str) < 0) {
      return 1;
    }

    int exit_code = 0;
    command_status_t status = command_run(cmd, &exit_code);

    command_free_no_src(&cmd);

    switch (status) {
    case COMMAND_SUCCESS:
      return exit_code;
    case COMMAND_EXEC_FAILURE:
      return 127;
    case COMMAND_FAILURE:
      return 1;
    case COMMAND_EXIT:
      return exit_code;
    default:
      return 1;
    }
  }
}

void print_exec_error(char *argv[], const command_t *const cmd) {
  fprintf(stderr, "%s: unable to run command: %s\n", argv[0], cmd->argv[0]);
}

int shell_loop(int argc, char *argv[]) {

  TRACE("Initializing shell");
  pid_t pid = getpid();
  int exit_code = 0;

  do {
    if (feof(stdin)) {
      fprintf(stdout, "\n");
      return 0;
    }
    if (ferror(stdin)) {
      return -1;
    }

    fprintf(stdout, "%d $ ", pid);
    fflush(stdout);

    command_t *cmd = command_new();
    TRACE("Reading command");
    if (command_read_from(cmd, stdin) < 0) {
      return -1;
    }

    command_debug(cmd);

    TRACE("Running command");
    int ret = command_run(cmd, &exit_code);

    switch (ret) {
    case COMMAND_SUCCESS:
      INFO("Command succeeded or was executed in background");
      break;
    case COMMAND_EXEC_FAILURE:
      ERROR("Command failed to execute");
      print_exec_error(argv, cmd);
      command_free(&cmd);
      return -1;
    case COMMAND_FAILURE:
      ERROR("Command failed");
      command_free(&cmd);
      break;
    case COMMAND_EXIT:
      INFOV("Exiting with code %d\n", exit_code);
      command_free(&cmd);
      return exit_code;
    default:
      ERRORV("Unknown command status %d\n", ret);
      return -1;
    }

    TRACE("Freeing command");
    command_free(&cmd);

  } while (1);

  return 0;
}