#define __JWEVANS__RCSH__CMD_H__INTERNAL__ 1

#include <stdlib.h>

#include <rcsh_log.h>

#include <rcsh_cmd.h>

void
rcsh_cmd_add_arg (rcsh_cmd_t *const self, char *arg)
{
  rcsh_log_trace ("Adding argument \"%s\"", arg);
  if (self == NULL)
    {
      return;
    }

  if (self->argv_capacity <= (self->argc + 1))
    {
      rcsh_cmd_grow_argv (self);
    }

  self->argc++;
  self->argv[self->argc - 1] = arg;
  self->argv[self->argc] = NULL;
}

void
rcsh_cmd_grow_argv (rcsh_cmd_t *const self)
{
  rcsh_log_trace ("Growing argv");
  if (self == NULL)
    {
      return;
    }

  size_t new_capacity = self->argv_capacity == 0 ? 4 : self->argv_capacity * 2;

  char **new_argv = calloc (new_capacity, sizeof (char *));
  for (size_t i = 0; i < self->argc; i++)
    {
      new_argv[i] = self->argv[i];
    }

  free (self->argv);
  self->argv = new_argv;
  self->argv_capacity = new_capacity;
}
