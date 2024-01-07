#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include <command.h>

int main(int argc, char *argv[]) {
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
    if (command_read_from(cmd, stdin) < 0) {
      return -1;
    }

    command_debug(cmd);

    int ret = command_run(cmd, &exit_code);

    switch (ret) {
    case COMMAND_SUCCESS:
      break;
    case COMMAND_FAILURE:
      fprintf(stderr, "ERROR: Command failed\n");
      command_free(&cmd);
      break;
    case COMMAND_EXIT:
      fprintf(stderr, "INFO: Exiting with code %d\n", exit_code);
      command_free(&cmd);
      return exit_code;
    default:
      fprintf(stderr, "ERROR: Unknown command status\n");
      return -1;
    }
    command_free(&cmd);

  } while (1);

  return 0;
}
