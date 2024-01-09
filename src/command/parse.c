#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <rcsh_log.h>

#include <command.h>

int command_read_from(command_t *self, FILE *in) {
  char *buffer = NULL;
  size_t len = 0, len_read = 0;

  len_read = getline(&buffer, &len, in);
  
  if (len_read < 0) {
    perror(LOC "getline");
    return -1;
  }
  rcsh_trace("Read %zu bytes from input with a buffer size of %zu", len_read,
             len);
  rcsh_info("Read buffer: \"%s\"", buffer);

  // Remove trailing newline
  buffer[len_read - 1] = '\0';

  rcsh_trace("Parsing input line");
  if (command_parse_string(self, buffer) < 0) {
    return -1;
  }

  return 0;
}

int command_add_arg(command_t *self, size_t current, char *start, char *end) {
  rcsh_trace("Adding new argument");
  if (current >= self->argc_cap - 1) {
    rcsh_trace("Resizing argv for new argument");
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

  rcsh_trace("Allocating memory for new argument");
  self->argv[current] = calloc(len + 1, sizeof(char));
  if (self->argv[current] == NULL) {
    perror(LOC "calloc");
    return -1;
  }

  rcsh_trace("Copying %zu bytes from %p to %p", len, start,
             self->argv[current]);
  strncpy(self->argv[current], start, len);
  self->argc++;

  return 0;
}

int command_parse_string(command_t *self, char *str) {
  if (str == NULL) {
    return -1;
  }
  self->source = str;

  rcsh_trace("Parsing string \"%s\"", str);
  rcsh_trace("Allocating memory for argv");
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