#ifndef __SKS_H_SELINUX_HIDE
#define __SKS_H_SELINUX_HIDE

void sksu_selinux_hide_init();
void sksu_selinux_hide_exit();
void sksu_selinux_hide_drop_backup_if_unused();
void sksu_selinux_hide_handle_second_stage();
void sksu_selinux_hide_handle_post_fs_data();

#endif
