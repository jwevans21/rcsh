#ifndef __JWEVANS__RCSH__RUN_H_
#define __JWEVANS__RCSH__RUN_H_

#include <rcsh_cmd.h>
#include <rcsh_ctx.h>

typedef enum rcsh_run_status
{
  RCSH_RUN_STATUS_SUCCESS,
  RCSH_RUN_STATUS_FAILURE,
  RCSH_RUN_STATUS_EXIT,
} rcsh_run_status_t;

rcsh_run_status_t rcsh_run_command (rcsh_cmd_t *const cmd,
                                    rcsh_ctx_t *const ctx);

rcsh_run_status_t rcsh_run_external (rcsh_cmd_t *const cmd,
                                     rcsh_ctx_t *const ctx);

#endif
