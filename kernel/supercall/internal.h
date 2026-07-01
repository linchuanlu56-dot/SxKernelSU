#ifndef __SKS_H_SUPERCALL_INTERNAL
#define __SKS_H_SUPERCALL_INTERNAL

#include <linux/types.h>
#include <linux/uaccess.h>

bool only_manager(void);
bool only_root(void);
bool manager_or_root(void);
bool always_allow(void);
bool allowed_for_su(void);

long sksu_supercall_handle_ioctl(unsigned int cmd, void __user *argp);
void sksu_supercall_dump_commands(void);
void sksu_supercall_cleanup_state(void);

#endif // __SKS_H_SUPERCALL_INTERNAL
