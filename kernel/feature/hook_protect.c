// SPDX-License-Identifier: GPL-2.0-only
/*
 * Layer 2: Hook Point Protection
 *
 * - Timer-based syscall hook integrity check + auto-recovery
 * - Registers hook slots via KSU's existing hook infrastructure
 * - Uses find_kernel_symbol_exact() instead of direct extern access
 */
#include "ksu.h"
#include "klog.h"
#include "infra/symbol_resolver.h"
#include <linux/timer.h>
#include <linux/version.h>
#include <linux/slab.h>

struct hook_slot {
    void **target_addr;
    void *expected_fn;
};

#define MAX_SLOTS 16
static struct hook_slot *slots[MAX_SLOTS];
static int slot_count = 0;
static struct timer_list check_timer;
static DEFINE_MUTEX(slots_lock);

int ksu_register_hook_slot(void **target, void *expected)
{
    int ret = 0;
    mutex_lock(&slots_lock);
    if (slot_count >= MAX_SLOTS) {
        ret = -ENOSPC;
        goto out;
    }
    slots[slot_count] = kmalloc(sizeof(struct hook_slot), GFP_KERNEL);
    if (!slots[slot_count]) {
        ret = -ENOMEM;
        goto out;
    }
    slots[slot_count]->target_addr = target;
    slots[slot_count]->expected_fn = expected;
    slot_count++;
out:
    mutex_unlock(&slots_lock);
    return ret;
}

static void integrity_check(struct timer_list *t)
{
    int i;
    mutex_lock(&slots_lock);
    for (i = 0; i < slot_count; i++) {
        struct hook_slot *s = slots[i];
        if (!s || !s->target_addr || !s->expected_fn)
            continue;
        void *current = (void *)*s->target_addr;
        if (current != s->expected_fn) {
            pr_warn("hook_protect: slot %d overwritten! expected %pS, found %pS, restoring\n",
                     i, s->expected_fn, current);
            ksu_patch_text(s->target_addr, &s->expected_fn,
                          sizeof(s->expected_fn), KSU_PATCH_TEXT_FLUSH_DCACHE);
        }
    }
    mutex_unlock(&slots_lock);
    mod_timer(&check_timer, jiffies + msecs_to_jiffies(30000));
}

/* Hide kprobes from debugfs — remove the debugfs kprobes directory */
void ksu_hide_kprobes(void)
{
    struct dentry *d;
    /* Try to remove the kprobes debugfs entry to hide our probes */
    d = debugfs_lookup("kprobes", NULL);
    if (d && !IS_ERR(d)) {
        debugfs_remove(d);
        pr_info("hook_protect: kprobes debugfs hidden\n");
    }
}

void ksu_hook_protect_start(void)
{
    timer_setup(&check_timer, integrity_check, 0);
    mod_timer(&check_timer, jiffies + msecs_to_jiffies(30000));
    pr_info("hook_protect: started (30s interval)\n");
}

void ksu_hook_protect_stop(void)
{
    int i;
    del_timer_sync(&check_timer);
    mutex_lock(&slots_lock);
    for (i = 0; i < slot_count; i++) {
        kfree(slots[i]);
        slots[i] = NULL;
    }
    slot_count = 0;
    mutex_unlock(&slots_lock);
    pr_info("hook_protect: stopped\n");
}
