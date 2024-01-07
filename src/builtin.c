
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <builtin.h>
#include <command.h>

#include <rcsh_debug.h>

command_status_t builtin_cd(const command_t *const self, int *const exit_code) {
  if (self->argc <= 1) {
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    if (pwd == NULL) {
      *exit_code = 1;
      ERROR_PERROR("getpwuid");
      return COMMAND_SUCCESS;
    }

    if (chdir(pwd->pw_dir) < 0) {
      *exit_code = 1;
      ERROR_PERROR("chdir");
      return COMMAND_SUCCESS;
    }
  } else if (self->argc == 2) {
    if (chdir(self->argv[1]) < 0) {
      *exit_code = 1;
      ERROR_PERROR("chdir");
      return COMMAND_SUCCESS;
    }
  } else {
    WARN("cd: too many arguments");
    if (chdir(self->argv[1]) < 0) {
      *exit_code = 1;
      ERROR_PERROR("chdir");
      return COMMAND_SUCCESS;
    }
  }

  *exit_code = 0;
  return COMMAND_SUCCESS;
}

command_status_t builtin_exit(const command_t *const self, int *const exit_code) {
  if (self->argc > 1) {
    *exit_code = atoi(self->argv[1]);
  }

  return COMMAND_EXIT;
}

command_status_t builtin_status(const command_t *const self, int *const exit_code) {
  fprintf(stdout, "%d\n", *exit_code);
  *exit_code = 0;
  return COMMAND_SUCCESS;
}