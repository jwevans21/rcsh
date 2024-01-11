#ifndef __JWEVANS__RCSH__LOG_H_
#define __JWEVANS__RCSH__LOG_H_

#define _POSIX_C_SOURCE 200809L

typedef enum rcsh_log_level
{
  RCSH_LOG_LEVEL_QUIET = 0,
  RCSH_LOG_LEVEL_ERROR = 1,
  RCSH_LOG_LEVEL_WARN = 2,
  RCSH_LOG_LEVEL_INFO = 3,
  RCSH_LOG_LEVEL_TRACE = 4,
} rcsh_log_level_t;

void rcsh_log_set_level (rcsh_log_level_t level);

__attribute__ ((format (printf, 1, 2))) int
rcsh_log_error (const char *restrict format, ...);

__attribute__ ((format (printf, 1, 2))) int
rcsh_log_warn (const char *restrict format, ...);

__attribute__ ((format (printf, 1, 2))) int
rcsh_log_info (const char *restrict format, ...);

__attribute__ ((format (printf, 1, 2))) int
rcsh_log_trace (const char *restrict format, ...);

#endif
