#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <rcsh_log.h>

static const char *const rcsh_log_level_names[] = {
    "QUIET", "ERROR", "WARN", "INFO", "TRACE", "FATAL",
};

static const char *const rcsh_log_level_colors[] = {
    // QUIET - Default
    "\x1b[39m",
    // ERROR - Red
    "\x1b[31m",
    // WARN - Yellow
    "\x1b[33m",
    // INFO - Cyan
    "\x1b[36m",
    // TRACE - Magenta
    "\x1b[35m",
    // FATAL - Red
    "\x1b[31m",
};

static const char *const rcsh_log_level_reset = "\x1b[0m";

volatile rcsh_log_level_t rcsh_log_level = RCSH_LOG_ERROR;

void rcsh_log_set_level(const rcsh_log_level_t level) {
  if (level > RCSH_LOG_TRACE) {
    rcsh_log_level = RCSH_LOG_TRACE;
  } else if (level < RCSH_LOG_QUIET) {
    rcsh_log_level = RCSH_LOG_QUIET;
  } else {
    rcsh_log_level = level;
  }
}

__attribute__((format(printf, 1, 2))) void rcsh_error(const char *msg, ...) {
  if (rcsh_log_level >= RCSH_LOG_ERROR) {
    va_list args;
    va_start(args, msg);

    fprintf(stderr, "%s%s: ", rcsh_log_level_colors[RCSH_LOG_ERROR],
            rcsh_log_level_names[RCSH_LOG_ERROR]);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "%s\n", rcsh_log_level_reset);

    va_end(args);
  }
}

__attribute__((format(printf, 1, 2))) void rcsh_warn(const char *msg, ...) {
  if (rcsh_log_level >= RCSH_LOG_WARN) {
    va_list args;
    va_start(args, msg);

    fprintf(stderr, "%s%s: ", rcsh_log_level_colors[RCSH_LOG_WARN],
            rcsh_log_level_names[RCSH_LOG_WARN]);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "%s\n", rcsh_log_level_reset);

    va_end(args);
  }
}

__attribute__((format(printf, 1, 2))) void rcsh_info(const char *msg, ...) {
  if (rcsh_log_level >= RCSH_LOG_INFO) {
    va_list args;
    va_start(args, msg);

    fprintf(stderr, "%s%s: ", rcsh_log_level_colors[RCSH_LOG_INFO],
            rcsh_log_level_names[RCSH_LOG_INFO]);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "%s\n", rcsh_log_level_reset);

    va_end(args);
  }
}

__attribute__((format(printf, 1, 2))) void rcsh_trace(const char *msg, ...) {
  if (rcsh_log_level >= RCSH_LOG_TRACE) {
    va_list args;
    va_start(args, msg);

    fprintf(stderr, "%s%s: ", rcsh_log_level_colors[RCSH_LOG_TRACE],
            rcsh_log_level_names[RCSH_LOG_TRACE]);
    vfprintf(stderr, msg, args);
    fprintf(stderr, "%s\n", rcsh_log_level_reset);

    va_end(args);
  }
}

__attribute__((noreturn, format(printf, 2, 3))) void
rcsh_fatal(int code, const char *msg, ...) {
  va_list args;
  va_start(args, msg);

  fprintf(stderr, "%s%s: ", rcsh_log_level_colors[RCSH_LOG_ERROR], "FATAL");
  vfprintf(stderr, msg, args);
  fprintf(stderr, "%s\n", rcsh_log_level_reset);

  va_end(args);

  exit(code);
}
