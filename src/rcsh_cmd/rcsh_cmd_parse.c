#include <stdio.h>
#define __JWEVANS__RCSH__CMD_H__INTERNAL__ 1

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <rcsh_ctx.h>
#include <rcsh_log.h>
#include <rcsh_str.h>

#include <rcsh_cmd.h>

int
rcsh_cmd_set_input_file (rcsh_cmd_t *const self, char *fname)
{
  if (self == NULL || fname == NULL || *fname == '\0')
    {
      return -1;
    }

  if (self->input != NULL)
    {
      fclose (self->input);
      rcsh_log_warn ("Input file already set, closing previous file");
    }

  self->input = fopen (fname, "r");

  if (self->input == NULL || ferror (self->input))
    {
      rcsh_log_error ("Failed to open input file \"%s\"", fname);
      return -1;
    }

  return 0;
}

int
rcsh_cmd_set_output_file (rcsh_cmd_t *const self, char *fname)
{
  if (self == NULL || fname == NULL || *fname == '\0')
    {
      return -1;
    }

  if (self->output != NULL)
    {
      fclose (self->output);
      rcsh_log_warn ("Output file already set, closing previous file");
    }

  self->output = fopen (fname, "w");

  if (self->output == NULL || ferror (self->output))
    {
      rcsh_log_error ("Failed to open output file \"%s\"", fname);
      return -1;
    }

  return 0;
}

char
rcsh_cmd_parse_escape (char **line)
{
  rcsh_log_trace ("Parsing escape character");
  switch (**line)
    {
    case '"':
      rcsh_log_trace ("Found double quote");
      return '"';
    case '`':
      rcsh_log_trace ("Found backtick");
      return '`';
    case '$':
      rcsh_log_trace ("Found dollar sign");
      return '$';
    case '\\':
      rcsh_log_trace ("Found backslash");
      return '\\';
    case '\n':
      rcsh_log_trace ("Found newline");
      return '\n';
    default:
      rcsh_log_trace ("Found unknown escape character");
      return **line;
    }
}

void
rcsh_cmd_parse_env_var_braced (char **line, rcsh_str_t *env_var)
{
  rcsh_log_trace ("Consuming braced environment variable");
  (*line)++;
  while (**line != '\0' && **line != '=' && **line != '}')
    {
      rcsh_log_trace ("Found normal character");
      rcsh_str_append_char (env_var, **line);
      (*line)++;
    }
  (*line)++;

  rcsh_log_trace ("Consumed braced environment variable");
}

void
rcsh_cmd_parse_env_var_normal (char **line, rcsh_str_t *env_var)
{
  rcsh_log_trace ("Consuming normal environment variable");
  while (**line != '\0' && (isalnum (**line) || **line == '_'))
    {
      // rcsh_log_trace ("Found normal character");
      rcsh_str_append_char (env_var, **line);
      (*line)++;
    }
  rcsh_log_trace ("Consumed normal environment variable");
}

char *
rcsh_cmd_parse_env_var (char **line, rcsh_ctx_t *const ctx)
{
  rcsh_log_trace ("Consuming environment variable");

  rcsh_str_t *env_var = rcsh_str_init ();

  if (**line == '{')
    {
      rcsh_cmd_parse_env_var_braced (line, env_var);
    }
  else if (**line == '?')
    {
      (*line)++;
      rcsh_str_append_char (env_var, '?');
    }
  else
    {
      rcsh_cmd_parse_env_var_normal (line, env_var);
    }

  rcsh_log_trace ("Consumed environment variable");

  char *env_var_str = rcsh_str_deinit (&env_var);
  if (env_var_str == NULL)
    {
      rcsh_log_trace ("Environment variable string was NULL");
      return NULL;
    }

  if (strcasecmp ("PID", env_var_str) == 0)
    {
      free (env_var_str);
      char *pid_str = calloc (32, sizeof (char));
      sprintf (pid_str, "%d", ctx->pid);
      return pid_str;
    }
  else if (strcasecmp ("PPID", env_var_str) == 0)
    {
      free (env_var_str);
      char *ppid_str = calloc (32, sizeof (char));
      sprintf (ppid_str, "%d", ctx->ppid);
      return ppid_str;
    }
  else if (strcmp ("?", env_var_str) == 0)
    {
      free (env_var_str);
      char *status_str = calloc (32, sizeof (char));
      sprintf (status_str, "%d", WEXITSTATUS (ctx->exit_status));
      return status_str;
    }

  char *env_var_val = getenv (env_var_str);

  if (env_var_val == NULL)
    {
      rcsh_log_trace ("Environment variable \"%s\" not found", env_var_str);
      free (env_var_str);
      return NULL;
    }

  rcsh_log_trace ("Environment variable \"%s\" found with value \"%s\"",
                  env_var_str, env_var_val);
  free (env_var_str);
  return strdup (env_var_val);
}

char *
rcsh_cmd_consume_double_string (char **line, rcsh_ctx_t *const ctx)
{
  rcsh_log_trace ("Consuming double quoted string");
  rcsh_str_t *collected = rcsh_str_init ();

  while (**line != '\0')
    {
      if (**line == '\\')
        {
          // rcsh_log_trace ("Found escape character");
          (*line)++;
          rcsh_str_append_char (collected, rcsh_cmd_parse_escape (line));
          (*line)++;
        }
      else if (**line == '"')
        {
          // rcsh_log_trace ("Found closing double quote");
          (*line)++;
          break;
        }
      else if (**line == '$')
        {
          // rcsh_log_trace ("Found environment variable");
          (*line)++;
          char *env_var = rcsh_cmd_parse_env_var (line, ctx);
          rcsh_str_append_str (collected, env_var);
        }
      else
        {
          // rcsh_log_trace ("Found normal character");
          rcsh_str_append_char (collected, **line);
          (*line)++;
        }
    }

  rcsh_log_trace ("Consumed double quoted string");

  return rcsh_str_deinit (&collected);
}

char *
rcsh_cmd_consume_single_string (char **line)
{
  rcsh_log_trace ("Consuming single quoted string");
  rcsh_str_t *collected = rcsh_str_init ();

  while (**line != '\0')
    {
      if (**line == '\'')
        {
          // rcsh_log_trace ("Found closing single quote");
          (*line)++;
          break;
        }
      else
        {
          // rcsh_log_trace ("Found normal character");
          rcsh_str_append_char (collected, **line);
          (*line)++;
        }
    }

  rcsh_log_trace ("Consumed single quoted string");

  return rcsh_str_deinit (&collected);
}

char *
rcsh_cmd_consume_normal (char **line, rcsh_ctx_t *const ctx)
{
  rcsh_log_trace ("Consuming normal string");
  rcsh_str_t *collected = rcsh_str_init ();

  while (**line != '\0')
    {
      if (isspace (**line))
        {
          // rcsh_log_trace ("Found whitespace");
          (*line)++;
          break;
        }
      else if (**line == '"')
        {
          // rcsh_log_trace ("Found double quote");
          (*line)++;
          rcsh_str_append_str (collected,
                               rcsh_cmd_consume_double_string (line, ctx));
        }
      else if (**line == '\'')
        {
          // rcsh_log_trace ("Found single quote");
          (*line)++;
          rcsh_str_append_str (collected,
                               rcsh_cmd_consume_single_string (line));
        }
      else if (**line == '$')
        {
          // rcsh_log_trace ("Found environment variable");
          (*line)++;
          char *env_var = rcsh_cmd_parse_env_var (line, ctx);
          rcsh_str_append_str (collected, env_var);
        }
      else if (**line == '#')
        {
          rcsh_log_trace ("Found comment");
          (*line)++;
          while (**line != '\0' && **line != '\n')
            {
              (*line)++;
            }
        }
      else
        {
          // rcsh_log_trace ("Found normal character");
          rcsh_str_append_char (collected, **line);
          (*line)++;
        }
    }

  rcsh_log_trace ("Consumed normal string");

  return rcsh_str_deinit (&collected);
}

void
rcsh_cmd_parse (rcsh_cmd_t *const self, char *line, rcsh_ctx_t *const ctx)
{
  if (self == NULL || line == NULL)
    {
      return;
    }

  rcsh_log_trace ("Parsing command line \"%s\"", line);

  while (*line != '\0')
    {
      if (isspace (*line))
        {
          line++;
        }
      else if (*line == '"')
        {
          line++;
          rcsh_cmd_add_arg (self, rcsh_cmd_consume_double_string (&line, ctx));
        }
      else if (*line == '\'')
        {
          line++;
          rcsh_cmd_add_arg (self, rcsh_cmd_consume_single_string (&line));
        }
      else if (*line == '$')
        {
          line++;
          char *env_var = rcsh_cmd_parse_env_var (&line, ctx);
          if (env_var != NULL)
            {
              rcsh_cmd_add_arg (self, env_var);
            }
          else
            {
              rcsh_log_warn ("Environment variable not found");
            }
        }
      else if (*line == '#')
        {
          rcsh_log_trace ("Found comment");
          line++;
          while (*line != '\0' && *line != '\n')
            {
              line++;
            }
        }
      else if (*line == '<')
        {
          rcsh_log_trace ("Found input redirection");
          line++;
          char *fname = rcsh_cmd_consume_normal (&line, ctx);
          if (rcsh_cmd_set_input_file (self, fname) == -1)
            {
              rcsh_log_error ("Failed to set input file");
            }
        }
      else if (*line == '>')
        {
          rcsh_log_trace ("Found output redirection");
          line++;
          char *fname = rcsh_cmd_consume_normal (&line, ctx);
          if (rcsh_cmd_set_output_file (self, fname) == -1)
            {
              rcsh_log_error ("Failed to set output file");
            }
        }
      else if (*line == '&')
        {
          rcsh_log_trace ("Found background process");
          line++;
          self->background = 1;

          // Consume any whitespace
          while (*line != '\0' && isspace (*line))
            {
              line++;
            }

          // Consume any comments
          if (*line == '#')
            {
              rcsh_log_trace ("Found comment");
              line++;
              while (*line != '\0' && *line != '\n')
                {
                  line++;
                }
            }

          // Consume any whitespace
          while (*line != '\0' && isspace (*line))
            {
              line++;
            }

          if (*line != '\0')
            {
              rcsh_log_error ("Background process must be last command");
              return;
            }
        }
      else
        {
          rcsh_cmd_add_arg (self, rcsh_cmd_consume_normal (&line, ctx));
        }
    }
}
