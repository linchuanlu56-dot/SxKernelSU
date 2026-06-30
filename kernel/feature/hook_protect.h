#ifndef _KSU_HOOK_PROTECT_H
#define _KSU_HOOK_PROTECT_H

int ksu_register_hook_slot(void **target, void *expected);
void ksu_hook_protect_start(void);
void ksu_hook_protect_stop(void);
void ksu_hide_kprobes(void);

#endif
