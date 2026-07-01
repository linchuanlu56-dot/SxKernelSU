#ifndef __SKS_H_SYSCALL_EVENT_BRIDGE
#define __SKS_H_SYSCALL_EVENT_BRIDGE

#include <asm/ptrace.h>

long sksu_hook_newfstatat(int orig_nr, const struct pt_regs *regs);
long sksu_hook_faccessat(int orig_nr, const struct pt_regs *regs);
long sksu_hook_execve(int orig_nr, const struct pt_regs *regs);
long sksu_hook_setresuid(int orig_nr, const struct pt_regs *regs);

void sksu_stop_sksud_execve_hook(void);

#endif // __SKS_H_SYSCALL_EVENT_BRIDGE
