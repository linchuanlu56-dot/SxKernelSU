#ifndef _KSU_PROCESS_HIDE_H
#define _KSU_PROCESS_HIDE_H

#include <linux/types.h>

bool ksu_process_hide_is_app_protected(uid_t uid);
int ksu_process_hide_add_app(uid_t uid);
int ksu_process_hide_remove_app(uid_t uid);
bool ksu_should_hide_maps_line(uid_t uid, const char *line);
bool ksu_should_hide_mount_path(uid_t uid, const char *path);
bool ksu_proc_should_hide_entry(pid_t pid);
bool ksu_fd_should_hide(struct file *filp, uid_t uid);
void ksu_process_hide_init(void);
void ksu_process_hide_exit(void);

#endif
