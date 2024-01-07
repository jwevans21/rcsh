#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include <command.h>
#include <external.h>
#include <rcsh_debug.h>

command_status_t external_command(const command_t *const self,
                                  int *const exit_code) {
  pid_t pid = fork();
  TRACEV("pid = %d\n", pid);

  if (pid < 0) {
    ERROR_PERROR("fork");
    return COMMAND_FAILURE;
  } else if (pid == 0) {
    if (execvp(self->argv[0], self->argv) < 0) {
      ERROR_PERROR("execvp");
      return COMMAND_FAILURE;
    }
  } else {
    int status;
    if (waitpid(pid, &status, 0) < 0) {
      ERROR_PERROR("waitpid");
      return COMMAND_FAILURE;
    }

    TRACEV("waitpid returned %d\n", status);

    if (WIFEXITED(status)) {
      *exit_code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
      *exit_code = WTERMSIG(status);
    } else {
      *exit_code = 1;
    }
  }

  return COMMAND_SUCCESS;
}