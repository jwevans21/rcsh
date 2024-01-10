#include <stdlib.h>

#include <rcsh_log.h>

#include <command.h>

command_t *command_new() {
  command_t *self = calloc(1, sizeof(command_t));
  self->source = NULL;
  self->argc_cap = 0;
  self->argc = 0;
  self->argv = NULL;

  self->in = NULL;
  self->out = NULL;

  return self;
}

void command_free(command_t **const self) {
  command_t *cmd = *self;

  command_free_no_src(self);
}

void command_free_no_src(command_t **const self) {
  command_t *cmd = *self;

  if (cmd->argv != NULL) {
    for (size_t i = 0; i < cmd->argc; i++) {
      free(cmd->argv[i]);
      cmd->argv[i] = NULL;
    }

    free(cmd->argv);
    cmd->argv = NULL;
  }

  if (cmd->in != NULL) {
    fclose(cmd->in);
    cmd->in = NULL;
  }

  if (cmd->out != NULL) {
    fclose(cmd->out);
    cmd->out = NULL;
  }

  free(cmd);
  *self = NULL;
}

void command_debug(const command_t *const self) {
  rcsh_trace("struct command {");
  rcsh_trace("\tsource = \"%s\"", self->source);
  rcsh_trace("\targc = %zu", self->argc);
  rcsh_trace("\targv = [");
  for (size_t i = 0; i < self->argc; i++) {
    rcsh_trace("\t\t\"%s\",", self->argv[i]);
  }
  rcsh_trace("\t]");
  rcsh_trace("\tin = %p", self->in);
  rcsh_trace("\tout = %p", self->out);
  rcsh_trace("}");
}