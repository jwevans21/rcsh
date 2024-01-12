#ifndef __JWEVANS__RCSH__CONTEXT_H_
#define __JWEVANS__RCSH__CONTEXT_H_

#include <signal.h>
#include <stdio.h>

typedef struct job
{
  pid_t pid;
  char *cmd;
  struct job *next;
} job_t;

typedef struct rcsh_context
{
  /// The pid of the parent process
  pid_t ppid;
  /// The pid of the current shell process
  pid_t pid;
  /// The file `/dev/null` used for default background process io redirection
  FILE *null;
  /// The job list for the shell
  job_t *jobs;
  /// The exit status of the last exited foreground process.
  /// This is directly from the `waitpid` function so it can be used to
  /// determine the exit signal as well.
  int exit_status;
} rcsh_ctx_t;

/// Initialize the context for the shell. This ensures that the values are set
/// appropriately for later use.
///
/// @param self The context to initialize
void rcsh_ctx_init (rcsh_ctx_t *const self);
/// Deinitializes the context of the shell. This will stop any background
/// processes, free any used memory, and close opened files.
///
/// It is still safe to access not allocated or open members after calling this
/// function.
///
/// @param self The content to deinitialize
void rcsh_ctx_deint (rcsh_ctx_t *const self);

/// Adds a job to the job list of the shell. This will allocate memory for the
/// job and copy the command string.
///
/// @param self The context to add the job to
/// @param pid The pid of the job
/// @param cmd The command string of the job
void rcsh_ctx_add_job (rcsh_ctx_t *const self, pid_t pid,
                       const char *const cmd);

void rcsh_ctx_harvest_jobs (rcsh_ctx_t *const self);

#endif
