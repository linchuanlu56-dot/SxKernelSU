#ifndef __SKS_H_SEPOLICY
#define __SKS_H_SEPOLICY

#include <linux/types.h>

#include "ss/policydb.h"

struct selinux_policy *sksu_dup_sepolicy(struct selinux_policy *old_pol);

void sksu_destroy_sepolicy(struct selinux_policy *orig);

// Operation on types
bool sksu_type(struct policydb *db, const char *name, const char *attr);
bool sksu_attribute(struct policydb *db, const char *name);
bool sksu_permissive(struct policydb *db, const char *type);
bool sksu_enforce(struct policydb *db, const char *type);
bool sksu_typeattribute(struct policydb *db, const char *type, const char *attr);
bool sksu_exists(struct policydb *db, const char *type);

// Access vector rules
bool sksu_allow(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *perm);
bool sksu_deny(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *perm);
bool sksu_auditallow(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *perm);
bool sksu_dontaudit(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *perm);

// Extended permissions access vector rules
bool sksu_allowxperm(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *range);
bool sksu_auditallowxperm(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *range);
bool sksu_dontauditxperm(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *range);

// Type rules
bool sksu_type_transition(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *def,
                         const char *obj);
bool sksu_type_change(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *def);
bool sksu_type_member(struct policydb *db, const char *src, const char *tgt, const char *cls, const char *def);

// File system labeling
bool sksu_genfscon(struct policydb *db, const char *fs_name, const char *path, const char *ctx);

#endif
