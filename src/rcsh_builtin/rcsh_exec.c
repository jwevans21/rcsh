#include <stdio.h>
#include <unistd.h>

#include <rcsh_cmd.h>
#include <rcsh_log.h>

#include <rcsh_builtin.h>

rcsh_builtin_status_t
rcsh_builtin_exec (rcsh_cmd_t *const cmd, rcsh_ctx_t *const ctx)
{
  (void)ctx;

  rcsh_log_trace ("Executing command");

  if (cmd == NULL)
    {
      rcsh_log_error ("Command was NULL");
      return RCSH_BUILTIN_STATUS_FAILURE;
    }

  if (cmd->argc == 0 || cmd->argv == NULL || cmd->argv[0] == NULL)
    {
      rcsh_log_error ("No command to run");
      rcsh_cmd_deinit ((rcsh_cmd_t * *const)&cmd);
      return RCSH_BUILTIN_STATUS_FAILURE;
    }

  rcsh_log_trace ("Executing command `%s`", cmd->argv[0]);

  if (execvp (cmd->argv[0], cmd->argv) == -1)
    {
      rcsh_log_error ("Failed to execute command `%s`", cmd->argv[0]);
      return RCSH_BUILTIN_STATUS_FAILURE;
    }

  return RCSH_BUILTIN_STATUS_SUCCESS;
}
