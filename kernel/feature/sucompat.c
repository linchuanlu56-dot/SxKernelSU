#include "linux/file.h"
#include "linux/namei.h"
#include <linux/compiler_types.h>
#include <linux/preempt.h>
#include <linux/printk.h>
#include <linux/mm.h>
#include <linux/pgtable.h>
#include <linux/uaccess.h>
#include <asm/current.h>
#include <linux/cred.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/sched/task_stack.h>
#include <linux/ptrace.h>

#include "arch.h"
#include "policy/allowlist.h"
#include "policy/feature.h"
#include "klog.h" // IWYU pragma: keep
#include "runtime/sksud.h"
#include "feature/sucompat.h"
#include "policy/app_profile.h"
#include "hook/syscall_hook.h"
#include "sulog/event.h"
#include "sksu.h"
#include "util.h"

#define SU_PATH "/system/bin/su"
#define SH_PATH "/system/bin/sh"

bool sksu_su_compat_enabled __read_mostly = true;

static int su_compat_feature_get(u64 *value)
{
    *value = sksu_su_compat_enabled ? 1 : 0;
    return 0;
}

static int su_compat_feature_set(u64 value)
{
    bool enable = value != 0;
    sksu_su_compat_enabled = enable;
    pr_info("su_compat: set to %d\n", enable);
    return 0;
}

static const struct sksu_feature_handler su_compat_handler = {
    .feature_id = SKS_FEATURE_SU_COMPAT,
    .name = "su_compat",
    .get_handler = su_compat_feature_get,
    .set_handler = su_compat_feature_set,
};

static void __user *userspace_stack_buffer(const void *d, size_t len)
{
    // To avoid having to mmap a page in userspace, just write below the stack
    // pointer.
    char __user *p = (void __user *)current_user_stack_pointer() - len;

    return copy_to_user(p, d, len) ? NULL : p;
}

static char __user *sksud_user_path(void)
{
    static const char sksud_path[] = SKSUD_PATH;

    return userspace_stack_buffer(sksud_path, sizeof(sksud_path));
}

static char __user *empty_user_path(void)
{
    return userspace_stack_buffer("", sizeof(""));
}

static const char su_path[] = SU_PATH;

static bool is_sksud_exists()
{
    struct path path;

    if (kern_path(SKSUD_PATH, 0, &path) < 0) {
        return false;
    }
    path_put(&path);
    return true;
}

long sksu_handle_faccessat_sucompat(int orig_nr, struct pt_regs *regs)
{
    const char __user **filename_user, *orig_filename;
    long ret;
    const struct cred *old_cred;

    if (!sksu_is_allow_uid_for_current(current_uid().val)) {
        goto do_orig_facessat;
    }

    filename_user = (const char __user **)&PT_REGS_PARM2(regs);

    char path[sizeof(su_path) + 1];
    memset(path, 0, sizeof(path));
    strncpy_from_user_nofault(path, *filename_user, sizeof(path));

    if (unlikely(!memcmp(path, su_path, sizeof(su_path)))) {
        old_cred = override_creds(sksu_cred);
        if (is_sksud_exists()) {
            pr_info("faccessat su->sksud!\n");
            orig_filename = *filename_user;
            *filename_user = sksud_user_path();
            ret = sksu_syscall_table[orig_nr](regs);
            revert_creds(old_cred);
            *filename_user = orig_filename;
            return ret;
        } else {
            revert_creds(old_cred);
        }
    }

do_orig_facessat:
    return sksu_syscall_table[orig_nr](regs);
}

long sksu_handle_stat_sucompat(int orig_nr, struct pt_regs *regs)
{
    const char __user **filename_user, *orig_filename;
    long ret;
    const struct cred *old_cred;

    if (!sksu_is_allow_uid_for_current(current_uid().val)) {
        goto do_orig_stat;
    }

    filename_user = (const char __user **)&PT_REGS_PARM2(regs);

    char path[sizeof(su_path) + 1];
    memset(path, 0, sizeof(path));
    strncpy_from_user_nofault(path, *filename_user, sizeof(path));

    if (unlikely(!memcmp(path, su_path, sizeof(su_path)))) {
        old_cred = override_creds(sksu_cred);
        if (is_sksud_exists()) {
            pr_info("newfstatat su->sksud!\n");
            orig_filename = *filename_user;
            *filename_user = sksud_user_path();
            ret = sksu_syscall_table[orig_nr](regs);
            revert_creds(old_cred);
            *filename_user = orig_filename;
            return ret;
        } else {
            revert_creds(old_cred);
        }
    }

do_orig_stat:
    return sksu_syscall_table[orig_nr](regs);
}

long sksu_handle_execve_sucompat(const char __user **filename_user, int orig_nr, struct pt_regs *regs)
{
    const char __user *fn;
    const char __user *const __user *argv_user = (const char __user *const __user *)PT_REGS_PARM2(regs);
    struct sksu_sulog_pending_event *pending_sucompat = NULL;
    char path[sizeof(su_path) + 1];
    long ret, orig_regs[5];
    unsigned long addr;
    int tmp_fd;
    struct file *sksud_file;
    const struct cred *old_cred;

    if (unlikely(!filename_user))
        goto do_orig_execve;

    if (!sksu_is_allow_uid_for_current(current_uid().val))
        goto do_orig_execve;

    addr = untagged_addr((unsigned long)*filename_user);
    fn = (const char __user *)addr;
    memset(path, 0, sizeof(path));

    ret = strncpy_from_user(path, fn, sizeof(path));

    if (ret < 0) {
        pr_warn("Access filename when execve failed: %ld", ret);
        goto do_orig_execve;
    }

    if (likely(memcmp(path, su_path, sizeof(su_path))))
        goto do_orig_execve;

    pr_info("sys_execve su found\n");

    tmp_fd = get_unused_fd_flags(O_CLOEXEC);
    if (tmp_fd < 0) {
        pr_err("alloc tmp fd err: %d\n", tmp_fd);
        goto do_orig_execve;
    }

    old_cred = override_creds(sksu_cred);
    sksud_file = filp_open(SKSUD_PATH, O_PATH, 0);
    revert_creds(old_cred);
    if (IS_ERR(sksud_file)) {
        pr_err("open sksud err: %ld\n", PTR_ERR(sksud_file));
        put_unused_fd(tmp_fd);
        goto do_orig_execve;
    }

    fd_install(tmp_fd, sksud_file);

    pending_sucompat = sksu_sulog_capture_sucompat(*filename_user, argv_user, GFP_KERNEL);
    // execve(file, argv, environ)
    // execveat(fd, file, argv, environ, flags)
    orig_regs[0] = regs->__PT_PARM1_REG;
    orig_regs[1] = regs->__PT_PARM2_REG;
    orig_regs[2] = regs->__PT_PARM3_REG;
    orig_regs[3] = regs->__PT_SYSCALL_PARM4_REG;
    orig_regs[4] = regs->__PT_PARM5_REG;
    regs->__PT_PARM5_REG = AT_EMPTY_PATH;
    regs->__PT_SYSCALL_PARM4_REG = regs->__PT_PARM3_REG;
    regs->__PT_PARM3_REG = regs->__PT_PARM2_REG;
    regs->__PT_PARM2_REG = empty_user_path();
    regs->__PT_PARM1_REG = tmp_fd;

    ret = escape_with_root_profile();
    if (ret) {
        pr_err("escape_with_root_profile failed: %ld\n", ret);
    }
    sksu_sulog_emit_pending(pending_sucompat, ret, GFP_KERNEL);

    ret = sksu_syscall_table[__NR_execveat](regs);
    if (ret < 0) {
        sksu_close_fd(tmp_fd);
        regs->__PT_PARM1_REG = orig_regs[0];
        regs->__PT_PARM2_REG = orig_regs[1];
        regs->__PT_PARM3_REG = orig_regs[2];
        regs->__PT_SYSCALL_PARM4_REG = orig_regs[3];
        regs->__PT_PARM5_REG = orig_regs[4];
    }
    return ret;

do_orig_execve:
    return sksu_syscall_table[orig_nr](regs);
}

// sucompat: permitted process can execute 'su' to gain root access.
void __init sksu_sucompat_init()
{
    if (sksu_register_feature_handler(&su_compat_handler)) {
        pr_err("Failed to register su_compat feature handler\n");
    }
}

void __exit sksu_sucompat_exit()
{
    sksu_unregister_feature_handler(SKS_FEATURE_SU_COMPAT);
}
