// SPDX-License-Identifier: GPL-2.0-only
/*
 * Ghost Technology 1: Per-Task Virtual File System
 *
 * For protected processes, KSU files/dirs never existed.
 * Intercepts vfs_readdir and vfs_open at the lowest VFS layer.
 * Not mount namespace - truly per-task at VFS level.
 */
#include "ghost.h"
#include "klog.h"
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/slab.h>

/* Paths invisible to protected processes */
static const char *ghost_hidden_paths[] = {
    "/data/adb/ksu",
    "/data/adb/modules",
    "/data/adb/ksu.apk",
    NULL,
};

/* Names invisible in directory listings */
static const char *ghost_hidden_names[] = {
    "ksu",
    "ksu.apk",
    "modules",
    "ksud",
    "kernelsu",
    NULL,
};

static bool is_protected_app(uid_t uid)
{
    if (atomic_read(&ghost_emergency_stop))
        return false;
    uid_t appid = uid % 100000;
    return appid >= 10000;
}

bool ghost_tvfs_should_hide_path(const char *path, uid_t uid)
{
    int i;
    if (!path || !is_protected_app(uid))
        return false;

    for (i = 0; ghost_hidden_paths[i]; i++) {
        if (!strcmp(path, ghost_hidden_paths[i]))
            return true;
        size_t plen = strlen(ghost_hidden_paths[i]);
        if (!strncmp(path, ghost_hidden_paths[i], plen) &&
            (path[plen] == '/' || path[plen] == '\0'))
            return true;
    }
    return false;
}

bool ghost_tvfs_should_hide_dirent(const char *name, uid_t uid)
{
    int i;
    if (!name || !is_protected_app(uid))
        return false;

    for (i = 0; ghost_hidden_names[i]; i++) {
        if (!strcmp(name, ghost_hidden_names[i]))
            return true;
    }
    return false;
}

void ghost_tvfs_init(void)
{
    pr_info("ghost_tvfs: Per-Task VFS initialized\n");
}

void ghost_tvfs_exit(void)
{
    pr_info("ghost_tvfs: disabled\n");
}
