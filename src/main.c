#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <rcsh_cmd.h>
#include <rcsh_ctx.h>
#include <rcsh_log.h>
#include <rcsh_run.h>

#include <signal_handlers.h>

static struct
{
  rcsh_log_level_t log_level;
  char *command;
} arguments;

static struct option long_options[] = {
  { "command", required_argument, NULL, 'c' },
  { "verbose", no_argument, NULL, 'v' },
  { "quiet", no_argument, NULL, 'q' },
  { "version", no_argument, NULL, 'V' },
  { "help", no_argument, NULL, 'h' },
  { NULL, 0, NULL, 0 },
};

int
main (int argc, char *argv[])
{
  arguments.log_level = RCSH_LOG_LEVEL_ERROR;
  arguments.command = NULL;

  int c;
  while (1)
    {
      int option_index = 0;
      c = getopt_long (argc, argv, "c:vqhV?", long_options, &option_index);

      if (c == -1)
        {
          break;
        }

      switch (c)
        {
        case 'c':
          arguments.command = optarg;
          break;
        case 'v':
          arguments.log_level = arguments.log_level + 1 > RCSH_LOG_LEVEL_TRACE
                                    ? RCSH_LOG_LEVEL_TRACE
                                    : arguments.log_level + 1;
          break;
        case 'q':
          arguments.log_level = RCSH_LOG_LEVEL_QUIET;
          break;
        case 'h':
        case '?':
          printf ("Usage: %s [OPTIONS]\n", argv[0]);
          printf ("Options:\n");
          printf ("  -v, --verbose  Increase verbosity\n");
          printf ("  -q, --quiet    Decrease verbosity\n");
          printf ("  -h, --help     Print this help message\n");
          printf ("  -V, --version  Print the version of rcsh\n");
          printf ("\n");
          // fall through
        case 'V':
          printf ("rcsh version 0.1\n");
          printf ("\n");
          printf ("Author: Jacob Evans\n");
          return 0;
        default:
          rcsh_log_error ("Unknown option: %c", c);
          return -1;
        }
    }

  rcsh_log_set_level (arguments.log_level);

  rcsh_log_trace ("Setting up signal handlers");
  if (init_signal_handlers () == -1)
    {
      rcsh_log_error ("Failed to set up signal handlers");
      return -1;
    }

  rcsh_log_trace ("Initializing shell context");
  rcsh_ctx_t ctx = { 0 };
  rcsh_ctx_init (&ctx);

  if (arguments.command != NULL)
    {
      rcsh_cmd_t *cmd = calloc (1, sizeof (rcsh_cmd_t));

      rcsh_log_trace("Parsing command from arguments");

      rcsh_cmd_parse (cmd, arguments.command, &ctx);
      rcsh_cmd_debug (cmd);

      rcsh_log_trace("Running the command");

      rcsh_run_status_t res = rcsh_run_command (cmd, &ctx);
      cmd = NULL;

      switch (res)
        {
        case RCSH_RUN_STATUS_SUCCESS:
          rcsh_log_trace ("Command ran successfully");
          break;
        case RCSH_RUN_STATUS_EXIT:
          rcsh_log_trace ("Command exited the shell");
          break;
        case RCSH_RUN_STATUS_FAILURE:
          rcsh_log_trace ("Command failed to run");
          break;
        }
    }
  else
    {
      int continue_loop = 1;
      do
        {
          rcsh_ctx_harvest_jobs (&ctx);

          rcsh_log_trace ("Checking for EOF in stdin");
          if (feof (stdin))
            {
              rcsh_log_info ("Encountered EOF on stdin");
              continue_loop = 0;
              break;
            }

          rcsh_log_trace ("Printing shell prompt");
          fprintf (stderr, "%d $ ", ctx.pid);
          fflush (stdout);

          rcsh_cmd_t *cmd = rcsh_cmd_from_file (stdin, &ctx);
          rcsh_log_trace ("Read command from stdin");

          if (cmd == NULL)
            {
              rcsh_log_trace (
                  "Command parsed was NULL, reprompting for input");
              continue;
            }

          rcsh_cmd_debug (cmd);

          rcsh_log_trace ("Running the command");
          // NOTE: This call consumes the command (`cmd`) so it will be
          // unusable after this. As such it is set to null after to prevent
          // use of the command.
          rcsh_run_status_t res = rcsh_run_command (cmd, &ctx);
          cmd = NULL; // WARN: Do not remove for safety. See above.

          rcsh_log_trace ("Checking the result of the command");
          switch (res)
            {
            case RCSH_RUN_STATUS_SUCCESS:
              rcsh_log_trace ("Command ran successfully");
              break;
            case RCSH_RUN_STATUS_EXIT:
              rcsh_log_trace ("Command exited the shell");
              continue_loop = 0;
              break;
            case RCSH_RUN_STATUS_FAILURE:
              rcsh_log_trace ("Command failed to run");
              break;
            }
        }
      while (continue_loop);
    }

  rcsh_log_trace ("Deinitializing shell context");
  rcsh_ctx_deint (&ctx);
  return WEXITSTATUS (ctx.exit_status);
}
