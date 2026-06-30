// SPDX-License-Identifier: GPL-2.0-only
/*
 * Layer 2: Hook Point Protection
 *
 * - syscall_table integrity check + auto-recovery timer
 * - kprobe hide from debugfs
 * - Double-path hook takeover
 */
#include "ksu.h"
#include "klog.h"
#include <linux/timer.h>
#include <linux/version.h>
#include <linux/kallsyms.h>
#include <asm/syscall.h>

extern const void *sys_call_table[];

/* ── 2.1 Syscall table integrity check ── */
struct ksu_hook_entry {
    int syscall_nr;
    void *expected_fn;
    void **target_slot;
    const char *name;
};

#define MAX_HOOK_ENTRIES 16
static struct ksu_hook_entry hook_entries[MAX_HOOK_ENTRIES];
static int hook_entry_count = 0;
static struct timer_list hook_check_timer;

static DEFINE_MUTEX(hook_check_lock);

int ksu_register_hook_protect(int syscall_nr, void *our_fn, void **orig_slot)
{
    if (hook_entry_count >= MAX_HOOK_ENTRIES)
        return -ENOSPC;

    hook_entries[hook_entry_count].syscall_nr = syscall_nr;
    hook_entries[hook_entry_count].expected_fn = our_fn;
    hook_entries[hook_entry_count].target_slot = orig_slot;
    hook_entries[hook_entry_count].name = NULL;
    hook_entry_count++;
    return 0;
}

static void ksu_hook_integrity_check(struct timer_list *t)
{
    int i;
    mutex_lock(&hook_check_lock);

    for (i = 0; i < hook_entry_count; i++) {
        struct ksu_hook_entry *e = &hook_entries[i];
        if (!e->target_slot || !e->expected_fn)
            continue;

        void *current = (void *)smp_load_acquire(e->target_slot);

        if (current != e->expected_fn && current != NULL) {
            /* Hook was overwritten! Restore it. */
            pr_warn("hook_protect: syscall #%d overwritten (was %pS, now %pS), restoring\n",
                     e->syscall_nr, e->expected_fn, current);
            ksu_patch_text(e->target_slot, &e->expected_fn, sizeof(e->expected_fn),
                          KSU_PATCH_TEXT_FLUSH_DCACHE);
        }
    }
    mutex_unlock(&hook_check_lock);

    /* Re-arm timer */
    mod_timer(&hook_check_timer, jiffies + msecs_to_jiffies(30000));
}

/* ── 2.2 Kprobe hiding ── */
#include <linux/kprobes.h>

static void ksu_hide_kprobes(void)
{
    /* Remove kprobe entries from debugfs if they exist */
    /* This prevents /sys/kernel/debug/kprobes/list from showing our probes */
    struct dentry *kprobe_dentry = debugfs_lookup("kprobes", NULL);
    if (kprobe_dentry) {
        /* Iterate kprobe debugfs entries and remove ours */
        /* Simplified: just remove the entire kprobe debugfs dir */
        /* This is heavy-handed but effective */
        debugfs_remove(kprobe_dentry);
    }
}

/* ── 2.3 Timer-based protection ── */
void ksu_hook_protect_start(void)
{
    /* Start integrity check timer (every 30 seconds) */
    timer_setup(&hook_check_timer, ksu_hook_integrity_check, 0);
    mod_timer(&hook_check_timer, jiffies + msecs_to_jiffies(30000));
    pr_info("hook_protect: integrity check timer started (30s interval)\n");
}

void ksu_hook_protect_stop(void)
{
    /* Stop timer */
    del_timer_sync(&hook_check_timer);
    mutex_lock(&hook_check_lock);
    hook_entry_count = 0;
    mutex_unlock(&hook_check_lock);
    pr_info("hook_protect: stopped\n");
}
