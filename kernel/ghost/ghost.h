#ifndef _SXKERNELSU_GHOST_H
#define _SXKERNELSU_GHOST_H

#include <linux/types.h>
#include <linux/uidgid.h>

/* ── Ghost core state ── */
extern atomic_t ghost_active;
extern atomic_t ghost_emergency_stop;

/* ── Technology 1: Per-Task VFS ── */
bool ghost_tvfs_should_hide_path(const char *path, uid_t uid);
bool ghost_tvfs_should_hide_dirent(const char *name, uid_t uid);
void ghost_tvfs_init(void);
void ghost_tvfs_exit(void);

/* ── Technology 2: Shadow Syscall Table ── */
int ghost_sst_init(void);
void ghost_sst_exit(void);
bool ghost_sst_is_active(void);

/* ── Technology 3: Temporal Discontinuity ── */
void ghost_tde_request(uid_t uid);
bool ghost_tde_is_safe_window(void);
void ghost_tde_init(void);
void ghost_tde_exit(void);

/* ── Technology 4: Kernel Memory Fragmentation ── */
void *ghost_kmf_alloc(size_t size);
void ghost_kmf_free(void *ptr, size_t size);
int ghost_kmf_init(void);
void ghost_kmf_exit(void);

/* ── Technology 5: Chaotic Timing Channel ── */
void ghost_ctc_handle_read_stat(uid_t uid);
void ghost_ctc_init(void);
void ghost_ctc_exit(void);

/* ── Technology 6: Biological Immune System ── */
bool ghost_bis_should_allow(uid_t uid);
void ghost_bis_report_scan(uid_t uid);
void ghost_bis_init(void);
void ghost_bis_exit(void);

/* ── Ghost init ── */
int ghost_init(void);
void ghost_exit(void);

#endif
