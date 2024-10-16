#include <stdio.h>

#include <rcsh_cmd.h>
#include <rcsh_log.h>

#include <rcsh_builtin.h>

rcsh_builtin_status_t
rcsh_builtin_help (rcsh_cmd_t *const cmd, rcsh_ctx_t *const ctx)
{
  (void)cmd;
  (void)ctx;

  rcsh_log_trace ("Displaying help message");

  printf ("rcsh - A simple Linux shell\n");
  printf ("Built-in Commands:\n");
  printf ("  cd [dir] - Change the current working directory\n");
  printf ("  exit [code] - Exit the shell with an optional exit code\n");
  printf ("  help - Display this help message\n");
  printf ("  jobs - List all background jobs\n");
  printf ("  fg [job] - Bring a background job to the foreground\n");
  printf ("  exec [command] - Execute a command without forking\n");

  return RCSH_BUILTIN_STATUS_SUCCESS;
}
