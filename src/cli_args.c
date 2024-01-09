#include <getopt.h>
#include <stdint.h>
#include <stdio.h>

#include <cli_args.h>

const char *const USAGE =
    "Usage: %s [OPTION]... [FILE]\n"
    "\n"
    "Options:\n"
    "  -c COMMAND\texecute command\n"
    "\n"
    "  -v, --verbose\t\tverbose mode use more v's to increase level (e.g. "
    "-vvv)\n"
    "  -h, --help\t\tdisplay this help and exit\n"
    "  -V, --version\t\tdisplay version information and exit\n";

const char *const VERSION =
    "%s 0.0.1\n"
    "\n"
    "Author: Jacob Evans <jacob@jwevans.dev>\n"
    "License: GPL-3.0-or-later\n"
    "\n"
    "For source code and bug reports see <https://github.com/jwevans21/rcsh>\n"
    "\n";

int handle_long_option(struct cli_args *const args, int *const ret,
                       int option_index, const char *const program_name) {
  switch (option_index) {
  case 0:
    // --command
    args->has_command = 1;
    args->command = optarg;
    break;
  case 1:
    // --verbose
    args->verbose += 1;
    break;
  case 2:
    // --quiet
    args->quiet = 1;
    break;
  case 3:
    // --help
    print_help(program_name);
    args->help = 1;
    *ret = 0;
    return 1;
  case 4:
    // --version
    print_version(program_name);
    args->version = 1;
    *ret = 0;
    return 1;
  default:
    fprintf(stderr, "Unknown option %s\n\n", optarg);
    args->help = 1;
    *ret = 1;
    return 1;
  }

  return 0;
}

int parse_cli_args(struct cli_args *const args, int *const ret, int argc,
                   char *argv[]) {
  int opt, option_index;

  while ((opt = getopt_long(argc, argv, "c:vqh?V", CLI_ARGS_OPTIONS,
                            &option_index)) != -1) {
    switch (opt) {
    case 0:
      if (handle_long_option(args, ret, option_index, argv[0]) == 1) {
        return 1;
      }
      break;
    case 'c':
      args->has_command = 1;
      args->command = optarg;
      break;
    case 'v':
      args->verbose += 1;
      break;
    case 'q':
      args->quiet = 1;
      break;
    case 'h':
    case '?':
      print_help(argv[0]);
      args->help = 1;
      *ret = 0;
      return 1;
    case 'V':
      print_version(argv[0]);
      args->version = 1;
      *ret = 0;
      return 1;
    default:
      fprintf(stderr, "Unknown option %s\n\n", optarg);
      print_help(argv[0]);
      args->help = 1;
      *ret = 1;
      return 1;
    }
  }

  if (optind < argc) {
    args->has_file = 1;
    args->file = argv[optind];
    optind += 1;
  }

  if (optind < argc) {
    args->too_many_files = 1;
    return 1;
  }

  return 0;
}

void print_help(const char *const program_name) {
  fprintf(stderr, USAGE, program_name);
}

void print_version(const char *const program_name) {
  fprintf(stderr, VERSION, program_name);
}