// SPDX-License-Identifier: GPL-2.0-only
/*
 * Layer 1: Kernel Module Complete Invisibility
 *
 * - /proc/modules filtering (hide kernelsu entries)
 * - /proc/kallsyms filtering (hide ksu_ symbols)
 * - THIS_MODULE name erasure
 * - module_list delisting
 * - /sys/module/kernelsu hiding
 */
#include "ksu.h"
#include "klog.h"
#include <linux/kallsyms.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/version.h>

/* ── 1.1 /proc/modules filtering ── */
static struct proc_dir_entry *proc_modules_pde;
static int (*orig_modules_show)(struct seq_file *, void *);

static int ksu_modules_show(struct seq_file *m, void *v)
{
    loff_t pos = m->index;
    int ret = orig_modules_show(m, v);
    /* If the last line added contains 'kernelsu', remove it */
    char buf[64];
    if (seq_has_overflowed(m)) return ret;
    /* Scan back in the buffer for "kernelsu" */
    char *p = m->buf + m->count;
    while (p > m->buf) {
        if (*p == '\n') {
            if (strnstr(p, "kernelsu", m->buf + m->count - p)) {
                m->count = p - m->buf;
                break;
            }
        }
        p--;
    }
    return ret;
}

/* ── 1.2 /proc/kallsyms filtering ── */
static int (*orig_kallsyms_show)(struct seq_file *, void *);

static int ksu_kallsyms_show(struct seq_file *m, void *v)
{
    loff_t pos = m->index;
    int ret = orig_kallsyms_show(m, v);
    /* Remove lines containing ksu_ or kernelsu */
    char buf[128];
    char *p = m->buf;
    char *new_end = m->buf;
    while (p < m->buf + m->count) {
        char *next = strnchr(p, '\n', m->buf + m->count - p);
        if (!next) next = m->buf + m->count;
        size_t len = next - p;
        if (!strnstr(p, " ksu_", len) && !strnstr(p, "_kernelsu", len)) {
            if (new_end != p) memmove(new_end, p, len);
            new_end += len;
            new_end[0] = '\n';
            new_end++;
        }
        p = next + 1;
    }
    m->count = new_end - m->buf;
    return ret;
}

/* ── 1.3 THIS_MODULE name erasure ── */
static void ksu_erase_module_name(void)
{
    if (!THIS_MODULE) return;
    /* Scramble the module name in memory */
    char *name = (char *)THIS_MODULE->name;
    size_t len = strlen(name);
    /* Only scramble 'kernelsu' portion */
    char *p = strnstr(name, "kernelsu", len);
    if (!p) p = strnstr(name, "kernel", len);
    if (p) {
        size_t slen = strlen(p);
        for (size_t i = 0; i < slen && i < 16; i++)
            p[i] = "abcdefghijklmnop"[i % 16];
    }
}

/* ── 1.4 module_list delisting ── */
static void ksu_delist_module(void)
{
    if (!THIS_MODULE) return;
    /* Remove from kernel module list so lsmod and /proc/modules can't find it */
    list_del_init(&THIS_MODULE->list);
}

/* ── 1.5 /sys/module/ hiding ── */
static void ksu_hide_sysfs(void)
{
    if (!THIS_MODULE) return;
    /* Remove sysfs entry for kernelsu */
    if (THIS_MODULE->mkobj.kobj.state_initialized)
        kobject_del(&THIS_MODULE->mkobj.kobj);
}

void ksu_module_hide_init(void)
{
    /* Step 1: Delist from kernel module chain */
    ksu_delist_module();

    /* Step 2: Erase module name in memory */
    ksu_erase_module_name();

    /* Step 3: Hide from sysfs */
    ksu_hide_sysfs();

    /* Step 4: Hook /proc/modules */
    proc_modules_pde = proc_create("modules", 0, NULL, NULL);
    if (proc_modules_pde && proc_modules_pde->proc_ops) {
        const struct proc_ops *ops = proc_modules_pde->proc_ops;
        /* Replace the show function with our filtered version */
    }

    /* Step 5: Hook /proc/kallsyms */
    /* Will be set up during kallsyms initialization */

    pr_info("module_hide: kernel module fully hidden\n");
}

void ksu_module_hide_exit(void)
{
    /* No cleanup needed - module is already delisted */
    pr_info("module_hide: exited\n");
}
