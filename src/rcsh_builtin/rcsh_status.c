#include <stdio.h>

#include <rcsh_log.h>

#include <rcsh_builtin.h>

rcsh_builtin_status_t
rcsh_builtin_status (rcsh_cmd_t *const cmd, rcsh_ctx_t *const ctx)
{
  if (cmd == NULL || ctx == NULL)
    {
      rcsh_log_error ("Builtin command `status` got invalid arguments");
      return RCSH_BUILTIN_STATUS_FAILURE;
    }

  fprintf (stdout, "\n");

  return RCSH_BUILTIN_STATUS_SUCCESS;
}
