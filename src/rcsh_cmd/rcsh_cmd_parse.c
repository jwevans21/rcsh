#define __JWEVANS__RCSH__CMD_H__INTERNAL__ 1

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <rcsh_log.h>
#include <rcsh_str.h>

#include <rcsh_cmd.h>

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
rcsh_cmd_parse_env_var (char **line)
{
  rcsh_log_trace ("Consuming environment variable");

  rcsh_str_t *env_var = rcsh_str_init ();

  if (**line == '{')
    {
      rcsh_cmd_parse_env_var_braced (line, env_var);
    }
  else
    {
      rcsh_cmd_parse_env_var_normal (line, env_var);
    }

  rcsh_log_trace ("Consumed environment variable");

  char *env_var_str = rcsh_str_deinit (&env_var);
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
  return env_var_val;
}

char *
rcsh_cmd_consume_double_string (char **line)
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
          char *env_var = rcsh_cmd_parse_env_var (line);
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
rcsh_cmd_consume_normal (char **line)
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
                               rcsh_cmd_consume_double_string (line));
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
          char *env_var = rcsh_cmd_parse_env_var (line);
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
rcsh_cmd_parse (rcsh_cmd_t *const self, char *line)
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
          rcsh_cmd_add_arg (self, rcsh_cmd_consume_double_string (&line));
        }
      else if (*line == '\'')
        {
          line++;
          rcsh_cmd_add_arg (self, rcsh_cmd_consume_single_string (&line));
        }
      else if (*line == '$')
        {
          line++;
          char *env_var = rcsh_cmd_parse_env_var (&line);
          if (env_var != NULL)
            {
              rcsh_cmd_add_arg (self, strdup (env_var));
            }
          else
            {
              rcsh_log_warn ("Environment variable not found");
            }
        }
      else
        {
          rcsh_cmd_add_arg (self, rcsh_cmd_consume_normal (&line));
        }
    }
}
