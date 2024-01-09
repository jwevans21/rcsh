#include "cli_args.h"
#include "shell_loop.h"
#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

#include <command.h>

#include <rcsh_log.h>

int main(int argc, char *argv[]) {
  struct cli_args args = CLI_ARGS_DEFAULTS;

  int ret = 0;
  int leave = parse_cli_args(&args, &ret, argc, argv);

  if (leave) {
    return ret;
  }

  if (!args.quiet) {
    rcsh_log_set_level(args.verbose);
  }

  if (args.has_command) {

  } else if (args.has_file) {

  } else {
    return shell_loop(&args);
  }

  return -1;
}
