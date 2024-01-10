#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

#include <command.h>
#include <rcsh_log.h>

command_status_t command_run_external(const command_t *const self,
                                      int *const exit_status) {
  pid_t pid = fork();
  rcsh_trace("pid = %d\n", pid);

  if (pid < 0) {
    rcsh_error("fork");
    return COMMAND_FAILURE;
  } else if (pid == 0) {

    if (self->in != NULL) {
      dup2(fileno(self->in), STDIN_FILENO);
    }

    if (self->out != NULL) {
      dup2(fileno(self->out), STDOUT_FILENO);
    }


    if (execvp(self->argv[0], self->argv) < 0) {
      rcsh_error("execvp");
      return COMMAND_EXEC_FAILURE;
    }
  } else {
    int status;
    if (waitpid(pid, &status, 0) < 0) {
      rcsh_error("waitpid");
      return COMMAND_FAILURE;
    }

    rcsh_trace("waitpid returned %d\n", status);

    *exit_status = WEXITSTATUS(status);
  }

  return COMMAND_SUCCESS;
}