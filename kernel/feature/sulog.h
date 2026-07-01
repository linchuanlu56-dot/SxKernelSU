#ifndef __SKS_H_SULOG
#define __SKS_H_SULOG

#include <linux/types.h>

bool sksu_sulog_is_enabled(void);
void sksu_sulog_init(void);
void sksu_sulog_exit(void);

#endif
