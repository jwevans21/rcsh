#ifndef __JWEVANS__RCSH__BUILTIN_H_
#define __JWEVANS__RCSH__BUILTIN_H_

#include <rcsh_cmd.h>
#include <rcsh_ctx.h>
#include <rcsh_log.h>

typedef enum rcsh_builtin_status
{
  RCSH_BUILTIN_STATUS_SUCCESS,
  RCSH_BUILTIN_STATUS_FAILURE,
  RCSH_BUILTIN_STATUS_EXIT,
} rcsh_builtin_status_t;

rcsh_builtin_status_t rcsh_builtin_cd (rcsh_cmd_t *const cmd,
                                       rcsh_ctx_t *const ctx);

rcsh_builtin_status_t rcsh_builtin_exit (rcsh_cmd_t *const cmd,
                                         rcsh_ctx_t *const ctx);

rcsh_builtin_status_t rcsh_builtin_status (rcsh_cmd_t *const cmd,
                                           rcsh_ctx_t *const ctx);

#endif
