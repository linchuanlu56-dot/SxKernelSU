// SPDX-License-Identifier: GPL-2.0-only
/*
 * Ghost Technology 6: Biological Immune System
 *
 * Inspired by the human immune system:
 * - Positive selection: Only allow root for known whitelisted apps
 * - Negative selection: Detect scanning/probing → emergency shutdown
 * - Memory: Remember threats and respond faster next time
 * - Always active: Monitors for danger signals constantly
 */
#include "ghost.h"
#include "klog.h"
#include <linux/slab.h>
#include <linux/version.h>

#define MAX_WHITELIST 64
#define MAX_THREAT_MEMORY 32
#define SCAN_THRESHOLD 3

/* ── Whitlelist (positive selection) ── */
struct ghost_whitelist_entry {
    uid_t uid;
    bool active;
};

static struct ghost_whitelist_entry whitelist[MAX_WHITELIST];
static int whitelist_count = 0;
static DEFINE_MUTEX(whitelist_lock);

int ghost_bis_add_to_whitelist(uid_t uid)
{
    int ret = 0;
    mutex_lock(&whitelist_lock);
    if (whitelist_count >= MAX_WHITELIST) {
        ret = -ENOSPC;
        goto out;
    }
    whitelist[whitelist_count].uid = uid;
    whitelist[whitelist_count].active = true;
    whitelist_count++;
out:
    mutex_unlock(&whitelist_lock);
    return ret;
}

/* ── Threat memory (negative selection memory) ── */
struct ghost_threat {
    uid_t uid;
    unsigned long first_seen;
    int count;
};

static struct ghost_threat threats[MAX_THREAT_MEMORY];
static int threat_count = 0;
static DEFINE_MUTEX(threat_lock);

void ghost_bis_report_scan(uid_t uid)
{
    int i;
    mutex_lock(&threat_lock);

    /* Find existing threat or create new */
    for (i = 0; i < threat_count; i++) {
        if (threats[i].uid == uid) {
            threats[i].count++;
            threats[i].first_seen = threats[i].first_seen ?: jiffies;

            /* Scan threshold exceeded → emergency shutdown */
            if (threats[i].count >= SCAN_THRESHOLD) {
                pr_warn("ghost_bis: Threat detected from uid %d, emergency shutdown\n", uid);
                atomic_set(&ghost_emergency_stop, 1);
            }
            goto out;
        }
    }

    /* New threat */
    if (threat_count < MAX_THREAT_MEMORY) {
        threats[threat_count].uid = uid;
        threats[threat_count].count = 1;
        threats[threat_count].first_seen = jiffies;
        threat_count++;
    }

out:
    mutex_unlock(&threat_lock);
}

/* ── Positive selection: should this app get root? ── */
bool ghost_bis_should_allow(uid_t uid)
{
    int i;

    if (atomic_read(&ghost_emergency_stop))
        return false;

    /* Check for debugger/tracer */
    if (current->ptrace) {
        ghost_bis_report_scan(uid);
        return false;
    }

    /* Check whitelist */
    mutex_lock(&whitelist_lock);
    for (i = 0; i < whitelist_count; i++) {
        if (whitelist[i].uid == uid && whitelist[i].active) {
            mutex_unlock(&whitelist_lock);
            return true;
        }
    }
    mutex_unlock(&whitelist_lock);
    return false;
}

/* ── Danger signal detection ── */
void ghost_bis_check_danger_signals(void)
{
    /* Detect strace/ptrace on any protected app */
    if (unlikely(current->ptrace)) {
        uid_t uid = current_uid().val;
        ghost_bis_report_scan(uid);
    }

    /* Detect /proc scanning */
    /* Detect debugger attachments */
}

void ghost_bis_init(void)
{
    whitelist_count = 0;
    threat_count = 0;
    atomic_set(&ghost_emergency_stop, 0);
    pr_info("ghost_bis: Biological Immune System initialized\n");
}

void ghost_bis_exit(void)
{
    whitelist_count = 0;
    threat_count = 0;
    pr_info("ghost_bis: disabled\n");
}
