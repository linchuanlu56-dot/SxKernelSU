#ifndef __SKS_H_KSU
#define __SKS_H_KSU

#include <linux/types.h>
#include <linux/cred.h>
#include <linux/workqueue.h>

#define SXKERNEL_SU_VERSION SKS_VERSION

extern struct cred *sksu_cred;
extern bool sksu_late_loaded;
extern bool allow_shell;
extern struct selinux_policy *backup_sepolicy;
extern bool sksu_no_custom_rc;

static inline int startswith(char *s, char *prefix)
{
    return strncmp(s, prefix, strlen(prefix));
}

static inline int endswith(const char *s, const char *t)
{
    size_t slen = strlen(s);
    size_t tlen = strlen(t);
    if (tlen > slen)
        return 1;
    return strcmp(s + slen - tlen, t);
}

#endif
