#ifndef __SKS_H_ADB_ROOT
#define __SKS_H_ADB_ROOT
#include <asm/ptrace.h>

long sksu_adb_root_handle_execve(struct pt_regs *regs);

void sksu_adb_root_init(void);

void sksu_adb_root_exit(void);

#endif
