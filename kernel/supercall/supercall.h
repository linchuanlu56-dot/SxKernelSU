#ifndef __SKS_H_SUPERCALL
#define __SKS_H_SUPERCALL

#include <linux/types.h>
#include <linux/uaccess.h>

// IOCTL handler types
typedef int (*sksu_ioctl_handler_t)(void __user *arg);
typedef bool (*sksu_perm_check_t)(void);

// IOCTL command mapping
struct sksu_ioctl_cmd_map {
    unsigned int cmd;
    const char *name;
    sksu_ioctl_handler_t handler;
    sksu_perm_check_t perm_check; // Permission check function
};

// Install KSU fd to current process
int sksu_install_fd(void);

void sksu_supercalls_init(void);
void sksu_supercalls_exit(void);
#endif // __SKS_H_SUPERCALL
