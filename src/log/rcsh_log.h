#ifndef __JWEVANS__RCSH__LOG_H_
#define __JWEVANS__RCSH__LOG_H_

#define S1(x) #x
#define S2(x) S1(x)
#define LOC __FILE__ ":" S2(__LINE__) " -> "


typedef enum {
  RCSH_LOG_QUIET = 0,
  RCSH_LOG_ERROR = 1,
  RCSH_LOG_WARN = 2,
  RCSH_LOG_INFO = 3,
  RCSH_LOG_TRACE = 4,
} rcsh_log_level_t;

void rcsh_log_set_level(const rcsh_log_level_t level);

__attribute__((format(printf, 1, 2))) void rcsh_error(const char *msg, ...);
__attribute__((format(printf, 1, 2))) void rcsh_warn(const char *msg, ...);
__attribute__((format(printf, 1, 2))) void rcsh_info(const char *msg, ...);
__attribute__((format(printf, 1, 2))) void rcsh_trace(const char *msg, ...);

__attribute__((noreturn, format(printf, 2, 3))) void
rcsh_fatal(int code, const char *msg, ...);

#endif