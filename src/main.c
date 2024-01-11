#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <rcsh_cmd.h>
#include <rcsh_ctx.h>
#include <rcsh_log.h>

int
main (int argc, char *argv[])
{
  rcsh_log_set_level (RCSH_LOG_LEVEL_TRACE);
  rcsh_log_trace ("Initializing shell context");
  rcsh_ctx_t ctx = { 0 };
  rcsh_ctx_init (&ctx);

  do
    {
      rcsh_log_trace ("Checking for EOF in stdin");
      if (feof (stdin))
        {
          rcsh_log_info ("Encountered EOF on stdin");
          break;
        }

      rcsh_log_trace ("Printing shell prompt");
      fprintf (stderr, "%d $ ", ctx.pid);
      fflush (stdout);

      rcsh_cmd_t *cmd = rcsh_cmd_from_file (stdin);
      rcsh_log_trace ("Read command from stdin");

      if (cmd == NULL)
        {
          rcsh_log_trace ("Command parsed was NULL, reprompting for input");
          continue;
        }

      rcsh_cmd_debug (cmd);

      rcsh_log_trace ("Running the command (TODO)");

      rcsh_log_trace ("Updating shell context with exit status or background "
                      "process (TODO)");

      // sleep (5);

      rcsh_cmd_deinit (&cmd);
    }
  while (1);

  rcsh_log_trace ("Deinitializing shell context");
  rcsh_ctx_deint (&ctx);
  return WEXITSTATUS (ctx.exit_status);
}
