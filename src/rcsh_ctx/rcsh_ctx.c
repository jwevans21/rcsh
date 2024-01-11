#define _POSIX_C_SOURCE 200809L

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  rcsh_log_trace ("Opened /dev/null %p", self->null);

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
