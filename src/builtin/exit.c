#include <stdlib.h>

#include <builtin.h>

command_status_t builtin_exit(const command_t *const self,
                              int *const exit_code) {
  if (self->argc > 1) {
    *exit_code = atoi(self->argv[1]);
  }

  return COMMAND_EXIT;
}
