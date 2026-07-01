#ifndef __SKS_H_SKSUD_BOOT
#define __SKS_H_SKSUD_BOOT

#include <linux/types.h>

void on_post_fs_data(void);
void on_module_mounted(void);
void on_boot_completed(void);

bool sksu_is_safe_mode(void);

int nuke_ext4_sysfs(const char *mnt);

extern bool sksu_module_mounted;
extern bool sksu_boot_completed;

#endif // __SKS_H_SKSUD_BOOT
