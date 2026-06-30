// SPDX-License-Identifier: GPL-2.0-only
/*
 * Layer 1: Kernel Module Complete Invisibility
 *
 * - THIS_MODULE name erasure (prevent lsmod from showing kernelsu)
 * - module_list delisting (remove from kernel module chain)
 * - /sys/module/kernelsu hiding (remove sysfs entry)
 */
#include "ksu.h"
#include "klog.h"
#include <linux/module.h>
#include <linux/version.h>

/* ── 1.1 THIS_MODULE name erasure ── */
static void ksu_erase_module_name(void)
{
    if (!THIS_MODULE) return;
    char *name = (char *)THIS_MODULE->name;
    size_t len = strlen(name);
    /* Scramble the 'kernelsu' portion in memory */
    for (size_t i = 0; i < len && i < 20; i++)
        name[i] = 'x';
}

/* ── 1.2 module_list delisting ── */
static void ksu_delist_module(void)
{
    if (!THIS_MODULE) return;
    /* Remove from kernel module list so lsmod /proc/modules can't traverse */
    list_del_init(&THIS_MODULE->list);
}

/* ── 1.3 /sys/module/ hiding ── */
static void ksu_hide_sysfs(void)
{
    if (!THIS_MODULE) return;
    /* Remove sysfs entry for kernelsu */
    if (THIS_MODULE->mkobj.kobj.state_initialized)
        kobject_del(&THIS_MODULE->mkobj.kobj);
}

void ksu_module_hide_init(void)
{
    /* Step 1: Delist from kernel module chain (prevents lsmod /proc/modules) */
    ksu_delist_module();

    /* Step 2: Erase module name in memory */
    ksu_erase_module_name();

    /* Step 3: Hide from sysfs */
    ksu_hide_sysfs();

    pr_info("module_hide: kernel module fully hidden\n");
}

void ksu_module_hide_exit(void)
{
    pr_info("module_hide: exited\n");
}
