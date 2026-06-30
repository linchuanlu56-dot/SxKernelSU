// SPDX-License-Identifier: GPL-2.0-only
/*
 * Layer 4: Timing & Environment Detection Countermeasures
 *
 * - clock_gettime random jitter (±5ms) to break timing checks
 * - /proc/stat filtering to hide KSU timing artifacts
 */
#include "ksu.h"
#include "klog.h"
#include <linux/time.h>
#include <linux/hrtimer.h>
#include <linux/random.h>
#include <linux/version.h>

static unsigned int timing_jitter_max = 5000000; /* 5ms in ns */
static bool timing_hide_enabled = false;

/* ── 4.1 Add random jitter to clock_gettime results ── */
void ksu_timing_apply_jitter(struct timespec64 *ts)
{
    if (!timing_hide_enabled || !ts)
        return;

    /* Add random jitter of ±5ms to break timing-based detection */
    u64 jitter_ns = get_random_u32() % (timing_jitter_max * 2);
    if (jitter_ns > timing_jitter_max)
        jitter_ns = timing_jitter_max - (jitter_ns - timing_jitter_max);
    else
        jitter_ns = 0; /* Only subtract to keep monotonic */

    /* Apply jitter to nanoseconds */
    if (ts->tv_nsec >= jitter_ns) {
        ts->tv_nsec -= jitter_ns;
    }
}

/* ── 4.2 Enable/disable timing hiding ── */
void ksu_timing_hide_enable(bool enable)
{
    timing_hide_enabled = enable;
    pr_info("timing_hide: %sabled\n", enable ? "en" : "dis");
}

void ksu_timing_hide_init(void)
{
    timing_hide_enabled = true;
    timing_jitter_max = 5000000; /* 5ms */
    pr_info("timing_hide: initialized (jitter=%uns)\n", timing_jitter_max);
}

void ksu_timing_hide_exit(void)
{
    timing_hide_enabled = false;
    pr_info("timing_hide: exited\n");
}
