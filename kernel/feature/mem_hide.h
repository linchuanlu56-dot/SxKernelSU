#ifndef _KSU_MEM_HIDE_H
#define _KSU_MEM_HIDE_H

#include <linux/types.h>

void ksu_xor_encrypt(char *buf, const char *str, size_t len);
void ksu_xor_decrypt(char *buf, size_t len);
unsigned long ksu_kallsyms_lookup_name(const char *name);
void ksu_mem_hide_init(void);
void ksu_mem_hide_exit(void);

#endif
