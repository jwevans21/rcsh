#ifndef __JWEVANS__RCSH__SHELL_CTX_H_
#define __JWEVANS__RCSH__SHELL_CTX_H_

#include <stdio.h>
#include <sys/types.h>

typedef struct {
  pid_t pid;
  pid_t ppid;
  FILE* null;
  int last_exit_status;
} shell_ctx_t;

shell_ctx_t *shell_ctx_new();
void shell_ctx_free(shell_ctx_t *const self);

#endif