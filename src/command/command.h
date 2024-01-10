#ifndef __JWEVANS__RCSH__COMMAND_H_
#define __JWEVANS__RCSH__COMMAND_H_

#include <stdio.h>

typedef struct command {
  char *source;
  size_t argc_cap;
  size_t argc;
  char **argv;

  FILE *in;
  FILE *out;
} command_t;

typedef struct {
  enum {
    /// The command was successfully parsed, but it continues on the next line
    COMMAND_PARSE_CONTINUE,
    /// The command was successfully parsed and is complete
    COMMAND_PARSE_COMPLETE,
    /// The command was not successfully parsed
    COMMAND_PARSE_FAILURE,
  } status;
  /// The command that was parsed
  command_t *command;
} command_parse_result_t;

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

// int command_read_from(command_t *self, FILE *in);
// int command_parse_string(command_t *self, char *str);

void command_debug(const command_t *const self);

command_parse_result_t command_from_file(FILE *const file);
command_parse_result_t command_from_string(const char *const str);
command_parse_result_t command_continue_from_file(FILE *const file,
                                                  command_t *const command);



#endif
