#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <rcsh_log.h>

#include <command.h>

int command_add_argument(command_t *const self, char *const arg) {
  static int redirect_out_count = 0;
  static int redirect_in_count = 0;

  if (redirect_out_count > 0) {
    if (self->out != NULL) {
      rcsh_error("Output redirection already set");
      return -1;
    }

    self->out = fopen(arg, redirect_out_count > 1 ? "a" : "w");
    redirect_out_count = 0;
    return 0;
  }

  if (redirect_in_count > 0) {
    if (self->in != NULL) {
      rcsh_error("Input redirection already set");
      return -1;
    }

    self->in = fopen(arg, "r");
    if (self->in == NULL || ferror(self->in)) {
      rcsh_error("Failed to open file for input redirection");
      return -1;
    }

    redirect_in_count = 0;
    return 0;
  }

  if (strncmp(arg, ">>", 2) == 0) {
    redirect_out_count = 2;
    return 0;
  }

  if (strncmp(arg, ">", 1) == 0) {
    redirect_out_count = 1;
    return 0;
  }

  if (strncmp(arg, "<", 1) == 0) {
    redirect_in_count = 1;
    return 0;
  }

  if (self->argc_cap == 0) {
    self->argv = calloc(1, sizeof(char *));
    if (self->argv == NULL) {
      rcsh_error("Failed to allocate memory for argv");
      return -1;
    }
    self->argc_cap = 1;
  }

  if (self->argc >= self->argc_cap) {
    self->argc_cap *= 2;

    self->argv = reallocarray(self->argv, self->argc_cap, sizeof(char *));
    if (self->argv == NULL) {
      rcsh_error("Failed to reallocate memory for argv");
      return -1;
    }

    for (size_t i = self->argc; i < self->argc_cap; i++) {
      self->argv[i] = NULL;
    }
  }

  self->argv[self->argc] = arg;
  self->argc++;

  return 0;
}

char *command_next_argument(char **const saveptr) {
  if (saveptr == NULL) {
    rcsh_error("saveptr is NULL" LOC);
    return NULL;
  }

  if (*saveptr == NULL) {
    rcsh_error("*saveptr is NULL" LOC);
    return NULL;
  }

  if (**saveptr == '\0') {
    rcsh_trace("Reached end of string");
    return NULL;
  }

  char *start = *saveptr;
  char *end = *saveptr;
  char *arg = NULL;

  // consume leading whitespace
  while (*end != '\0' && isspace(*end)) {
    end++;
  }

  if (*end == '\0') {
    rcsh_trace("Reached end of string");
    return NULL;
  }

  start = end;

  if (*end == '"') {
    end++;
    start = end;
    while (*end != '\0' && (*end != '"' && *(end - 1) != '\\')) {
      end++;
    }
    if (*end == '\0') {
      rcsh_error("Unmatched double quote");
      return NULL;
    }

    arg = strndup(start, end - start);
    if (arg == NULL) {
      rcsh_error("Failed to duplicate argument string");
      return NULL;
    }

    end++;
  } else {
    while (*end != '\0' && !isspace(*end)) {
      end++;
    }

    arg = strndup(start, end - start);
    if (arg == NULL) {
      rcsh_error("Failed to duplicate argument string");
      return NULL;
    }
  }

  *saveptr = end;
  return arg;
}

command_parse_result_t command_from_file(FILE *const file) {
  command_parse_result_t result = {0};

  char *line = NULL;
  size_t line_size = 0;
  ssize_t line_len = 0;
  line_len = getline(&line, &line_size, file);
  if (line_len < 0) {
    rcsh_error("Failed to read line from file");
    result.status = COMMAND_PARSE_FAILURE;
    return result;
  }

  result.command = command_new();
  if (result.command == NULL) {
    rcsh_error("Failed to allocate memory for command");
    result.status = COMMAND_PARSE_FAILURE;
    return result;
  }

  result.command->source = line;

  char *saveptr = line;
  char *arg = NULL;

  while ((arg = command_next_argument(&saveptr)) != NULL) {
    if (command_add_argument(result.command, arg) < 0) {
      rcsh_error("Failed to add argument to command");
      command_free(&result.command);
      result.status = COMMAND_PARSE_FAILURE;
      return result;
    }
  }

  result.status = COMMAND_PARSE_COMPLETE;
  return result;
}

command_parse_result_t command_from_string(const char *const string) {
  command_parse_result_t result = {0};
  result.command = command_new();
  if (result.command == NULL) {
    rcsh_error("Failed to allocate memory for command");
    result.status = COMMAND_PARSE_FAILURE;
    result.command = NULL;
    return result;
  }

  result.command->source = strdup(string);
  if (result.command->source == NULL) {
    rcsh_error("Failed to duplicate string");
    command_free(&result.command);
    result.status = COMMAND_PARSE_FAILURE;
    result.command = NULL;
    return result;
  }

  char *start = result.command->source;
  char *end = result.command->source;

  while (*end != '\0') {
    start = end;
    if (isspace(*end)) {
      end++;
    } else if (*end == '"') {
      end++;
      start = end;
      while (*end != '\0' && *end != '"') {
        end++;
      }
      if (*end == '\0') {
        rcsh_error("Unmatched double quote");
        command_free(&result.command);
        result.status = COMMAND_PARSE_FAILURE;
        result.command = NULL;
        return result;
      }

      char *arg = strndup(start, end - start);
      if (arg == NULL) {
        rcsh_error("Failed to duplicate argument string");
        command_free(&result.command);
        result.status = COMMAND_PARSE_FAILURE;
        result.command = NULL;
        return result;
      }

      if (command_add_argument(result.command, arg) < 0) {
        rcsh_error("Failed to add argument to command");
        command_free(&result.command);
        result.status = COMMAND_PARSE_FAILURE;
        result.command = NULL;
        return result;
      }

      end++;
    } else {
      while (*end != '\0' && !isspace(*end) && *end != '"') {
        end++;
      }

      char *arg = strndup(start, end - start);
      if (arg == NULL) {
        rcsh_error("Failed to duplicate argument string");
        command_free(&result.command);
        result.status = COMMAND_PARSE_FAILURE;
        result.command = NULL;
        return result;
      }

      if (command_add_argument(result.command, arg) < 0) {
        rcsh_error("Failed to add argument to command");
        command_free(&result.command);
        result.status = COMMAND_PARSE_FAILURE;
        result.command = NULL;
        return result;
      }
    }
  }

  return result;
}