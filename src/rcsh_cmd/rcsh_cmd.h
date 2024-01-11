#ifndef __JWEVANS__RCSH__CMD_H_
#define __JWEVANS__RCSH__CMD_H_

#include <stddef.h>
#include <stdio.h>

// typedef struct rcsh_arg {
//   enum {
//     RCSH_ARG_PARAMETER_EXPANSION,
//     RCSH_ARG_COMMAND_SUBSTITUTION,
//     RCSH_ARG_ARITHMETIC_EXPANSION,
//     RCSH_ARG_NORMAL,
//     RCSH_ARG_COMBINATION,
//   } type;
//   union {
//     struct {
//       char *name;
//     } parameter_expansion;
//     struct {
//       rcsh_cmd_t *cmd;
//     } command_substitution;
//     struct {
//       char *expression;
//     } arithmetic_expansion;
//     struct {
//       char *string;
//     } normal;
//     struct {
//       struct rcsh_arg *args;
//       size_t argc;
//     } combination;
//   } data;
// } rcsh_arg_t;

typedef struct rcsh_cmd
{
  size_t argv_capacity;
  size_t argc;
  char **argv;
} rcsh_cmd_t;

rcsh_cmd_t *rcsh_cmd_init (void);
void rcsh_cmd_deinit (rcsh_cmd_t **const self);
void rcsh_cmd_debug (const rcsh_cmd_t *const self);

rcsh_cmd_t *rcsh_cmd_from_file (FILE *file);

#ifdef __JWEVANS__RCSH__CMD_H__INTERNAL__

#include <rcsh_str.h>

void rcsh_cmd_add_arg (rcsh_cmd_t *const self, char *arg);
void rcsh_cmd_grow_argv (rcsh_cmd_t *const self);

char rcsh_cmd_parse_escape (char **line);
void rcsh_cmd_parse_env_var_braced (char **line, rcsh_str_t *env_var);
void rcsh_cmd_parse_env_var_normal (char **line, rcsh_str_t *env_var);
char *rcsh_cmd_parse_env_var (char **line);
char *rcsh_cmd_consume_double_string (char **line);
char *rcsh_cmd_consume_single_string (char **line);
char *rcsh_cmd_consume_normal (char **line);
void rcsh_cmd_parse (rcsh_cmd_t *const self, char *line);

#endif // __JWEVANS__RCSH__CMD_H__INTERNAL__

#endif // __JWEVANS__RCSH__CMD_H_
