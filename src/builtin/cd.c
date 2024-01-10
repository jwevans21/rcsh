#include <errno.h>
#include <linux/limits.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <rcsh_log.h>

#include <builtin.h>

command_status_t builtin_cd(const command_t *const self, int *const exit_code) {
  if (self->argc <= 1) {
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);
    if (pwd == NULL) {
      *exit_code = 1;
      rcsh_error("rcsh: getpwuid: %s", strerror(errno));
      return COMMAND_SUCCESS;
    }

    if (chdir(pwd->pw_dir) < 0) {
      *exit_code = 1;
      rcsh_error("rcsh: chdir: %s", strerror(errno));
      return COMMAND_SUCCESS;
    }
  } else if (self->argc == 2) {
    if (chdir(self->argv[1]) < 0) {
      *exit_code = 1;
      rcsh_error("rcsh: chdir: %s", strerror(errno));
      return COMMAND_SUCCESS;
    }
  } else {
    rcsh_warn("rcsh: cd: too many arguments");
    if (chdir(self->argv[1]) < 0) {
      *exit_code = 1;
      rcsh_error("rcsh: chdir: %s", strerror(errno));
      return COMMAND_SUCCESS;
    }
  }

  char pwd[PATH_MAX] = {0};


  setenv("PWD", getcwd(pwd, PATH_MAX), 1);

  *exit_code = 0;
  return COMMAND_SUCCESS;
}