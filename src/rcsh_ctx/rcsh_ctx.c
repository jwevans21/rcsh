#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include <rcsh_log.h>

#include <rcsh_ctx.h>

void
rcsh_ctx_init (rcsh_ctx_t *const self)
{
  self->ppid = getppid ();
  rcsh_log_trace ("PPID = %d", self->ppid);
  self->pid = getpid ();
  rcsh_log_trace (" PID = %d", self->pid);

  rcsh_log_trace ("Opening /dev/null");
  self->null = fopen ("/dev/null", "w+");
  if (ferror (self->null))
    {
      rcsh_log_error ("Unable to open /dev/null: %s", strerror (errno));
      exit (1);
    }
  rcsh_log_trace ("Opened /dev/null %p", (void *)self->null);

  self->exit_status = 0;
}

void
rcsh_ctx_deint (rcsh_ctx_t *const self)
{
  if (self->null != NULL)
    {
      rcsh_log_trace ("Closing /dev/null");
      fclose (self->null);
      self->null = NULL;
    }
}

void
rcsh_ctx_add_job (rcsh_ctx_t *const self, pid_t pid, const char *const cmd)
{
  rcsh_log_trace ("Adding job %d: %s", pid, cmd);
  job_t *job = calloc (1, sizeof (job_t));
  job->pid = pid;
  job->cmd = strdup (cmd);
  job->next = self->jobs;
  self->jobs = job;
}

void
rcsh_ctx_harvest_jobs (rcsh_ctx_t *const self)
{
  rcsh_log_trace ("Harvesting jobs");
  job_t *prev = NULL;
  job_t *job = self->jobs;
  while (job != NULL)
    {
      rcsh_log_trace ("Harvesting job %d", job->pid);
      int status = -1;
      pid_t res = waitpid (job->pid, &status, WNOHANG);
      if (res == -1)
        {
          rcsh_log_error ("Failed to wait for job %d: %s", job->pid,
                          strerror (errno));
          exit (1);
        }
      else if (res == 0)
        {
          rcsh_log_trace ("Job %d is still running", job->pid);
          prev = job;
          job = job->next;
        }
      else
        {
          rcsh_log_trace ("Job %d has exited", job->pid);
          if (WIFEXITED (status))
            {
              rcsh_log_trace ("Job %d exited with status %d", job->pid,
                              WEXITSTATUS (status));
            }
          else if (WIFSIGNALED (status))
            {
              rcsh_log_trace ("Job %d exited with signal %d", job->pid,
                              WTERMSIG (status));
            }
          else
            {
              rcsh_log_trace ("Job %d exited with unknown status %d", job->pid,
                              status);
            }
          job_t *next = job->next;
          free (job->cmd);
          free (job);
          job = next;
          if (prev == NULL)
            {
              self->jobs = job;
            }
          else
            {
              prev->next = job;
            }
        }
    }
}
