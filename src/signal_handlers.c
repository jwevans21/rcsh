#include <errno.h>
#include <signal.h>
#include <string.h>

#include <rcsh_log.h>

#include <signal_handlers.h>

int
init_signal_handlers (void)
{
  struct sigaction SIGINT_ACTION = { 0 };
  SIGINT_ACTION.sa_handler = SIG_IGN;
  sigemptyset (&SIGINT_ACTION.sa_mask);
  SIGINT_ACTION.sa_flags = SA_RESTART;
  if (sigaction (SIGINT, &SIGINT_ACTION, NULL) == -1)
    {
      rcsh_log_error ("Failed to set SIGINT handler: %s", strerror (errno));
      return -1;
    }

  struct sigaction SIGQUIT_ACTION = { 0 };
  SIGQUIT_ACTION.sa_handler = SIG_IGN;
  sigemptyset (&SIGQUIT_ACTION.sa_mask);
  SIGQUIT_ACTION.sa_flags = SA_RESTART;
  if (sigaction (SIGQUIT, &SIGQUIT_ACTION, NULL) == -1)
    {
      rcsh_log_error ("Failed to set SIGQUIT handler: %s", strerror (errno));
      return -1;
    }

  struct sigaction SIGTSTP_ACTION = { 0 };
  SIGTSTP_ACTION.sa_handler = SIG_IGN;
  sigemptyset (&SIGTSTP_ACTION.sa_mask);
  SIGTSTP_ACTION.sa_flags = SA_RESTART;
  if (sigaction (SIGTSTP, &SIGTSTP_ACTION, NULL) == -1)
    {
      rcsh_log_error ("Failed to set SIGTSTP handler: %s", strerror (errno));
      return -1;
    }

  return 0;
}
