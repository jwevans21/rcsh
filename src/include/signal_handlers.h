#ifndef __JWEVANS__RCSH__SIGNAL_HANDLERS_H_
#define __JWEVANS__RCSH__SIGNAL_HANDLERS_H_

#include <signal.h>
#include <stdlib.h>

// // #ifdef __JWEVANS__RCSH__SIGNAL_HANDLERS_H_INTERNAL_
// const struct sigaction SIGINT_ACTION = {
//   .sa_handler = SIG_IGN,
//   .sa_mask = { 0 },
//   .sa_flags = SA_RESTART,
//   .sa_restorer = NULL,
// };
// sigemptyset(sigset_t *set)

// const struct sigaction SIGQUIT_ACTION = {
//   .sa_handler = SIG_IGN,
//   .sa_mask = { 0 },
//   .sa_flags = SA_RESTART,
//   .sa_restorer = NULL,
// };

// const struct sigaction SIGTSTP_ACTION = {
//   .sa_handler = SIG_IGN,
//   .sa_mask = { 0 },
//   .sa_flags = SA_RESTART,
//   .sa_restorer = NULL,
// };

// const struct sigaction SIGSTOP_ACTION = {
//   .sa_handler = SIG_IGN,
//   .sa_mask = { 0 },
//   .sa_flags = SA_RESTART,
//   .sa_restorer = NULL,
// };
// // #endif

int init_signal_handlers (void);

#endif
