#include <stdio.h>

#include <cli_args.h>
#include <rcsh_log.h>
#include <shell_ctx.h>
#include <command.h>

#include <shell_loop.h>

int shell_loop(const struct cli_args *const args) {
  rcsh_trace("Initializing shell loop");
  shell_ctx_t *ctx = shell_ctx_new();

  do {
    if (feof(stdin)) {
      rcsh_info("Reached end of the stdin stream");
      fprintf(stdout, "\n");
      return 0;
    }
    if (ferror(stdin)) {
      rcsh_error("Error reading from stdin");
      return -1;
    }

    rcsh_trace("Reading command");
    fprintf(stdout, "%d $ ", ctx->pid);
    fflush(stdout);

    command_t *cmd = command_new();
    if (command_read_from(cmd, stdin) < 0) {
      rcsh_error("Error reading command");
      return -1;
    }

    command_debug(cmd);
    
    command_status_t ret = command_run(cmd, &ctx->last_exit_status);

    switch (ret) {
    case COMMAND_SUCCESS:
      rcsh_info("Command succeeded or was executed in background");
      break;
    case COMMAND_EXEC_FAILURE:
      rcsh_error("Command failed to execute: %s\n", cmd->argv[0]);
      command_free(&cmd);
      return -1;
    case COMMAND_FAILURE:
      rcsh_error("Command failed");
      command_free(&cmd);
      break;
    case COMMAND_EXIT:
      rcsh_info("Exiting with code %d\n", ctx->last_exit_status);
      command_free(&cmd);
      return ctx->last_exit_status;
    default:
      rcsh_error("Unknown command status %d\n", ret);
      return -1;
    }

    rcsh_trace("Freeing command");
    command_free(&cmd);

  } while (1);

  return 0;
}