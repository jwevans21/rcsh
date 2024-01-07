#ifndef __JWEVANS__RCSH__DEBUG_H_
#define __JWEVANS__RCSH__DEBUG_H_

#include <errno.h>
#include <stdio.h>
#include <string.h>

static volatile enum {
  DEBUG_NONE = 0,
  DEBUG_ERROR = 1,
  DEBUG_WARN = 2,
  DEBUG_INFO = 3,
  DEBUG_TRACE = 4,
} debug_level = DEBUG_NONE;

static inline void rcsh_debug_set_level(int level) {
  if (level < DEBUG_NONE) {
    level = DEBUG_NONE;
  } else if (level > DEBUG_TRACE) {
    level = DEBUG_TRACE;
  } else {
    debug_level = level;
  }
}

#define S1(x) #x
#define S2(x) S1(x)
#define LOC __FILE__ ":" S2(__LINE__) " -> "

#define ERROR_PERROR(msg)                                                      \
  do {                                                                         \
    if (debug_level >= DEBUG_ERROR) {                                          \
      fprintf(stderr, "ERROR: " msg LOC "%s\n", strerror(errno));              \
    }                                                                          \
  } while (0)

#define ERROR(msg)                                                             \
  do {                                                                         \
    if (debug_level >= DEBUG_ERROR) {                                          \
      fprintf(stderr, "ERROR: " msg "\n");                                     \
    }                                                                          \
  } while (0)

#define ERRORV(msg, ...)                                                       \
  do {                                                                         \
    if (debug_level >= DEBUG_ERROR) {                                          \
      fprintf(stderr, "ERROR: " msg "\n", __VA_ARGS__);                        \
    }                                                                          \
  } while (0)

#define WARN(msg)                                                              \
  do {                                                                         \
    if (debug_level >= DEBUG_WARN) {                                           \
      fprintf(stderr, "WARN: " msg "\n");                                      \
    }                                                                          \
  } while (0)

#define WARNV(msg, ...)                                                        \
  do {                                                                         \
    if (debug_level >= DEBUG_WARN) {                                           \
      fprintf(stderr, "WARN: " msg "\n", __VA_ARGS__);                         \
    }                                                                          \
  } while (0)

#define INFO(msg)                                                              \
  do {                                                                         \
    if (debug_level >= DEBUG_INFO) {                                           \
      fprintf(stderr, "INFO: " msg "\n");                                      \
    }                                                                          \
  } while (0)

#define INFOV(msg, ...)                                                        \
  do {                                                                         \
    if (debug_level >= DEBUG_INFO) {                                           \
      fprintf(stderr, "INFO: " msg "\n", __VA_ARGS__);                         \
    }                                                                          \
  } while (0)

#define TRACE(msg)                                                             \
  do {                                                                         \
    if (debug_level >= DEBUG_TRACE) {                                          \
      fprintf(stderr, "TRACE: " msg "\n");                                     \
    }                                                                          \
  } while (0)

#define TRACEV(msg, ...)                                                       \
  do {                                                                         \
    if (debug_level >= DEBUG_TRACE) {                                          \
      fprintf(stderr, "TRACE: " msg "\n", __VA_ARGS__);                        \
    }                                                                          \
  } while (0)

#endif
