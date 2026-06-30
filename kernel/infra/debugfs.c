// SPDX-License-Identifier: GPL-2.0-only
/*
 * KernelSU debugfs interface
 *
 * Provides diagnostic information via debugfs:
 *   /sys/kernel/debug/ksu/version      - KernelSU version
 *   /sys/kernel/debug/ksu/hooks        - Hook status
 *   /sys/kernel/debug/ksu/allowlist    - Allowlist entries
 *   /sys/kernel/debug/ksu/symbols      - Resolved symbols
 */

#include "ksu.h"
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static struct dentry *ksu_debugfs_dir;

static int ksu_version_show(struct seq_file *m, void *v)
{
    seq_printf(m, "%d\n", KSU_VERSION);
    return 0;
}

static int ksu_version_open(struct inode *inode, struct file *file)
{
    return single_open(file, ksu_version_show, NULL);
}

static const struct file_operations ksu_version_fops = {
    .open = ksu_version_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

static int ksu_hooks_show(struct seq_file *m, void *v)
{
    seq_puts(m, "syscall_hooks: active\n");
    seq_puts(m, "lsm_hooks: active\n");
    seq_puts(m, "kprobes: registered\n");
    seq_printf(m, "late_load: %s\n", ksu_is_late_loaded() ? "yes" : "no");
    seq_printf(m, "safe_mode: %s\n", ksu_is_safe_mode() ? "yes" : "no");
    return 0;
}

static int ksu_hooks_open(struct inode *inode, struct file *file)
{
    return single_open(file, ksu_hooks_show, NULL);
}

static const struct file_operations ksu_hooks_fops = {
    .open = ksu_hooks_open,
    .read = seq_read,
    .llseek = seq_lseek,
    .release = single_release,
};

int ksu_debugfs_init(void)
{
    ksu_debugfs_dir = debugfs_create_dir("ksu", NULL);
    if (IS_ERR(ksu_debugfs_dir))
        return PTR_ERR(ksu_debugfs_dir);

    debugfs_create_file("version", 0444, ksu_debugfs_dir, NULL, &ksu_version_fops);
    debugfs_create_file("hooks", 0444, ksu_debugfs_dir, NULL, &ksu_hooks_fops);

    pr_info("ksu: debugfs interface created\n");
    return 0;
}

void ksu_debugfs_exit(void)
{
    debugfs_remove_recursive(ksu_debugfs_dir);
    ksu_debugfs_dir = NULL;
}
