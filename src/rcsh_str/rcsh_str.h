#ifndef __JWEVANS__RCSH__STR_H_
#define __JWEVANS__RCSH__STR_H_

#define _POSIX_C_SOURCE 200809L

#include <stddef.h>

typedef struct rcsh_str
{
  char *buffer;
  size_t length;
  size_t capacity;
} rcsh_str_t;

rcsh_str_t *rcsh_str_init ();
char *rcsh_str_deinit (rcsh_str_t **const self);

void rcsh_str_append_str (rcsh_str_t *const self, char *str);
void rcsh_str_append_str_n (rcsh_str_t *const self, const char *str, size_t n);
void rcsh_str_append_char (rcsh_str_t *const self, const char c);

#endif
