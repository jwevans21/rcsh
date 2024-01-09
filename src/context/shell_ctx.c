#include <stdlib.h>
#include <unistd.h>

#include <shell_ctx.h>

shell_ctx_t *shell_ctx_new() {
    shell_ctx_t *self = malloc(sizeof(shell_ctx_t));
    if (self == NULL) {
        return NULL;
    }
    
    self->pid = getpid();
    self->ppid = getppid();
    self->last_exit_status = 0;
    
    return self;
    
}

void shell_ctx_free(shell_ctx_t *const self) {
    free(self);
}