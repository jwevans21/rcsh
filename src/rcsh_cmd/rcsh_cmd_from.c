#include "rcsh_ctx.h"
#define __JWEVANS__RCSH__CMD_H__INTERNAL__ 1

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <rcsh_log.h>
#include <rcsh_str.h>

#include <rcsh_cmd.h>

typedef struct rcsh_cmd_parse_ctx
{
  char *buffer;
  enum
  {
    RCSH_CMD_PARSE_NORMAL = 0,
    RCSH_CMD_PARSE_DOUBLE_QUOTE = 1,
    RCSH_CMD_PARSE_SINGLE_QUOTE = 2,
  } mode;
  rcsh_str_t *collected;
} rcsh_cmd_parse_ctx_t;

rcsh_cmd_t *
rcsh_cmd_from_file (FILE *file, rcsh_ctx_t *const ctx)
{
  char *line = NULL;
  size_t buf_size = 0;
  ssize_t len_read = getline (&line, &buf_size, file);
  rcsh_log_trace ("read %zd bytes, resulting in a buffer size of %zu bytes",
                  len_read, buf_size);
  rcsh_log_trace ("line read = \"%s\"", len_read > 0 ? line : "(null)");

  if (line == NULL)
    {
      rcsh_log_trace ("line is NULL, returning NULL");
      return NULL;
    }

  if (len_read < 0)
    {
      rcsh_log_trace ("line is empty, returning NULL");
      free (line);
      return NULL;
    }

  rcsh_cmd_t *cmd = rcsh_cmd_init ();
  if (cmd == NULL)
    {
      rcsh_log_error ("Failed to initialize a command");
      free (line);
      return NULL;
    }

  rcsh_cmd_parse (cmd, line, ctx);

  free (line);
  return cmd;
}
