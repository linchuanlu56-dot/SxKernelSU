// SPDX-License-Identifier: GPL-2.0-only
/*
 * SxKernelSU filesystem-level root hiding
 *
 * Hides KSU-related files and directories from target processes
 * by hooking vfs_stat, vfs_open, and directory listing.
 */

#include "ksu.h"
#include "klog.h"
#include "feature/process_hide.h"
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/dcache.h>
#include <linux/slab.h>
#include <linux/version.h>

/* Protected paths - shown as non-existent to hidden apps */
static const char * const protected_paths[] = {
    "/data/adb/ksu",
    "/data/adb/modules",
    "/data/adb/ksu.apk",
    "/debug_ramdisk/ksu",
    NULL,
};

static bool is_path_protected(const char *path)
{
    int i;
    if (!path)
        return false;

    for (i = 0; protected_paths[i]; i++) {
        if (!strcmp(path, protected_paths[i]))
            return true;
        /* Check if path starts with a protected path */
        size_t len = strlen(protected_paths[i]);
        if (!strncmp(path, protected_paths[i], len) &&
            (path[len] == '/' || path[len] == '\0'))
            return true;
    }
    return false;
}

/* Called before vfs_stat/open for target processes */
bool ksu_fs_should_hide_path(const char *path, uid_t uid)
{
    if (!uid || !path)
        return false;

    /* Only hide if this app is in the hide list */
    if (!ksu_process_hide_is_app_protected(uid))
        return false;

    return is_path_protected(path);
}

/* Hook for filldir - hide KSU files from directory listing */
bool ksu_fs_should_hide_dirent(const char *name, uid_t uid)
{
    if (!name || !uid)
        return false;

    if (!ksu_process_hide_is_app_protected(uid))
        return false;

    if (!strcmp(name, "ksu") ||
        !strcmp(name, "ksu.apk") ||
        !strcmp(name, "modules") ||
        !strcmp(name, "ksud"))
        return true;

    return false;
}

void ksu_fs_hide_init(void)
{
    pr_info("fs_hide: initialized\n");
}

void ksu_fs_hide_exit(void)
{
    pr_info("fs_hide: exited\n");
}
