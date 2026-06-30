// SPDX-License-Identifier: GPL-2.0-only
/*
 * Layer 2: Hook Point Protection (simplified)
 *
 * - Timer-based hook integrity check + auto-recovery
 */
#include "ksu.h"
#include "klog.h"
#include <linux/timer.h>
#include <linux/version.h>
#include <linux/slab.h>
#include <linux/kprobes.h>

struct ksu_hook_slot {
    void **target;
    void *expected;
    void *restore;
    size_t size;
};

#define MAX_SLOTS 32
static struct ksu_hook_slot *hook_slots[MAX_SLOTS];
static int slot_count = 0;
static struct timer_list hook_timer;
static DEFINE_MUTEX(hook_lock);

int ksu_register_hook_slot(void **target, void *expected, void *restore, size_t size)
{
    if (slot_count >= MAX_SLOTS) return -ENOSPC;
    hook_slots[slot_count] = kmalloc(sizeof(struct ksu_hook_slot), GFP_KERNEL);
    if (!hook_slots[slot_count]) return -ENOMEM;
    hook_slots[slot_count]->target = target;
    hook_slots[slot_count]->expected = expected;
    hook_slots[slot_count]->restore = restore;
    hook_slots[slot_count]->size = size;
    slot_count++;
    return 0;
}

static void ksu_hook_check(struct timer_list *t)
{
    int i;
    mutex_lock(&hook_lock);
    for (i = 0; i < slot_count; i++) {
        struct ksu_hook_slot *s = hook_slots[i];
        if (!s || !s->target || !s->expected) continue;
        void *current = (void *)*s->target;
        if (current != s->expected && current != s->restore) {
            pr_warn("hook_protect: slot %d overwritten (%pS != %pS), restoring\n",
                     i, current, s->expected);
            ksu_patch_text(s->target, s->restore?s->restore:s->expected,
                          s->restore?sizeof(s->restore):sizeof(s->expected),
                          KSU_PATCH_TEXT_FLUSH_DCACHE);
        }
    }
    mutex_unlock(&hook_lock);
    mod_timer(&hook_timer, jiffies + msecs_to_jiffies(30000));
}

/* Hide kprobes from debugfs */
void ksu_hide_kprobes(void)
{
    struct dentry *dbg = debugfs_lookup("kprobes", NULL);
    if (dbg && !IS_ERR(dbg))
        debugfs_remove(dbg);
}

void ksu_hook_protect_start(void)
{
    timer_setup(&hook_timer, ksu_hook_check, 0);
    mod_timer(&hook_timer, jiffies + msecs_to_jiffies(30000));
    pr_info("hook_protect: started (30s interval)\n");
}

void ksu_hook_protect_stop(void)
{
    del_timer_sync(&hook_timer);
    mutex_lock(&hook_lock);
    for (int i = 0; i < slot_count; i++) {
        kfree(hook_slots[i]);
        hook_slots[i] = NULL;
    }
    slot_count = 0;
    mutex_unlock(&hook_lock);
    pr_info("hook_protect: stopped\n");
}
