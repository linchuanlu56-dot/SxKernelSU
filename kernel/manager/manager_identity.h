#ifndef __SKS_H_MANAGER_IDENTITY
#define __SKS_H_MANAGER_IDENTITY

#include <linux/cred.h>
#include <linux/types.h>

#define SKS_INVALID_APPID -1
#define SKS_PER_USER_RANGE 100000

#ifdef CONFIG_SKS_DISABLE_MANAGER
static inline bool sksu_is_manager_appid_valid()
{
    return true;
}

static inline bool is_manager()
{
    return current_uid().val == 0;
}

static inline bool is_uid_manager(uid_t uid)
{
    return uid == 0;
}

static inline uid_t sksu_get_manager_appid()
{
    return 0;
}

static inline void sksu_set_manager_appid(uid_t appid)
{
    (void)appid;
}

static inline void sksu_invalidate_manager_uid()
{
}
#else
extern uid_t sksu_manager_appid; // DO NOT DIRECT USE

static inline bool sksu_is_manager_appid_valid()
{
    return sksu_manager_appid != SKS_INVALID_APPID;
}

static inline bool is_manager()
{
    return unlikely(sksu_manager_appid == current_uid().val % SKS_PER_USER_RANGE);
}

static inline bool is_uid_manager(uid_t uid)
{
    return unlikely(sksu_manager_appid == uid % SKS_PER_USER_RANGE);
}

static inline uid_t sksu_get_manager_appid()
{
    return sksu_manager_appid;
}

static inline void sksu_set_manager_appid(uid_t appid)
{
    sksu_manager_appid = appid;
}

static inline void sksu_invalidate_manager_uid()
{
    sksu_manager_appid = SKS_INVALID_APPID;
}
#endif

#endif // __SKS_H_MANAGER_IDENTITY
