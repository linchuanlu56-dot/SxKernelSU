#ifndef __SKS_H_SKS_CORE
#define __SKS_H_SKS_CORE

#include <linux/init.h>
#include <linux/types.h>

void sksu_setuid_hook_init(void);
void sksu_setuid_hook_exit(void);

// Handler functions for hook_manager
int sksu_handle_setresuid(uid_t old_uid, uid_t new_uid);

#endif
