#ifndef __SKS_H_SELINUX
#define __SKS_H_SELINUX

#include <linux/types.h>
#include <linux/version.h>
#include <linux/cred.h>

#define SXKERNEL_SU_DOMAIN "ksu"
#define SXKERNEL_SU_FILE "sksu_file"

#define SXKERNEL_SU_CONTEXT "u:r:" SXKERNEL_SU_DOMAIN ":s0"
#define SKS_FILE_CONTEXT "u:object_r:" SXKERNEL_SU_FILE ":s0"
#define ZYGOTE_CONTEXT "u:r:zygote:s0"
#define INIT_CONTEXT "u:r:init:s0"

void setup_selinux(const char *, struct cred *);

void setenforce(bool);

bool getenforce();

void cache_sid(void);

bool is_task_sksu_domain(const struct cred *cred);

bool is_sksu_domain();

bool is_zygote(const struct cred *cred);

bool is_init(const struct cred *cred);

void apply_sxkernelsu_rules();

int handle_sepolicy(void __user *user_data, u64 data_len);

void setup_sksu_cred();

void escape_to_root_for_adb_root();

extern u32 sksu_file_sid;

#endif
