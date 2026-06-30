#ifndef _KSU_MEM_HIDE_H
#define _KSU_MEM_HIDE_H

#include <linux/types.h>

void ksu_xor_transform(char *buf, size_t len);
void ksu_mem_hide_init(void);
void ksu_mem_hide_exit(void);

#endif
