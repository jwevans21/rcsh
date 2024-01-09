#include <builtin.h>

command_status_t builtin_status(const command_t *const self,
                                int *const exit_code) {
  fprintf(stdout, "%d\n", *exit_code);
  *exit_code = 0;
  return COMMAND_SUCCESS;
}