#include <errno.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <rcsh_cmd.h>
#include <rcsh_log.h>

#include <rcsh_builtin.h>

rcsh_builtin_status_t
rcsh_builtin_cd_chdir (const char *path, rcsh_cmd_t *const cmd)
{
  rcsh_log_trace ("Attempting to change directory");
  int ch = chdir (path);
  if (ch < 0)
    {
      rcsh_log_error ("Unable to change directory: %s", strerror (errno));
      rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
      return RCSH_BUILTIN_STATUS_FAILURE;
    }
  rcsh_log_trace ("Changing directory succeeded");

  rcsh_log_trace ("Setting $PWD environment variable");
  int en = setenv ("PWD", path, 1);
  if (en < 0)
    {
      rcsh_log_error ("Unable to set $PWD: %s", strerror (errno));
      rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
      return RCSH_BUILTIN_STATUS_FAILURE;
    }

  rcsh_log_trace ("Successfully set $PWD variable");
  rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
  return RCSH_BUILTIN_STATUS_SUCCESS;
}

rcsh_builtin_status_t
rcsh_builtin_cd (rcsh_cmd_t *const cmd, rcsh_ctx_t *const ctx)
{
  rcsh_log_trace ("Checking parameters and argument");
  if (cmd == NULL || ctx == NULL || cmd->argc == 0 || cmd->argv == NULL)
    {
      rcsh_log_error (
          "Invalid parameters passed to function: `rcsh_builtin_cd`");
      exit (-1);
    }

  if (cmd->argc > 2)
    {
      rcsh_log_warn ("Too many arguments passed to builtin `cd`");
    }

  if (cmd->argc >= 2)
    {
      rcsh_log_trace ("Checking for valid path provided");
      struct stat buffer = { 0 };
      int st = stat (cmd->argv[1], &buffer);
      if (st < 0)
        {
          rcsh_log_error ("Unable to stat provided path: %s",
                          strerror (errno));
          rcsh_cmd_deinit ((rcsh_cmd_t **)&cmd);
          return RCSH_BUILTIN_STATUS_FAILURE;
        }

      return rcsh_builtin_cd_chdir (cmd->argv[1], cmd);
    }
  else
    {
      rcsh_log_trace ("Getting $HOME environment variable");
      char *home_env = getenv ("HOME");
      if (home_env != NULL)
        {
          return rcsh_builtin_cd_chdir (home_env, cmd);
        }

      rcsh_log_trace ("$HOME is not defined");

      rcsh_log_trace ("Getting uid and its corresponding home directory");
      uid_t uid = geteuid ();
      struct passwd *pw = getpwuid (uid);
      return rcsh_builtin_cd_chdir (pw->pw_dir, cmd);
    }
}
