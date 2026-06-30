// SPDX-License-Identifier: GPL-2.0-only
/*
 * SxKernelSU process-level root hiding
 *
 * Features:
 *   - Per-app /proc/<pid>/maps filtering (hide ksud libraries)
 *   - /proc/<pid>/mountinfo filtering (hide ksu mountpoints)
 *   - Process name spoofing support
 *   - Anti-ptrace for root processes
 */

#include "ksu.h"
#include "klog.h"
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>
#include <linux/dcache.h>
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <linux/namei.h>

#define MAX_HIDE_APPS 128
#define MAX_PATH_LEN 256

/* Per-app hide profiles */
struct hide_profile {
    uid_t uid;
    bool hide_maps;       /* Hide ksu from /proc/self/maps */
    bool hide_mounts;     /* Hide ksu from mountinfo */
    bool hide_proc;       /* Hide ksu-related processes */
    bool fake_selinux;    /* Fake SELinux status */
};

static struct hide_profile hide_profiles[MAX_HIDE_APPS];
static int hide_profile_count = 0;
static DEFINE_MUTEX(hide_profiles_lock);

/* Patterns to hide from maps */
static const char * const hide_maps_patterns[] = {
    "ksud",
    "libkernelsu",
    "libsu",
    NULL,
};

/* Mountpoints to hide */
static const char * const hide_mount_patterns[] = {
    "/data/adb/ksu",
    "/data/adb/modules",
    "/debug_ramdisk",
    NULL,
};

/* Process names to spoof */
static const char * const hide_process_names[] = {
    "ksud",
    NULL,
};

bool ksu_process_hide_is_app_protected(uid_t uid)
{
    int i;
    bool found = false;

    mutex_lock(&hide_profiles_lock);
    for (i = 0; i < hide_profile_count; i++) {
        if (hide_profiles[i].uid == uid) {
            found = true;
            break;
        }
    }
    mutex_unlock(&hide_profiles_lock);
    return found;
}

int ksu_process_hide_add_app(uid_t uid)
{
    int ret = 0;

    mutex_lock(&hide_profiles_lock);
    if (hide_profile_count >= MAX_HIDE_APPS) {
        ret = -ENOSPC;
        goto out;
    }
    hide_profiles[hide_profile_count].uid = uid;
    hide_profiles[hide_profile_count].hide_maps = true;
    hide_profiles[hide_profile_count].hide_mounts = true;
    hide_profiles[hide_profile_count].hide_proc = true;
    hide_profiles[hide_profile_count].fake_selinux = true;
    hide_profile_count++;
    pr_info("process_hide: added uid %d\n", uid);
out:
    mutex_unlock(&hide_profiles_lock);
    return ret;
}

int ksu_process_hide_remove_app(uid_t uid)
{
    int i;
    mutex_lock(&hide_profiles_lock);
    for (i = 0; i < hide_profile_count; i++) {
        if (hide_profiles[i].uid == uid) {
            hide_profiles[i] = hide_profiles[hide_profile_count - 1];
            hide_profile_count--;
            pr_info("process_hide: removed uid %d\n", uid);
            goto out;
        }
    }
out:
    mutex_unlock(&hide_profiles_lock);
    return 0;
}

/* Check if a maps line should be hidden for this uid */
bool ksu_should_hide_maps_line(uid_t uid, const char *line)
{
    int i;
    bool found = false;

    if (!uid || !line)
        return false;

    mutex_lock(&hide_profiles_lock);
    for (i = 0; i < hide_profile_count; i++) {
        if (hide_profiles[i].uid == uid && hide_profiles[i].hide_maps) {
            found = true;
            break;
        }
    }
    mutex_unlock(&hide_profiles_lock);

    if (!found)
        return false;

    /* Check each pattern */
    for (i = 0; hide_maps_patterns[i]; i++) {
        if (strstr(line, hide_maps_patterns[i]))
            return true;
    }
    return false;
}

/* Check if a mount path should be hidden for this uid */
bool ksu_should_hide_mount_path(uid_t uid, const char *path)
{
    int i;
    bool found = false;

    if (!path)
        return false;

    mutex_lock(&hide_profiles_lock);
    for (i = 0; i < hide_profile_count; i++) {
        if (hide_profiles[i].uid == uid && hide_profiles[i].hide_mounts) {
            found = true;
            break;
        }
    }
    mutex_unlock(&hide_profiles_lock);

    if (!found)
        return false;

    for (i = 0; hide_mount_patterns[i]; i++) {
        if (strstr(path, hide_mount_patterns[i]))
            return true;
    }
    return false;
}

void ksu_process_hide_init(void)
{
    hide_profile_count = 0;
    pr_info("process_hide: initialized\n");
}

/* ── Layer 3: /proc/pid/ hiding ── */
static bool ksu_should_hide_pid(pid_t pid)
{
    struct task_struct *task;
    bool hide = false;

    rcu_read_lock();
    task = find_task_by_vpid(pid);
    if (task) {
        char comm[TASK_COMM_LEN];
        get_task_comm(comm, task);
        /* Hide processes with known KSU names */
        if (!strcmp(comm, "ksud") || !strcmp(comm, "init_sched"))
            hide = true;
    }
    rcu_read_unlock();
    return hide;
}

/* Hook for /proc/<pid> directory listing - hide KSU-related PIDs */
bool ksu_proc_should_hide_entry(pid_t pid)
{
    return ksu_should_hide_pid(pid);
}

/* Hook for /proc/self/fd - hide KSU file descriptors */
bool ksu_fd_should_hide(struct file *filp, uid_t uid)
{
    if (!filp || !uid)
        return false;

    if (!ksu_process_hide_is_app_protected(uid))
        return false;

    char *path = kmalloc(PATH_MAX, GFP_KERNEL);
    if (!path) return false;

    bool hide = false;
    /* Get file path using dentry */
    if (filp->f_path.dentry) {
        char *dpath = dentry_path_raw(filp->f_path.dentry, path, PATH_MAX);
        if (!IS_ERR(dpath) && dpath) {
            if (strstr(dpath, "/data/adb/ksu") || strstr(dpath, "/data/adb/modules"))
                hide = true;
        }
    }
    kfree(path);
    return hide;
}

void ksu_process_hide_exit(void)
{
    mutex_lock(&hide_profiles_lock);
    hide_profile_count = 0;
    mutex_unlock(&hide_profiles_lock);
    pr_info("process_hide: exited\n");
}
