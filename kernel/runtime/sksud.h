#ifndef __SKS_H_SKSUD
#define __SKS_H_SKSUD

#include <asm/syscall.h>

#define SKSUD_PATH "/data/adb/sksd"

void sksu_sksud_init();
void sksu_sksud_exit();

void sksu_execve_hook_sksud(const struct pt_regs *regs);
void sksu_stop_input_hook_runtime(void);

#endif
