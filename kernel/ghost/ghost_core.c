// SPDX-License-Identifier: GPL-2.0-only
/*
 * SxKernelSU Ghost Architecture — Core
 *
 * The Ghost architecture redefines root hiding: instead of patching
 * and hiding, KSU never existed for protected processes.
 *
 * 6 orthogonal technologies work together to achieve this.
 */
#include "ghost.h"
#include "../klog.h"
#include <linux/atomic.h>

atomic_t ghost_active = ATOMIC_INIT(1);
atomic_t ghost_emergency_stop = ATOMIC_INIT(0);
static bool ghost_initialized = false;

int ghost_init(void)
{
    if (ghost_initialized) return 0;

    ghost_tvfs_init();
    ghost_sst_init();
    ghost_tde_init();
    ghost_kmf_init();
    ghost_ctc_init();
    ghost_bis_init();

    ghost_initialized = true;
    pr_info("ghost: Ghost architecture initialized (6 technologies)\n");
    return 0;
}

void ghost_exit(void)
{
    if (!ghost_initialized) return;

    atomic_set(&ghost_emergency_stop, 1);
    ghost_bis_exit();
    ghost_ctc_exit();
    ghost_kmf_exit();
    ghost_tde_exit();
    ghost_sst_exit();
    ghost_tvfs_exit();

    ghost_initialized = false;
    pr_info("ghost: Ghost architecture shutdown\n");
}
