#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <rcsh_cmd.h>
#include <rcsh_log.h>

#include <rcsh_builtin.h>

rcsh_builtin_status_t
rcsh_builtin_exit (rcsh_cmd_t *const cmd, rcsh_ctx_t *const ctx)
{
  if (cmd == NULL || ctx == NULL || cmd->argc == 0 || cmd->argv == NULL)
    {
      rcsh_log_error (
          "Invalid parameters passed to function: `rcsh_builtin_exit`");
      exit (-1);
    }

  if (cmd->argc > 2)
    {
      rcsh_log_warn ("Too many arguments passed to builtin `exit`");
    }

  if (cmd->argc >= 2)
    {
      char *end = NULL;
      errno = 0;
      long code = strtol (cmd->argv[1], &end, 10);
      if (errno != 0)
        {
          rcsh_log_error ("Error parsing provided exit code: %s",
                          strerror (errno));
          ctx->exit_status = -1;
          rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
          return RCSH_BUILTIN_STATUS_EXIT;
        }

      if (end == cmd->argv[1])
        {
          rcsh_log_error ("Empty string provided as exit code");
          ctx->exit_status = -1;
          rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
          return RCSH_BUILTIN_STATUS_EXIT;
        }

      if (*end != '\0')
        {
          rcsh_log_error ("Invalid characters provided in exit code");
          ctx->exit_status = -1;
          rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
          return RCSH_BUILTIN_STATUS_EXIT;
        }

      // Shift left to allow the exit status to be obtained
      ctx->exit_status = code << 8;
    }
  else
    {
      ctx->exit_status = 0;
    }

  rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
  return RCSH_BUILTIN_STATUS_EXIT;
}
