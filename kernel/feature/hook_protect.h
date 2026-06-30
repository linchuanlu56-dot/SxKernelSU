#ifndef _KSU_HOOK_PROTECT_H
#define _KSU_HOOK_PROTECT_H

int ksu_register_hook_protect(int syscall_nr, void *our_fn, void **orig_slot);
void ksu_hook_protect_start(void);
void ksu_hook_protect_stop(void);
void ksu_hide_kprobes(void);

#endif
