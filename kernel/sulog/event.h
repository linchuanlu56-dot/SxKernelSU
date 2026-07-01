#ifndef __SKS_H_SULOG_EVENT
#define __SKS_H_SULOG_EVENT

#include <linux/compiler_types.h>
#include <linux/gfp.h>
#include <linux/types.h>
#include "uapi/sulog.h" // IWYU pragma: keep

struct sksu_event_queue;
struct sksu_sulog_pending_event;

int sksu_sulog_events_init(void);
void sksu_sulog_events_exit(void);

struct sksu_sulog_pending_event *sksu_sulog_capture_root_execve(const char __user *filename_user,
                                                              const char __user *const __user *argv_user, gfp_t gfp);
struct sksu_sulog_pending_event *sksu_sulog_capture_sucompat(const char __user *filename_user,
                                                           const char __user *const __user *argv_user, gfp_t gfp);
void sksu_sulog_emit_pending(struct sksu_sulog_pending_event *pending, int retval, gfp_t gfp);
int sksu_sulog_emit_grant_root(int retval, __u32 uid, __u32 euid, gfp_t gfp);

struct sksu_event_queue *sksu_sulog_get_queue(void);

#endif
