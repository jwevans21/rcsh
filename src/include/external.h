#ifndef __JWEVANS__RCSH__EXTERNAL_H_
#define __JWEVANS__RCSH__EXTERNAL_H_

#include <command.h>

command_status_t external_command(const command_t *const self,
                                  int *const exit_code);

#endif