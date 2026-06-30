// SPDX-License-Identifier: GPL-2.0-only
/*
 * Layer 1: Kernel Module Complete Invisibility
 *
 * - THIS_MODULE name erasure
 * - module_list delisting (hides from /proc/modules & lsmod)
 * - /sys/module/ hiding (kobject_del)
 */
#include "ksu.h"
#include "klog.h"
#include <linux/module.h>
#include <linux/version.h>

static void ksu_erase_module_name(void)
{
    if (!THIS_MODULE) return;
    char *name = (char *)THIS_MODULE->name;
    size_t len = strlen(name);
    size_t i;
    for (i = 0; i < len && i < 24; i++)
        name[i] = 'x';
}

static void ksu_delist_module(void)
{
    if (!THIS_MODULE) return;
    list_del_init(&THIS_MODULE->list);
}

static void ksu_hide_sysfs(void)
{
    if (!THIS_MODULE) return;
    if (THIS_MODULE->mkobj.kobj.state_initialized)
        kobject_del(&THIS_MODULE->mkobj.kobj);
}

void ksu_module_hide_init(void)
{
    ksu_delist_module();
    ksu_erase_module_name();
    ksu_hide_sysfs();
    pr_info("module_hide: kernel module fully hidden\n");
}

void ksu_module_hide_exit(void)
{
    pr_info("module_hide: exited\n");
}
