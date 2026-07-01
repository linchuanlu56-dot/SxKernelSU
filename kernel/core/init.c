#include <linux/export.h>
#include <linux/fs.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/rcupdate.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/moduleparam.h>

#include "policy/allowlist.h"
#include "policy/app_profile.h"
#include "policy/feature.h"
#include "klog.h" // IWYU pragma: keep
#include "manager/manager_observer.h"
#include "manager/throne_tracker.h"
#include "hook/syscall_hook_manager.h"
#include "hook/lsm_hook.h"
#include "runtime/sksud.h"
#include "runtime/sksud_boot.h"
#include "feature/sulog.h"
#include "supercall/supercall.h"
#include "sksu.h"
#include "infra/file_wrapper.h"
#include "selinux/selinux.h"
#include "hook/syscall_hook.h"
#include "feature/adb_root.h"
#include "feature/selinux_hide.h"
#include "infra/symbol_resolver.h"

#if defined(__x86_64__)
#include <asm/cpufeature.h>
#include <linux/version.h>
#ifndef X86_FEATURE_INDIRECT_SAFE
#error "FATAL: Your kernel is missing the indirect syscall bypass patches!"
#endif
#endif

// workaround for A12-5.10 kernel
// Some third-party kernel (e.g. linegaeOS) uses wrong toolchain, which supports
// CC_HAVE_STACKPROTECTOR_SYSREG while gki's toolchain doesn't.
// Therefore, ksu lkm, which uses gki toolchain, requires this __stack_chk_guard,
// while those third-party kernel can't provide.
// Thus, we manually provide it instead of using kernel's
#if defined(CONFIG_STACKPROTECTOR) &&                                                                                  \
    (defined(CONFIG_ARM64) && defined(MODULE) && !defined(CONFIG_STACKPROTECTOR_PER_TASK))
#include <linux/stackprotector.h>
#include <linux/random.h>
unsigned long __stack_chk_guard __ro_after_init __attribute__((visibility("hidden")));

__attribute__((no_stack_protector)) void __init sksu_setup_stack_chk_guard()
{
    unsigned long canary;

    /* Try to get a semi random initial value. */
    get_random_bytes(&canary, sizeof(canary));
    canary ^= LINUX_VERSION_CODE;
    canary &= CANARY_MASK;
    __stack_chk_guard = canary;
}

__attribute__((naked)) int __init sxkernelsu_init_early(void)
{
    asm("mov x19, x30;\n"
        "bl sksu_setup_stack_chk_guard;\n"
        "mov x30, x19;\n"
        "b sxkernelsu_init;\n");
}
#define NEED_OWN_STACKPROTECTOR 1
#else
#define NEED_OWN_STACKPROTECTOR 0
#endif

struct cred *sksu_cred;
bool sksu_late_loaded;

#ifdef CONFIG_SKS_DEBUG
bool allow_shell = true;
#else
bool allow_shell = false;
#endif
module_param(allow_shell, bool, 0);

bool sksu_no_custom_rc = false;
module_param_named(norc, sksu_no_custom_rc, bool, 0);

int __init sxkernelsu_init(void)
{
#if defined(__x86_64__)
    // If the kernel has the hardening patch, X86_FEATURE_INDIRECT_SAFE must be set
    if (!boot_cpu_has(X86_FEATURE_INDIRECT_SAFE)) {
        pr_alert("*************************************************************");
        pr_alert("**     NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE    **");
        pr_alert("**                                                         **");
        pr_alert("**        X86_FEATURE_INDIRECT_SAFE is not enabled!        **");
        pr_alert("**      SxKernelSU will abort initialization to prevent      **");
        pr_alert("**                     kernel panic.                       **");
        pr_alert("**                                                         **");
        pr_alert("**     NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE    **");
        pr_alert("*************************************************************");
        return -ENOSYS;
    }
#endif

#ifdef MODULE
    sksu_late_loaded = (current->pid != 1);
#else
    sksu_late_loaded = false;
#endif

#ifdef CONFIG_SKS_DEBUG
    pr_alert("*************************************************************");
    pr_alert("**     NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE    **");
    pr_alert("**                                                         **");
    pr_alert("**         You are running SxKernelSU in DEBUG mode          **");
    pr_alert("**                                                         **");
    pr_alert("**     NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE NOTICE    **");
    pr_alert("*************************************************************");
#endif
    if (allow_shell) {
        pr_alert("shell is allowed at init!");
    }

    sksu_cred = prepare_creds();
    if (!sksu_cred) {
        pr_err("prepare cred failed!\n");
        return -ENOSYS;
    }

    sksu_init_symbol_resolver();
    sksu_syscall_hook_init();

    sksu_feature_init();
    sksu_sulog_init();
    sksu_adb_root_init();
    sksu_lsm_hook_init();
    sksu_selinux_hide_init();

    sksu_supercalls_init();

    if (sksu_late_loaded) {
        pr_info("late load mode, skipping kprobe hooks\n");

        apply_sxkernelsu_rules();
        cache_sid();
        setup_sksu_cred();

        // Grant current process (sksud late-load) root
        // with KSU SELinux domain before enforcing SELinux, so it
        // can continue to access /data/app etc. after enforcement.
        escape_to_root_for_init();

        sksu_allowlist_init();
        sksu_load_allow_list();

        sksu_syscall_hook_manager_init();

        sksu_throne_tracker_init();
        sksu_observer_init();
        sksu_file_wrapper_init();

        sksu_boot_completed = true;
        track_throne(false);

        if (!getenforce()) {
            pr_info("Permissive SELinux, enforcing\n");
            setenforce(true);
        }

    } else {
        sksu_syscall_hook_manager_init();

        sksu_allowlist_init();

        sksu_throne_tracker_init();

        sksu_sksud_init();

        sksu_file_wrapper_init();
    }

#ifdef MODULE
#ifndef CONFIG_SKS_DEBUG
    kobject_del(&THIS_MODULE->mkobj.kobj);
#endif
#endif
    return 0;
}

void __exit sxkernelsu_exit(void)
{
    // Phase 1: Stop all hooks first to prevent new callbacks
    sksu_syscall_hook_manager_exit();

    sksu_supercalls_exit();

    if (!sksu_late_loaded)
        sksu_sksud_exit();

    // Wait for any in-flight RCU readers (e.g. handler traversing allow_list)
    synchronize_rcu();

    // Phase 2: Now safe to release data structures
    sksu_observer_exit();

    sksu_throne_tracker_exit();

    sksu_allowlist_exit();

    sksu_selinux_hide_exit();
    sksu_lsm_hook_exit();
    sksu_adb_root_exit();
    sksu_sulog_exit();
    sksu_feature_exit();

    put_cred(sksu_cred);
}

#if NEED_OWN_STACKPROTECTOR
module_init(sxkernelsu_init_early);
#else
module_init(sxkernelsu_init);
#endif
module_exit(sxkernelsu_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("weishu");
MODULE_DESCRIPTION("Android SxKernelSU");
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 13, 0)
MODULE_IMPORT_NS("VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver");
#else
MODULE_IMPORT_NS(VFS_internal_I_am_really_a_filesystem_and_am_NOT_a_driver);
#endif
/* SxKernelSU CI trigger */


/* verify ci */

/* SxKernelSU final */
