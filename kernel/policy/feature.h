#ifndef __SKS_H_FEATURE
#define __SKS_H_FEATURE

#include <linux/types.h>
#include "uapi/feature.h" // IWYU pragma: keep

typedef int (*sksu_feature_get_t)(u64 *value);
typedef int (*sksu_feature_set_t)(u64 value);

struct sksu_feature_handler {
    u32 feature_id;
    const char *name;
    sksu_feature_get_t get_handler;
    sksu_feature_set_t set_handler;
};

int sksu_register_feature_handler(const struct sksu_feature_handler *handler);

int sksu_unregister_feature_handler(u32 feature_id);

int sksu_get_feature(u32 feature_id, u64 *value, bool *supported);

int sksu_set_feature(u32 feature_id, u64 value);

void sksu_feature_init(void);

void sksu_feature_exit(void);

#endif // __SKS_H_FEATURE
