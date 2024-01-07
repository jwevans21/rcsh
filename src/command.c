#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <command.h>
#include <builtin.h>
#include <external.h>
#include <rcsh_debug.h>

command_t *command_new() {
  command_t *self = calloc(1, sizeof(command_t));
  self->source = NULL;
  self->argc_cap = 0;
  self->argc = 0;
  self->argv = NULL;

  return self;
}

void command_free(command_t **const self) {
  command_t *cmd = *self;

  if (cmd->source != NULL) {
    free(cmd->source);
    cmd->source = NULL;
  }

  if (cmd->argv != NULL) {
    for (size_t i = 0; i < cmd->argc; i++) {
      free(cmd->argv[i]);
      cmd->argv[i] = NULL;
    }

    free(cmd->argv);
    cmd->argv = NULL;
  }

  free(cmd);
  *self = NULL;
}

int command_read_from(command_t *self, FILE *in) {
  char *buffer = NULL;
  size_t len = 0, len_read = 0;

  len_read = getline(&buffer, &len, in);
  if (len_read < 0) {
    perror(LOC "getline");
    return -1;
  }

  // Remove trailing newline
  buffer[len_read - 1] = '\0';

  if (command_parse_string(self, buffer) < 0) {
    return -1;
  }

  return 0;
}

int command_add_arg(command_t *self, size_t current, char *start, char *end) {
  if (current >= self->argc_cap - 1) {
    size_t new_cap = ((self->argc_cap - 1) * 2) + 1;
    char **new_argv = reallocarray(self->argv, new_cap, sizeof(char *));
    if (new_argv == NULL) {
      perror(LOC "reallocarray");
      return -1;
    }

    self->argc_cap = new_cap;
    self->argv = new_argv;
  }

  size_t len = end - start;

  self->argv[current] = calloc(len + 1, sizeof(char));
  if (self->argv[current] == NULL) {
    perror(LOC "calloc");
    return -1;
  }

  strncpy(self->argv[current], start, len);
  self->argc++;

  return 0;
}

int command_parse_string(command_t *self, char *str) {
  if (str == NULL) {
    return -1;
  }
  self->source = str;

  self->argc_cap = 5;
  self->argv = calloc(self->argc_cap, sizeof(char *));
  if (self->argv == NULL) {
    perror(LOC "calloc");
    return -1;
  }

  size_t current_arg = 0;
  char *start = str;

  while (*str != '\0') {
    if (isspace(*str)) {
      if (str == start) {
        start++;
        str++;
      } else {
        if (command_add_arg(self, current_arg, start, str) < 0) {
          return -1;
        }
        current_arg++;
        str++;
        start = str;
      }
    } else {
      str++;
    }
  }

  if (str != start) {
    if (command_add_arg(self, current_arg, start, str) < 0) {
      return -1;
    }
  }

  return 0;
}

void command_debug(const command_t *const self) {
  TRACE("struct command {");
  TRACEV("\tsource = \"%s\"", self->source);
  TRACEV("\targc = %zu", self->argc);
  TRACE("\targv = [");
  for (size_t i = 0; i < self->argc; i++) {
    TRACEV("\t\t\"%s\",", self->argv[i]);
  }
  TRACE("\t]");
  TRACE("}");
}

command_status_t command_run(const command_t *const self,
                             int *const exit_code) {
  if (strncmp("cd", self->argv[0], 2) == 0) {
    TRACE("builtin cd");
    return builtin_cd(self, exit_code);
  } else if (strncmp("exit", self->argv[0], 4) == 0) {
    TRACE("builtin exit");
    return builtin_exit(self, exit_code);
  } else if (strncmp("status", self->argv[0], 6) == 0) {
    TRACE("builtin status");
    return builtin_status(self, exit_code);
  } else {
    TRACE("external command");
    return external_command(self, exit_code);
  }

  return 0;
}
