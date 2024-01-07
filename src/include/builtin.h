#ifndef __JWEVANS__RCSH__BUILTIN_H_
#define __JWEVANS__RCSH__BUILTIN_H_

#include <command.h>

command_status_t builtin_cd(const command_t *const self, int *const exit_code);
command_status_t builtin_exit(const command_t *const self, int *const exit_code);
command_status_t builtin_status(const command_t *const self, int *const exit_code);


#endif
