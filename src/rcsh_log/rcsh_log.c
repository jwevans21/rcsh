#include <stdarg.h>
#include <stdio.h>

#include <rcsh_log.h>

volatile rcsh_log_level_t current_log_level = RCSH_LOG_LEVEL_ERROR;

const char *const NAMES[] = {
  "QUIET", "ERROR", "WARN", "INFO", "TRACE",
};
const char *const COLOR[] = {
  "\x1b[39m", "\x1b[31m", "\x1b[33m", "\x1b[36m", "\x1b[35m",
};
const char *const RESET = "\x1b[0m";

void
rcsh_log_set_level (rcsh_log_level_t level)
{
  if (level < RCSH_LOG_LEVEL_QUIET)
    {
      current_log_level = RCSH_LOG_LEVEL_QUIET;
    }
  else if (level > RCSH_LOG_LEVEL_TRACE)
    {
      current_log_level = RCSH_LOG_LEVEL_TRACE;
    }
  else
    {
      current_log_level = level;
    }
}

__attribute__ ((format (printf, 1, 2))) int
rcsh_log_error (const char *restrict format, ...)
{
  if (current_log_level < RCSH_LOG_LEVEL_ERROR)
    return 0;

  va_list args;
  va_start (args, format);

  int ret1 = fprintf (stderr, "%s%s: ", COLOR[RCSH_LOG_LEVEL_ERROR],
                      NAMES[RCSH_LOG_LEVEL_ERROR]);
  int ret2 = vfprintf (stderr, format, args);
  int ret3 = fprintf (stderr, "%s\n", RESET);

  va_end (args);

  if (ret1 < 0 || ret2 < 0 || ret3 < 0)
    {
      return -1;
    }
  else
    {
      return ret1 + ret2 + ret3;
    }
}

__attribute__ ((format (printf, 1, 2))) int
rcsh_log_warn (const char *restrict format, ...)
{
  if (current_log_level < RCSH_LOG_LEVEL_WARN)
    return 0;

  va_list args;
  va_start (args, format);

  int ret1 = fprintf (stderr, "%s%s: ", COLOR[RCSH_LOG_LEVEL_WARN],
                      NAMES[RCSH_LOG_LEVEL_WARN]);
  int ret2 = vfprintf (stderr, format, args);
  int ret3 = fprintf (stderr, "%s\n", RESET);

  va_end (args);

  if (ret1 < 0 || ret2 < 0 || ret3 < 0)
    {
      return -1;
    }
  else
    {
      return ret1 + ret2 + ret3;
    }
}

__attribute__ ((format (printf, 1, 2))) int
rcsh_log_info (const char *restrict format, ...)
{
  if (current_log_level < RCSH_LOG_LEVEL_INFO)
    return 0;

  va_list args;
  va_start (args, format);

  int ret1 = fprintf (stderr, "%s%s: ", COLOR[RCSH_LOG_LEVEL_INFO],
                      NAMES[RCSH_LOG_LEVEL_INFO]);
  int ret2 = vfprintf (stderr, format, args);
  int ret3 = fprintf (stderr, "%s\n", RESET);

  va_end (args);

  if (ret1 < 0 || ret2 < 0 || ret3 < 0)
    {
      return -1;
    }
  else
    {
      return ret1 + ret2 + ret3;
    }
}

__attribute__ ((format (printf, 1, 2))) int
rcsh_log_trace (const char *restrict format, ...)
{
  if (current_log_level < RCSH_LOG_LEVEL_TRACE)
    return 0;

  va_list args;
  va_start (args, format);

  int ret1 = fprintf (stderr, "%s%s: ", COLOR[RCSH_LOG_LEVEL_TRACE],
                      NAMES[RCSH_LOG_LEVEL_TRACE]);
  int ret2 = vfprintf (stderr, format, args);
  int ret3 = fprintf (stderr, "%s\n", RESET);

  va_end (args);

  if (ret1 < 0 || ret2 < 0 || ret3 < 0)
    {
      return -1;
    }
  else
    {
      return ret1 + ret2 + ret3;
    }
}
