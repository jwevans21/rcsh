#include <string.h>

#include <rcsh_builtin.h>
#include <rcsh_cmd.h>
#include <rcsh_log.h>

#include <rcsh_run.h>

rcsh_run_status_t
rcsh_run_command (rcsh_cmd_t *const cmd, rcsh_ctx_t *const ctx)
{
  rcsh_log_trace ("Running command");

  if (cmd == NULL)
    {
      rcsh_log_trace ("Command was NULL");
      return RCSH_RUN_STATUS_SUCCESS;
    }

  if (cmd->argc == 0 || cmd->argv == NULL || cmd->argv[0] == NULL)
    {
      rcsh_log_trace ("No command to run");
      rcsh_cmd_deinit ((rcsh_cmd_t * *const)&cmd);
      return RCSH_RUN_STATUS_SUCCESS;
    }

  if (strcmp (cmd->argv[0], "exit") == 0)
    {
      rcsh_log_trace ("Built-in command `exit`");

      return (rcsh_run_status_t)rcsh_builtin_exit (cmd, ctx);
    }
  else if (strcmp (cmd->argv[0], "status") == 0)
    {
      rcsh_log_trace ("Built-in command `status`");

      return (rcsh_run_status_t)rcsh_builtin_status (cmd, ctx);
    }
  else if (strcmp (cmd->argv[0], "cd") == 0)
    {
      rcsh_log_trace ("Built-in command `cd`");

      return (rcsh_run_status_t)rcsh_builtin_cd (cmd, ctx);
    }
  else
    {
      rcsh_log_trace ("External command");
      rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
      return RCSH_RUN_STATUS_SUCCESS;
    }
}
