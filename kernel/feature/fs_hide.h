#ifndef _KSU_FS_HIDE_H
#define _KSU_FS_HIDE_H

#include <linux/types.h>

bool ksu_fs_should_hide_path(const char *path, uid_t uid);
bool ksu_fs_should_hide_dirent(const char *name, uid_t uid);
void ksu_fs_hide_init(void);
void ksu_fs_hide_exit(void);

#endif
