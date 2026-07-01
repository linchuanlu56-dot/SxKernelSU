#ifndef __SKS_H_HOOK_MANAGER
#define __SKS_H_HOOK_MANAGER

#include <asm/ptrace.h>

// Hook manager initialization and cleanup
void sksu_syscall_hook_manager_init(void);
void sksu_syscall_hook_manager_exit(void);

#endif
