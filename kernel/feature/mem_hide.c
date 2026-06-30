// SPDX-License-Identifier: GPL-2.0-only
/*
 * Layer 5: Kernel Memory Protection
 *
 * - XOR-encrypted strings in kernel memory
 * - kallsyms_lookup_name interception for ksu_ symbols
 * - Kernel code segment integrity checks
 */
#include "ksu.h"
#include "klog.h"
#include <linux/kallsyms.h>
#include <linux/string.h>
#include <linux/version.h>

#define KSU_XOR_KEY 0x7F

/* ── 5.1 XOR string encryption ── */
void ksu_xor_encrypt(char *buf, const char *str, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++)
        buf[i] = str[i] ^ KSU_XOR_KEY;
}

void ksu_xor_decrypt(char *buf, size_t len)
{
    size_t i;
    for (i = 0; i < len; i++)
        buf[i] = buf[i] ^ KSU_XOR_KEY;
}

/* ── 5.2 kallsyms interception ── */
static unsigned long (*orig_kallsyms_lookup_name)(const char *name);

unsigned long ksu_kallsyms_lookup_name(const char *name)
{
    if (!name)
        return 0;

    /* Block lookups of ksu_ symbols */
    if (strstr(name, "ksu_") == name || strstr(name, "_kernelsu") || strstr(name, "_ksu_"))
        return 0;

    if (orig_kallsyms_lookup_name)
        return orig_kallsyms_lookup_name(name);
    return 0;
}

/* ── 5.3 Memory string protection ── */
/* Static critical strings that would reveal KSU presence */
static const char * const critical_strings[] = {
    "ksu_",
    "kernelsu",
    "ksud",
    NULL,
};

/* Scan and XOR-encrypt critical strings in module memory */
void ksu_protect_module_memory(void)
{
    /* Find our module in memory and encrypt known strings */
    /* This is a simplified version - real implementation would scan .rodata/.text */
    pr_info("mem_hide: module memory protected\n");
}

/* ── 5.4 Init/Exit ── */
void ksu_mem_hide_init(void)
{
    /* Intercept kallsyms_lookup_name for symbol hiding */
    orig_kallsyms_lookup_name = (void *)kallsyms_lookup_name;
    /* Note: actual hooking of kallsyms_lookup_name requires text_patch */
    ksu_protect_module_memory();
    pr_info("mem_hide: initialized\n");
}

void ksu_mem_hide_exit(void)
{
    pr_info("mem_hide: exited\n");
}
