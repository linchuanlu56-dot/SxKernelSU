#ifndef __SKS_SU_MOUNT_NS_H
#define __SKS_SU_MOUNT_NS_H

#include <linux/types.h>

#define SKS_NS_INHERITED 0
#define SKS_NS_GLOBAL 1
#define SKS_NS_INDIVIDUAL 2

struct sksu_mns_tw {
    struct callback_head cb;
    int32_t ns_mode;
};

void setup_mount_ns(int32_t ns_mode);

#endif
