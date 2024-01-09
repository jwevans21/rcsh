#ifndef __JWEVANS__RCSH__CLI_ARGS_H_
#define __JWEVANS__RCSH__CLI_ARGS_H_

#include <getopt.h>
#include <stdlib.h>

struct cli_args {
  int verbose;
  int quiet;
  int version;
  int help;

  int has_command;
  char *command;

  int has_file;
  char *file;
  int too_many_files;
};

static const struct cli_args CLI_ARGS_DEFAULTS = {
    .verbose = 1,
    .quiet = 0,
    .version = 0,
    .help = 0,

    .has_command = 0,
    .command = NULL,

    .has_file = 0,
    .file = NULL,
    .too_many_files = 0,
};

static const struct option CLI_ARGS_OPTIONS[] = {
    {"command", required_argument, NULL, 'c'},
    {"verbose", no_argument, NULL, 'v'},
    {"quiet", no_argument, NULL, 'q'},
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'V'},
    {NULL, 0, NULL, 0},
};

int parse_cli_args(struct cli_args *const args, int *const ret, int argc,
                   char *argv[]);

void print_help(const char *const program_name);
void print_version(const char *const program_name);

#endif