#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <rcsh_log.h>

#include <shell_ctx.h>

shell_ctx_t *shell_ctx_new() {
    shell_ctx_t *self = malloc(sizeof(shell_ctx_t));
    if (self == NULL) {
        rcsh_error("Failed to allocate memory for shell context");
        return NULL;
    }
    
    rcsh_trace("Getting shell PID and PPID");
    self->pid = getpid();
    self->ppid = getppid();

    rcsh_trace("Opening /dev/null");
    self->null = fopen("/dev/null", "w");
    if (self->null == NULL) {
        free(self);
        rcsh_error("Failed to open /dev/null");
        return NULL;
    }

    self->last_exit_status = 0;
    
    return self;
}

void shell_ctx_free(shell_ctx_t *const self) {
    free(self);
}