#include <string.h>

#include <builtin.h>
#include <rcsh_log.h>

#include <command.h>

command_status_t command_run(const command_t *const self,
                             int *const exit_code) {

  if (self->argc == 0 || self->argv == NULL) {
    rcsh_trace("No command to run");
    return COMMAND_SUCCESS;
  }

  if (strncmp("cd", self->argv[0], 2) == 0) {
    rcsh_trace("builtin cd");
    return builtin_cd(self, exit_code);
  } else if (strncmp("exit", self->argv[0], 4) == 0) {
    rcsh_trace("builtin exit");
    return builtin_exit(self, exit_code);
  } else if (strncmp("status", self->argv[0], 6) == 0) {
    rcsh_trace("builtin status");
    return builtin_status(self, exit_code);
  } else {
    rcsh_trace("external command");
    return command_run_external(self, exit_code);
  }
}
