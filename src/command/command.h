#ifndef __JWEVANS__RCSH__COMMAND_H_
#define __JWEVANS__RCSH__COMMAND_H_

#include <stdio.h>

typedef struct command {
  char *source;
  size_t argc_cap;
  size_t argc;
  char **argv;
} command_t;

typedef enum command_status {
  /// The command was successfully executed. This does not reflect the exit code
  COMMAND_SUCCESS,
  /// The command was successfully executed in the background. This does not
  /// indicate that the command has finished executing
  COMMAND_BACKGROUND,
  /// The command was not executed because of an error. This caused the shell to
  /// exit
  COMMAND_FAILURE,
  /// The command was not executed because of an error. This did not cause the
  /// shell to exit, but the command was not executed.
  ///
  /// This is kind of a lie since the shell will exit, but the shell that exited
  /// is a forked child process and not the main shell process.
  COMMAND_EXEC_FAILURE,
  /// The command was the exit command and the shell should exit
  COMMAND_EXIT,
} command_status_t;

command_t *command_new();
void command_free(command_t **const self);
void command_free_no_src(command_t **const self);

command_status_t command_run(const command_t *const self, int *const exit_code);
command_status_t command_run_external(const command_t *const self,
                                      int *const exit_code);

int command_read_from(command_t *self, FILE *in);
int command_parse_string(command_t *self, char *str);

void command_debug(const command_t *const self);

#endif
