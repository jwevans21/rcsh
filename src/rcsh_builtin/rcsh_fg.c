#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <rcsh_cmd.h>
#include <rcsh_log.h>

#include <rcsh_builtin.h>

rcsh_builtin_status_t
rcsh_builtin_fg (rcsh_cmd_t *const cmd, rcsh_ctx_t *const ctx)
{

  pid_t pid = 0;
  if (cmd->argc >= 2)
    {
      pid = atoi (cmd->argv[1]);
    }
  else
    {
      rcsh_log_error ("Invalid number of arguments");
      return RCSH_BUILTIN_STATUS_FAILURE;
    }

  rcsh_log_info ("Bringing job %d to the foreground", pid);

  job_t *job = ctx->jobs, *prev = NULL;
  while (job != NULL)
    {
      if (job->pid == pid)
        {
          if (prev == NULL)
            {
              ctx->jobs = job->next;
            }
          else
            {
              prev->next = job->next;
            }
          break;
        }
      prev = job;
      job = job->next;
    }

  if (job == NULL)
    {
      rcsh_log_error ("Job %d not found", pid);
      return RCSH_BUILTIN_STATUS_FAILURE;
    }

  rcsh_log_trace ("Resuming job %d", pid);

  if (kill (pid, SIGCONT) == -1)
    {
      rcsh_log_error ("Failed to resume job %d", pid);
      return RCSH_BUILTIN_STATUS_FAILURE;
    }

  rcsh_log_trace ("Waiting for job %d", pid);

  int status = 0;
  if (waitpid (pid, &status, WUNTRACED) == -1)
    {
      rcsh_log_error ("Failed to wait for job %d", pid);
      return RCSH_BUILTIN_STATUS_FAILURE;
    }

  if (WIFEXITED (status))
    {
      rcsh_log_trace ("Job %d exited with status %d", pid,
                      WEXITSTATUS (status));
    }
  else if (WIFSIGNALED (status))
    {
      rcsh_log_trace ("Job %d terminated by signal %d", pid,
                      WTERMSIG (status));
    }
  else if (WIFSTOPPED (status))
    {
      rcsh_log_trace ("Job %d stopped by signal %d", pid, WSTOPSIG (status));

      ctx->exit_status = 0;

      job->next = ctx->jobs;
      ctx->jobs = job;
    }
  else if (WIFCONTINUED (status))
    {
      rcsh_log_trace ("Job %d continued", pid);
    }
  else
    {
      rcsh_log_trace ("Job %d terminated abnormally", pid);
    }

  ctx->exit_status = status;

  rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);

  return RCSH_BUILTIN_STATUS_SUCCESS;
}
