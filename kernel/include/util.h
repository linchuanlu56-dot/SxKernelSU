#ifndef __SKS_H_UTIL
#define __SKS_H_UTIL

#include "linux/fdtable.h" // IWYU pragma: keep
#include <linux/version.h>
#include <linux/syscalls.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 11, 0)
#define sksu_close_fd close_fd
#else
#define sksu_close_fd ksys_close
#endif

#endif
