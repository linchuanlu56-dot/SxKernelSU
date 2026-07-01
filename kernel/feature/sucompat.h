#ifndef __SKS_H_SUCOMPAT
#define __SKS_H_SUCOMPAT
#include <asm/ptrace.h>
#include <linux/types.h>

extern bool sksu_su_compat_enabled;

void sksu_sucompat_init(void);
void sksu_sucompat_exit(void);

// Handler functions exported for hook_manager
long sksu_handle_faccessat_sucompat(int orig_nr, struct pt_regs *regs);
long sksu_handle_stat_sucompat(int orig_nr, struct pt_regs *regs);
long sksu_handle_execve_sucompat(const char __user **filename_user, int orig_nr, struct pt_regs *regs);

#endif