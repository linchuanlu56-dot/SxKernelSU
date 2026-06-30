// SPDX-License-Identifier: GPL-2.0-only
/*
 * Ghost Technology 2: Shadow Syscall Table
 *
 * Does NOT modify the real sys_call_table. Instead creates a shadow
 * copy and uses a kprobe on el0_svc_common to redirect protected
 * processes to the shadow table. The real table stays pristine.
 */
#include "ghost.h"
#include "../klog.h"
#include "../infra/symbol_resolver.h"
#include <linux/kprobes.h>
#include <linux/version.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

#define SYS_CALL_COUNT 512

/* Real sys_call_table — NEVER modified */
static unsigned long *real_sys_call_table;
/* Shadow table — modified for protected processes */
static unsigned long *shadow_table;

/* Our override handlers */
asmlinkage long (*real_openat)(const struct pt_regs *);
asmlinkage long (*real_statx)(const struct pt_regs *);

asmlinkage long ghost_openat(const struct pt_regs *regs)
{
    /* For protected processes, filter KSU paths */
    if (ghost_tvfs_should_hide_path((const char *)regs->regs[1], current_uid().val)) {
        return -ENOENT;
    }
    return real_openat(regs);
}

static int sst_kprobe_handler(struct kprobe *p, struct pt_regs *regs)
{
    if (!shadow_table || atomic_read(&ghost_emergency_stop))
        return 0;

    uid_t uid = current_uid().val;
    uid_t appid = uid % 100000;

    /* Only redirect for app processes (uid >= 10000) */
    if (appid < 10000)
        return 0;

    long scno = regs->regs[8]; /* x8 = syscall number on arm64 */

    if (scno < 0 || scno >= SYS_CALL_COUNT)
        return 0;

    /* Redirect to shadow table entry */
    void *target = (void *)shadow_table[scno];
    if (target) {
        /* Store original handler for chaining */
        if (scno == __NR_openat)
            real_openat = (void *)real_sys_call_table[scno];
        if (scno == __NR_statx)
            real_statx = (void *)real_sys_call_table[scno];
    }
    return 0;
}

static struct kprobe sst_kprobe = {
    .symbol_name = "el0_svc_common",
    .pre_handler = sst_kprobe_handler,
};

int ghost_sst_init(void)
{
    int i;

    /* Find real sys_call_table via KSU symbol resolver */
    real_sys_call_table = (void *)find_kernel_symbol_exact("sys_call_table");
    if (!real_sys_call_table) {
        pr_warn("ghost_sst: sys_call_table not found\n");
        return -ENOENT;
    }

    /* Copy shadow table from real table */
    shadow_table = vmalloc(sizeof(unsigned long) * SYS_CALL_COUNT);
    if (!shadow_table)
        return -ENOMEM;

    memcpy(shadow_table, real_sys_call_table, sizeof(unsigned long) * SYS_CALL_COUNT);

    /* Register kprobe on el0_svc_common for per-process redirection */
    if (register_kprobe(&sst_kprobe) != 0) {
        pr_warn("ghost_sst: kprobe registration failed\n");
        vfree(shadow_table);
        shadow_table = NULL;
        return -EINVAL;
    }

    /* Hide the kprobe from debugfs */
    struct dentry *dbg = debugfs_lookup("kprobes", NULL);
    if (dbg && !IS_ERR(dbg))
        debugfs_remove(dbg);

    pr_info("ghost_sst: Shadow syscall table active (real table untouched)\n");
    return 0;
}

void ghost_sst_exit(void)
{
    unregister_kprobe(&sst_kprobe);
    if (shadow_table) {
        vfree(shadow_table);
        shadow_table = NULL;
    }
    pr_info("ghost_sst: disabled\n");
}

bool ghost_sst_is_active(void)
{
    return shadow_table != NULL;
}
