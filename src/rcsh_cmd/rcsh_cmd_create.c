#define _POSIX_C_SOURCE 200809L
#define __JWEVANS__RCSH__CMD_H__INTERNAL__ 1

#include <stdlib.h>

#include <rcsh_log.h>

#include <rcsh_cmd.h>

rcsh_cmd_t *
rcsh_cmd_init ()
{
  rcsh_cmd_t *self = calloc (1, sizeof (rcsh_cmd_t));
  return self;
}

void
rcsh_cmd_deinit (rcsh_cmd_t **const self)
{
  if (self == NULL || *self == NULL)
    {
      return;
    }

  if ((*self)->argv != NULL)
    {
      for (size_t i = 0; (*self)->argv[i] != NULL; i++)
        {
          free ((*self)->argv[i]);
        }
      free ((*self)->argv);
    }

  free (*self);
  *self = NULL;
}

void
rcsh_cmd_debug (const rcsh_cmd_t *const self)
{
  if (self == NULL)
    {
      rcsh_log_trace ("rcsh_cmd_t is NULL");
      return;
    }

  rcsh_log_trace ("rcsh_cmd_t {");
  rcsh_log_trace ("  argc = %zu", self->argc);
  rcsh_log_trace ("  argv = {");
  for (size_t i = 0; i < self->argc; i++)
    {
      rcsh_log_trace ("    [%zu] = \"%s\"", i, self->argv[i]);
    }
  rcsh_log_trace ("  }");
  rcsh_log_trace ("}");
}
