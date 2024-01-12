#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <rcsh_cmd.h>
#include <rcsh_log.h>

#include <rcsh_run.h>

int
init_external_cmd_signal_handlers (const rcsh_cmd_t *const cmd)
{
  struct sigaction SIGINT_ACTION = { 0 };
  SIGINT_ACTION.sa_handler = SIG_DFL;
  sigemptyset (&SIGINT_ACTION.sa_mask);
  SIGINT_ACTION.sa_flags = SA_RESTART;

  struct sigaction SIGQUIT_ACTION = { 0 };
  SIGQUIT_ACTION.sa_handler = SIG_DFL;
  sigemptyset (&SIGQUIT_ACTION.sa_mask);
  SIGQUIT_ACTION.sa_flags = SA_RESTART;

  struct sigaction SIGTSTP_ACTION = { 0 };
  SIGTSTP_ACTION.sa_handler = SIG_DFL;
  sigemptyset (&SIGTSTP_ACTION.sa_mask);
  SIGTSTP_ACTION.sa_flags = SA_RESTART;

  if (cmd->background)
    {
      SIGINT_ACTION.sa_handler = SIG_IGN;
      SIGQUIT_ACTION.sa_handler = SIG_IGN;
      SIGTSTP_ACTION.sa_handler = SIG_IGN;
    }

  if (sigaction (SIGINT, &SIGINT_ACTION, NULL) == -1)
    {
      rcsh_log_error ("Failed to set SIGINT handler");
      return -1;
    }

  if (sigaction (SIGQUIT, &SIGQUIT_ACTION, NULL) == -1)
    {
      rcsh_log_error ("Failed to set SIGQUIT handler");
      return -1;
    }

  if (sigaction (SIGTSTP, &SIGTSTP_ACTION, NULL) == -1)
    {
      rcsh_log_error ("Failed to set SIGTSTP handler");
      return -1;
    }

  return 0;
}

rcsh_run_status_t
rcsh_run_external (rcsh_cmd_t *const cmd, rcsh_ctx_t *const ctx)
{
  pid_t child = fork ();
  if (child == -1)
    {
      rcsh_log_error ("fork() failed");

      rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
      return RCSH_RUN_STATUS_FAILURE;
    }

  if (child == 0)
    {
      // Child process
      init_external_cmd_signal_handlers (cmd);

      if (cmd->input != NULL)
        {
          rcsh_log_trace ("Redirecting stdin to %d", fileno (cmd->input));
          dup2 (fileno (cmd->input), STDIN_FILENO);
        }
      else if (cmd->background)
        {
          dup2 (fileno (ctx->null), STDIN_FILENO);
        }

      if (cmd->output != NULL)
        {
          rcsh_log_trace ("Redirecting stdout to %d", fileno (cmd->output));
          dup2 (fileno (cmd->output), STDOUT_FILENO);
        }
      else if (cmd->background)
        {
          dup2 (fileno (ctx->null), STDOUT_FILENO);
        }

      execvp (cmd->argv[0], cmd->argv);

      // If execvp() returns, it failed
      rcsh_log_error ("execvp() failed");

      exit (-1);
    }
  else
    {
      // Parent process
      rcsh_log_trace ("fork() succeeded. Child PID: %d", child);

      if (cmd->background)
        {
          rcsh_log_trace ("Adding job to job list");
          rcsh_ctx_add_job (ctx, child, cmd->argv[0]);

          rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
          return RCSH_RUN_STATUS_SUCCESS;
        }
      else
        {
          rcsh_log_trace ("Waiting for child to exit");
        }

      int status = 0;
      int ret = waitpid (child, &status, 0);
      if (ret == -1)
        {
          rcsh_log_error ("waitpid() failed");

          rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
          return RCSH_RUN_STATUS_FAILURE;
        }

      if (WIFEXITED (status))
        {
          rcsh_log_trace ("Child exited with status %d", WEXITSTATUS (status));
        }
      else if (WIFSIGNALED (status))
        {
          rcsh_log_trace ("Child terminated by signal %d", WTERMSIG (status));
        }
      else if (WIFSTOPPED (status))
        {
          rcsh_log_trace ("Child stopped by signal %d", WSTOPSIG (status));
        }
      else if (WIFCONTINUED (status))
        {
          rcsh_log_trace ("Child continued");
        }
      else
        {
          rcsh_log_trace ("Child terminated abnormally");
        }

      ctx->exit_status = status;

      rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
      return RCSH_RUN_STATUS_SUCCESS;
    }

  return RCSH_RUN_STATUS_SUCCESS;
}
