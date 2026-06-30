#ifndef _KSU_TIMING_HIDE_H
#define _KSU_TIMING_HIDE_H

#include <linux/types.h>

void ksu_timing_apply_jitter(struct timespec64 *ts);
void ksu_timing_hide_enable(bool enable);
void ksu_timing_hide_init(void);
void ksu_timing_hide_exit(void);

#endif
