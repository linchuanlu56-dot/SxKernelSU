#include "linux/rcupdate.h"
#include "security.h"
#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/version.h>
#include <linux/lockdep.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "uapi/selinux.h"
#include "klog.h" // IWYU pragma: keep
#include "selinux.h"
#include "sepolicy.h"
#include "ss/services.h"
#include "linux/lsm_audit.h" // IWYU pragma: keep
#include "xfrm.h"

struct selinux_policy *backup_sepolicy;

#define SELINUX_POLICY_INSTEAD_SELINUX_SS

#define ALL NULL

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0))
extern int avc_ss_reset(u32 seqno);
#else
extern int avc_ss_reset(struct selinux_avc *avc, u32 seqno);
#endif
// reset avc cache table, otherwise the new rules will not take effect if already denied
static void reset_avc_cache()
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(6, 4, 0))
    avc_ss_reset(0);
    selnl_notify_policyload(0);
    selinux_status_update_policyload(0);
#else
    struct selinux_avc *avc = selinux_state.avc;
    avc_ss_reset(avc, 0);
    selnl_notify_policyload(0);
    selinux_status_update_policyload(&selinux_state, 0);
#endif
    selinux_xfrm_notify_policyload();
}

void apply_sxkernelsu_rules()
{
    struct selinux_policy *pol, *old_pol = selinux_state.policy;
    struct policydb *db;

    if (!getenforce()) {
        pr_info("SELinux permissive or disabled, apply rules!\n");
    }

    mutex_lock(&selinux_state.policy_mutex);
    backup_sepolicy =
        sksu_dup_sepolicy(rcu_dereference_protected(old_pol, lockdep_is_held(&selinux_state.policy_mutex)));
    if (IS_ERR(backup_sepolicy)) {
        pr_err("failed to create backup sepolicy: %ld\n", PTR_ERR(backup_sepolicy));
        backup_sepolicy = NULL;
    } else {
        backup_sepolicy->sidtab = kzalloc(sizeof(*backup_sepolicy->sidtab), GFP_KERNEL);
        if (!backup_sepolicy->sidtab) {
            pr_err("failed to alloc backup sidtab\n");
            sksu_destroy_sepolicy(backup_sepolicy);
            backup_sepolicy = NULL;
        } else {
            int ret = policydb_load_isids(&backup_sepolicy->policydb, backup_sepolicy->sidtab);
            if (ret) {
                pr_err("failed to load isids for backup sepolicy: %d!\n", ret);
                kfree(backup_sepolicy->sidtab);
                sksu_destroy_sepolicy(backup_sepolicy);
                backup_sepolicy = NULL;
            } else {
                pr_info("backup sepolicy success! latest_granting=%d\n", backup_sepolicy->latest_granting);
            }
        }
    }
    pol = sksu_dup_sepolicy(rcu_dereference_protected(old_pol, lockdep_is_held(&selinux_state.policy_mutex)));
    if (IS_ERR(pol)) {
        pr_err("failed to dup selinux_policy: %ld\n", PTR_ERR(pol));
        goto out_unlock;
    }

    db = &pol->policydb;

    sksu_type(db, SXKERNEL_SU_DOMAIN, "domain");
    sksu_permissive(db, SXKERNEL_SU_DOMAIN);
    sksu_typeattribute(db, SXKERNEL_SU_DOMAIN, "mlstrustedsubject");
    sksu_typeattribute(db, SXKERNEL_SU_DOMAIN, "netdomain");
    sksu_typeattribute(db, SXKERNEL_SU_DOMAIN, "bluetoothdomain");

    // Create unconstrained file type
    sksu_type(db, SXKERNEL_SU_FILE, "file_type");
    sksu_typeattribute(db, SXKERNEL_SU_FILE, "mlstrustedobject");
    sksu_allow(db, "domain", SXKERNEL_SU_FILE, ALL, ALL);

    // allow all!
    sksu_allow(db, SXKERNEL_SU_DOMAIN, ALL, ALL, ALL);

    // allow us do any ioctl
    if (db->policyvers >= POLICYDB_VERSION_XPERMS_IOCTL) {
        sksu_allowxperm(db, SXKERNEL_SU_DOMAIN, ALL, "blk_file", ALL);
        sksu_allowxperm(db, SXKERNEL_SU_DOMAIN, ALL, "fifo_file", ALL);
        sksu_allowxperm(db, SXKERNEL_SU_DOMAIN, ALL, "chr_file", ALL);
        sksu_allowxperm(db, SXKERNEL_SU_DOMAIN, ALL, "file", ALL);
    }

    // our sksud triggered by init
    sksu_allow(db, "init", SXKERNEL_SU_DOMAIN, ALL, ALL);

    // copied from Magisk rules
    // suRights
    sksu_allow(db, "servicemanager", SXKERNEL_SU_DOMAIN, "dir", "search");
    sksu_allow(db, "servicemanager", SXKERNEL_SU_DOMAIN, "dir", "read");
    sksu_allow(db, "servicemanager", SXKERNEL_SU_DOMAIN, "file", "open");
    sksu_allow(db, "servicemanager", SXKERNEL_SU_DOMAIN, "file", "read");
    sksu_allow(db, "servicemanager", SXKERNEL_SU_DOMAIN, "process", "getattr");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "process", "sigchld");

    // allowLog
    sksu_allow(db, "logd", SXKERNEL_SU_DOMAIN, "dir", "search");
    sksu_allow(db, "logd", SXKERNEL_SU_DOMAIN, "file", "read");
    sksu_allow(db, "logd", SXKERNEL_SU_DOMAIN, "file", "open");
    sksu_allow(db, "logd", SXKERNEL_SU_DOMAIN, "file", "getattr");

    // dumpsys, send fd
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "fd", "use");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "fifo_file", "write");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "fifo_file", "read");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "fifo_file", "open");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "fifo_file", "getattr");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "unix_stream_socket", "read");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "unix_stream_socket", "write");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "unix_stream_socket", "connectto");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "unix_stream_socket", "getopt");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "unix_stream_socket", "getattr");

    // use memfd created by su domain
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "memfd_file", "execute");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "memfd_file", "getattr");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "memfd_file", "map");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "memfd_file", "read");
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "memfd_file", "write");

    // bootctl
    sksu_allow(db, "hwservicemanager", SXKERNEL_SU_DOMAIN, "dir", "search");
    sksu_allow(db, "hwservicemanager", SXKERNEL_SU_DOMAIN, "file", "read");
    sksu_allow(db, "hwservicemanager", SXKERNEL_SU_DOMAIN, "file", "open");
    sksu_allow(db, "hwservicemanager", SXKERNEL_SU_DOMAIN, "process", "getattr");

    // Allow all binder transactions
    sksu_allow(db, "domain", SXKERNEL_SU_DOMAIN, "binder", ALL);

    // Allow system server kill su process
    sksu_allow(db, "system_server", SXKERNEL_SU_DOMAIN, "process", "getpgid");
    sksu_allow(db, "system_server", SXKERNEL_SU_DOMAIN, "process", "sigkill");

    rcu_assign_pointer(selinux_state.policy, pol);
    synchronize_rcu();
    sksu_destroy_sepolicy(old_pol);

    reset_avc_cache();
out_unlock:
    mutex_unlock(&selinux_state.policy_mutex);
}

#define SKS_SEPOLICY_MAX_BATCH_SIZE (8U * 1024U * 1024U)
#define SKS_SEPOLICY_MAX_ARGS 5

struct sepol_data {
    u32 cmd;
    u32 subcmd;
};

struct sepol_batch_cursor {
    const u8 *cur;
    const u8 *end;
};

static size_t sepol_remaining(const struct sepol_batch_cursor *cursor)
{
    return (size_t)(cursor->end - cursor->cur);
}

static int sepol_read_cmd_header(struct sepol_batch_cursor *cursor, struct sepol_data *header)
{
    if (sepol_remaining(cursor) < sizeof(*header)) {
        return -EINVAL;
    }

    memcpy(header, cursor->cur, sizeof(*header));
    cursor->cur += sizeof(*header);

    return 0;
}

static int sepol_read_string(struct sepol_batch_cursor *cursor, const char **out)
{
    u32 len;
    const char *str;

    if (sepol_remaining(cursor) < sizeof(len)) {
        return -EINVAL;
    }

    memcpy(&len, cursor->cur, sizeof(len));
    cursor->cur += sizeof(len);

    if (len >= sepol_remaining(cursor)) {
        return -EINVAL;
    }

    str = (const char *)cursor->cur;
    if (memchr(str, '\0', len) != NULL || str[len] != '\0') {
        return -EINVAL;
    }

    cursor->cur += len + 1;
    if (len == 0) {
        *out = ALL;
        return 0;
    }

    *out = str;
    return 0;
}

static int sepol_require_not_all(const char *value, const char *name)
{
    if (value != ALL) {
        return 0;
    }

    pr_err("sepol: %s cannot be ALL.\n", name);
    return -EINVAL;
}

static int sepol_expected_argc(u32 cmd)
{
    switch (cmd) {
    case SKS_SEPOLICY_CMD_NORMAL_PERM:
        return 4;
    case SKS_SEPOLICY_CMD_XPERM:
        return 5;
    case SKS_SEPOLICY_CMD_TYPE_STATE:
        return 1;
    case SKS_SEPOLICY_CMD_TYPE:
    case SKS_SEPOLICY_CMD_TYPE_ATTR:
        return 2;
    case SKS_SEPOLICY_CMD_ATTR:
        return 1;
    case SKS_SEPOLICY_CMD_TYPE_TRANSITION:
        return 5;
    case SKS_SEPOLICY_CMD_TYPE_CHANGE:
        return 4;
    case SKS_SEPOLICY_CMD_GENFSCON:
        return 3;
    default:
        return -EINVAL;
    }
}

static int apply_one_sepolicy_cmd(struct policydb *db, const struct sepol_data *header, const char **args)
{
    bool success = false;
    int ret;

    switch (header->cmd) {
    case SKS_SEPOLICY_CMD_NORMAL_PERM:
        if (header->subcmd == SKS_SEPOLICY_SUBCMD_NORMAL_PERM_ALLOW) {
            success = sksu_allow(db, args[0], args[1], args[2], args[3]);
        } else if (header->subcmd == SKS_SEPOLICY_SUBCMD_NORMAL_PERM_DENY) {
            success = sksu_deny(db, args[0], args[1], args[2], args[3]);
        } else if (header->subcmd == SKS_SEPOLICY_SUBCMD_NORMAL_PERM_AUDITALLOW) {
            success = sksu_auditallow(db, args[0], args[1], args[2], args[3]);
        } else if (header->subcmd == SKS_SEPOLICY_SUBCMD_NORMAL_PERM_DONTAUDIT) {
            success = sksu_dontaudit(db, args[0], args[1], args[2], args[3]);
        } else {
            pr_err("sepol: unknown subcmd: %d\n", header->subcmd);
        }
        return success ? 0 : -EINVAL;

    case SKS_SEPOLICY_CMD_XPERM:
        ret = sepol_require_not_all(args[3], "operation");
        if (ret < 0) {
            return ret;
        }
        ret = sepol_require_not_all(args[4], "perm_set");
        if (ret < 0) {
            return ret;
        }

        if (header->subcmd == SKS_SEPOLICY_SUBCMD_XPERM_ALLOW) {
            success = sksu_allowxperm(db, args[0], args[1], args[2], args[4]);
        } else if (header->subcmd == SKS_SEPOLICY_SUBCMD_XPERM_AUDITALLOW) {
            success = sksu_auditallowxperm(db, args[0], args[1], args[2], args[4]);
        } else if (header->subcmd == SKS_SEPOLICY_SUBCMD_XPERM_DONTAUDIT) {
            success = sksu_dontauditxperm(db, args[0], args[1], args[2], args[4]);
        } else {
            pr_err("sepol: unknown subcmd: %d\n", header->subcmd);
        }
        return success ? 0 : -EINVAL;

    case SKS_SEPOLICY_CMD_TYPE_STATE:
        ret = sepol_require_not_all(args[0], "type");
        if (ret < 0) {
            return ret;
        }

        if (header->subcmd == SKS_SEPOLICY_SUBCMD_TYPE_STATE_PERMISSIVE) {
            success = sksu_permissive(db, args[0]);
        } else if (header->subcmd == SKS_SEPOLICY_SUBCMD_TYPE_STATE_ENFORCE) {
            success = sksu_enforce(db, args[0]);
        } else {
            pr_err("sepol: unknown subcmd: %d\n", header->subcmd);
        }
        return success ? 0 : -EINVAL;

    case SKS_SEPOLICY_CMD_TYPE:
    case SKS_SEPOLICY_CMD_TYPE_ATTR:
        ret = sepol_require_not_all(args[0], "type");
        if (ret < 0) {
            return ret;
        }
        ret = sepol_require_not_all(args[1], "attribute");
        if (ret < 0) {
            return ret;
        }

        if (header->cmd == SKS_SEPOLICY_CMD_TYPE) {
            success = sksu_type(db, args[0], args[1]);
        } else {
            success = sksu_typeattribute(db, args[0], args[1]);
        }
        if (!success) {
            pr_err("sepol: %d failed.\n", header->cmd);
            return -EINVAL;
        }
        return 0;

    case SKS_SEPOLICY_CMD_ATTR:
        ret = sepol_require_not_all(args[0], "attribute");
        if (ret < 0) {
            return ret;
        }

        if (!sksu_attribute(db, args[0])) {
            pr_err("sepol: %d failed.\n", header->cmd);
            return -EINVAL;
        }
        return 0;

    case SKS_SEPOLICY_CMD_TYPE_TRANSITION: {
        const char *object = ALL;

        ret = sepol_require_not_all(args[0], "src");
        if (ret < 0) {
            return ret;
        }
        ret = sepol_require_not_all(args[1], "tgt");
        if (ret < 0) {
            return ret;
        }
        ret = sepol_require_not_all(args[2], "cls");
        if (ret < 0) {
            return ret;
        }
        ret = sepol_require_not_all(args[3], "default_type");
        if (ret < 0) {
            return ret;
        }

        object = args[4];

        success = sksu_type_transition(db, args[0], args[1], args[2], args[3], object);
        return success ? 0 : -EINVAL;
    }

    case SKS_SEPOLICY_CMD_TYPE_CHANGE:
        ret = sepol_require_not_all(args[0], "src");
        if (ret < 0) {
            return ret;
        }
        ret = sepol_require_not_all(args[1], "tgt");
        if (ret < 0) {
            return ret;
        }
        ret = sepol_require_not_all(args[2], "cls");
        if (ret < 0) {
            return ret;
        }
        ret = sepol_require_not_all(args[3], "default_type");
        if (ret < 0) {
            return ret;
        }

        if (header->subcmd == SKS_SEPOLICY_SUBCMD_TYPE_CHANGE_CHANGE) {
            success = sksu_type_change(db, args[0], args[1], args[2], args[3]);
        } else if (header->subcmd == SKS_SEPOLICY_SUBCMD_TYPE_CHANGE_MEMBER) {
            success = sksu_type_member(db, args[0], args[1], args[2], args[3]);
        } else {
            pr_err("sepol: unknown subcmd: %d\n", header->subcmd);
        }
        return success ? 0 : -EINVAL;

    case SKS_SEPOLICY_CMD_GENFSCON:
        ret = sepol_require_not_all(args[0], "name");
        if (ret < 0) {
            return ret;
        }
        ret = sepol_require_not_all(args[1], "path");
        if (ret < 0) {
            return ret;
        }
        ret = sepol_require_not_all(args[2], "context");
        if (ret < 0) {
            return ret;
        }

        if (!sksu_genfscon(db, args[0], args[1], args[2])) {
            pr_err("sepol: %d failed.\n", header->cmd);
            return -EINVAL;
        }
        return 0;

    default:
        pr_err("sepol: unknown cmd: %d\n", header->cmd);
        return -EINVAL;
    }
}

int handle_sepolicy(void __user *user_data, u64 data_len)
{
    struct selinux_policy *pol, *old_pol;
    struct policydb *db;
    struct sepol_batch_cursor cursor;
    u8 *payload;
    int ret;
    int success_cmd_count;
    u32 cmd_index;

    if (!user_data || !data_len) {
        return -EINVAL;
    }

    if (data_len > SKS_SEPOLICY_MAX_BATCH_SIZE) {
        return -E2BIG;
    }

    payload = kvmalloc((size_t)data_len, GFP_KERNEL);
    if (!payload) {
        return -ENOMEM;
    }

    if (copy_from_user(payload, user_data, (size_t)data_len)) {
        ret = -EFAULT;
        goto out_free;
    }

    if (!getenforce()) {
        pr_info("SELinux permissive or disabled when handle policy!\n");
    }

    mutex_lock(&selinux_state.policy_mutex);

    old_pol = selinux_state.policy;
    pol = sksu_dup_sepolicy(rcu_dereference_protected(old_pol, lockdep_is_held(&selinux_state.policy_mutex)));
    if (IS_ERR(pol)) {
        ret = PTR_ERR(pol);
        pr_err("sksu_dup_sepolicy err: %d\n", ret);
        goto out_unlock;
    }
    db = &pol->policydb;

    cursor.cur = payload;
    cursor.end = payload + (size_t)data_len;

    ret = 0;
    success_cmd_count = 0;
    cmd_index = 0;
    while (cursor.cur < cursor.end) {
        struct sepol_data header;
        const char *args[SKS_SEPOLICY_MAX_ARGS] = { 0 };
        int expected_argc;
        u32 arg_index;

        ret = sepol_read_cmd_header(&cursor, &header);
        if (ret < 0) {
            pr_err("sepol: failed to read cmd header #%u.\n", cmd_index);
            goto out_drop_new_policy;
        }

        expected_argc = sepol_expected_argc(header.cmd);
        if (expected_argc < 0 || expected_argc > SKS_SEPOLICY_MAX_ARGS) {
            ret = -EINVAL;
            pr_err("sepol: invalid cmd header #%u.\n", cmd_index);
            goto out_drop_new_policy;
        }

        for (arg_index = 0; arg_index < (u32)expected_argc; arg_index++) {
            ret = sepol_read_string(&cursor, &args[arg_index]);
            if (ret < 0) {
                pr_err("sepol: failed to read cmd #%u arg #%u.\n", cmd_index, arg_index);
                goto out_drop_new_policy;
            }
        }

        ret = apply_one_sepolicy_cmd(db, &header, args);
        if (ret < 0) {
            pr_err("sepol: cmd #%u failed, cmd=%u subcmd=%u.\n", cmd_index, header.cmd, header.subcmd);
        } else {
            success_cmd_count++;
        }
        cmd_index++;
    }

    rcu_assign_pointer(selinux_state.policy, pol);
    synchronize_rcu();
    sksu_destroy_sepolicy(old_pol);

    reset_avc_cache();
    ret = success_cmd_count;
    goto out_unlock;

out_drop_new_policy:
    sksu_destroy_sepolicy(pol);
out_unlock:
    mutex_unlock(&selinux_state.policy_mutex);
out_free:
    kvfree(payload);

    return ret;
}
