#include <stdio.h>

#include <rcsh_cmd.h>
#include <rcsh_log.h>

#include <rcsh_builtin.h>

rcsh_builtin_status_t
rcsh_builtin_jobs (rcsh_cmd_t *const cmd, rcsh_ctx_t *const ctx)
{
  (void)cmd;

  rcsh_log_trace ("Fetching active jobs");
  job_t *job = ctx->jobs;

  if (job == NULL)
    {
      printf ("No active jobs\n");
      return RCSH_BUILTIN_STATUS_SUCCESS;
    }
  while (job != NULL)
    {
      printf ("[%d] %s\n", job->pid, job->cmd);
      job = job->next;
    }

  return RCSH_BUILTIN_STATUS_SUCCESS;
}
