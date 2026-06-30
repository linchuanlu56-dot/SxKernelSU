#include <linux/kallsyms.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>

#include "infra/symbol_resolver.h"
#include "klog.h"

// https://github.com/torvalds/linux/commit/89245600941e4e0f87d77f60ee269b5e61ef4e49
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0)
#define USE_KCFI 1
#else
#define USE_KCFI 0
#endif

#if !USE_KCFI
static const char cfi_suffix[] = ".cfi_jt";
static const size_t cfi_suffix_len = sizeof(cfi_suffix) - 1;
#endif

// It's not guaranteed to have this symbol exist in 5.x kernel
// https://github.com/torvalds/linux/commit/d721def7392a7348ffb9f3583b264239cbd3702c
// https://github.com/gregkh/linux/commit/2aa861ec72908b4bdc20d74725dc1c8c71a8d214
// https://github.com/gregkh/linux/commit/318a206633c248d876ea72f7133d2a2e50ad7e35
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 19, 0)
#define ALWAYS_HAVE_ON_EACH_SYMBOL 1
#else
#define ALWAYS_HAVE_ON_EACH_SYMBOL 0
#endif

#if !ALWAYS_HAVE_ON_EACH_SYMBOL
static int (*kallsyms_on_each_symbol_fn)(int (*fn)(void *, const char *, struct module *, unsigned long),
                                         void *data) = NULL;
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 1, 0)
#define HAVE_ON_EACH_MATCH_SYMBOL 1
#else
#define HAVE_ON_EACH_MATCH_SYMBOL 0
#endif

// https://github.com/torvalds/linux/commit/4dc533e0f2c04174e1ae4aa98e7cffc1c04b9998
#if HAVE_ON_EACH_MATCH_SYMBOL
static int (*kallsyms_on_each_match_symbol_fn)(int (*fn)(void *, unsigned long), const char *name, void *data) = NULL;
static int find_kernel_symbol_exact_cb(void *data, unsigned long addr)
{
    *(unsigned long *)data = addr;
    return 0;
}
#endif

struct ksu_lookup_symbol_ctx {
    const char *symbol_name;
    size_t symbol_len;
    void *match;
};

unsigned long __nocfi find_kernel_symbol_exact(const char *symbol_name)
{
    unsigned long addr = 0;
#if HAVE_ON_EACH_MATCH_SYMBOL
    if (likely(kallsyms_on_each_match_symbol_fn)) {
        kallsyms_on_each_match_symbol_fn(find_kernel_symbol_exact_cb, symbol_name, &addr);
        return addr;
    }
#endif
    char *module_name = NULL;
    char buf[KSYM_SYMBOL_LEN];

    addr = kallsyms_lookup_name(symbol_name);
    // check if it is kernel symbol
    kallsyms_lookup(addr, NULL, NULL, &module_name, buf);
    if (unlikely(module_name)) {
        pr_warn("ignore symbol %s of module %s\n", symbol_name, module_name);
        return 0;
    }
    return addr;
}

static inline bool ksu_symbol_has_suffix(const char *name, size_t name_len, const char *suffix, size_t suffix_len)
{
    return name_len >= suffix_len && strcmp(name + name_len - suffix_len, suffix) == 0;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
static int lookup_symbol_variant_cb(void *data, const char *name, unsigned long addr)
#else
static int lookup_symbol_variant_cb(void *data, const char *name, struct module *mod, unsigned long addr)
#endif
{
    struct ksu_lookup_symbol_ctx *ctx = data;
    size_t name_len;

    if (!name || !addr)
        return 0;

    name_len = strlen(name);

    if (strcmp(name, ctx->symbol_name) != 0) {
        if (name_len <= ctx->symbol_len || strncmp(name, ctx->symbol_name, ctx->symbol_len) != 0 ||
            (name[ctx->symbol_len] != '.' && name[ctx->symbol_len] != '$'))
            return 0;
    }

#if !USE_KCFI
    if (ksu_symbol_has_suffix(name, name_len, cfi_suffix, cfi_suffix_len)) {
        ctx->match = (void *)addr;
        pr_info("use .cfi_jt variant: %s\n", name);
        return 1;
    }
#endif

    if (!ctx->match) {
        ctx->match = (void *)addr;
        pr_info("found variant: %s\n", name);
#if USE_KCFI
        return 1;
#endif
    }

    return 0;
}

/*
 * Fallback symbol resolver: parse /proc/kallsyms line by line.
 * Used when kallsyms_on_each_symbol() is not available (< 5.19 or not exported).
 *
 * Format per line:  <addr> <type> <name>[<module>]
 * We match <name> against the target symbol name (including variant suffixes).
 */
#if !ALWAYS_HAVE_ON_EACH_SYMBOL
static void resolve_symbol_by_proc_kallsyms(struct ksu_lookup_symbol_ctx *ctx)
{
    struct file *fp;
    loff_t pos = 0;
    char *buf;
    int ret;

    fp = filp_open("/proc/kallsyms", O_RDONLY, 0);
    if (IS_ERR(fp)) {
        pr_debug("resolve_sym: open /proc/kallsyms failed: %ld\n", PTR_ERR(fp));
        return;
    }

    buf = vmalloc(PAGE_SIZE);
    if (!buf) {
        filp_close(fp, NULL);
        return;
    }

    while ((ret = kernel_read(fp, buf, PAGE_SIZE - 1, &pos)) > 0) {
        char *line = buf;
        char *end = buf + ret;
        char *p, *name_start, *addr_str;
        int i;

        buf[ret] = '\0';

        while (line < end) {
            /* Find end of line */
            p = strchr(line, '\n');
            if (!p)
                p = end;
            else
                *p = '\0';

            /* Skip empty lines */
            if (p - line < 3) {
                line = p + 1;
                continue;
            }

            /* Parse: <addr> <type> <name> */
            addr_str = line;
            /* Skip hex address */
            while (*addr_str && *addr_str != ' ')
                addr_str++;
            if (!*addr_str)
                goto next_line;
            *addr_str++ = '\0';
            /* Skip type char */
            if (*addr_str == ' ')
                addr_str++;
            if (!*addr_str || *addr_str == ' ')
                goto next_line;
            addr_str++; /* skip type */

            /* Skip spaces before name */
            while (*addr_str == ' ')
                addr_str++;
            if (!*addr_str)
                goto next_line;

            name_start = addr_str;
            /* Strip module suffix [... ] */
            for (i = 0; name_start[i]; i++) {
                if (name_start[i] == '\t' || name_start[i] == ' ') {
                    name_start[i] = '\0';
                    break;
                }
            }

            if (!name_start[0])
                goto next_line;

            /* Try to match the symbol name */
#if LINUX_VERSION_CODE >= KERNEL_VERSION(6, 6, 0)
            if (lookup_symbol_variant_cb(ctx, name_start, 0)) {
#else
            if (lookup_symbol_variant_cb(ctx, name_start, NULL, 0)) {
#endif
                /* matched! ctx->match is set */
                break;
            }

next_line:
            line = p + 1;
        }

        if (ctx->match)
            break;
    }

    vfree(buf);
    buf = NULL;
    filp_close(fp, NULL);
}
#endif

static __nocfi void *resolve_symbol_variant(const char *symbol_name, size_t symbol_len)
{
    struct ksu_lookup_symbol_ctx ctx = {
        .symbol_name = symbol_name,
        .symbol_len = symbol_len,
    };

#if !ALWAYS_HAVE_ON_EACH_SYMBOL
    if (kallsyms_on_each_symbol_fn) {
        kallsyms_on_each_symbol_fn(lookup_symbol_variant_cb, &ctx);
    }
    if (!ctx.match) {
        /* Fallback: parse /proc/kallsyms when kallsyms_on_each_symbol is
         * unavailable (pre-5.19 kernels that don't export it, or
         * hardened builds with CONFIG_KALLSYMS_SYMBOL_ITERATE disabled).
         */
        pr_info("resolve_sym: falling back to /proc/kallsyms for %s\n", symbol_name);
        resolve_symbol_by_proc_kallsyms(&ctx);
    }
#else
    kallsyms_on_each_symbol(lookup_symbol_variant_cb, &ctx);
#endif
    return ctx.match;
}

void *ksu_resolve_symbol_for_functable_hook(const char *symbol_name)
{
    void *addr;
    size_t symbol_len;

    if (!symbol_name || !symbol_name[0])
        return NULL;

    symbol_len = strlen(symbol_name);

    // Prefer find_kernel_symbol_exact since it uses binary search in higher kernel version

#if !USE_KCFI
    // Try .cfi_jt suffix first
    char cfi_name[KSYM_NAME_LEN];
    snprintf(cfi_name, sizeof(cfi_name), "%s.cfi_jt", symbol_name);
    addr = (void *)find_kernel_symbol_exact(cfi_name);
    if (addr)
        return addr;

    addr = resolve_symbol_variant(symbol_name, symbol_len);
    if (addr)
        return addr;

    return (void *)find_kernel_symbol_exact(symbol_name);
#else
    addr = (void *)find_kernel_symbol_exact(symbol_name);
    if (addr)
        return addr;

    return resolve_symbol_variant(symbol_name, symbol_len);
#endif
}

void __init ksu_init_symbol_resolver()
{
#if !ALWAYS_HAVE_ON_EACH_SYMBOL
    kallsyms_on_each_symbol_fn = find_kernel_symbol_exact("kallsyms_on_each_symbol");
    if (!kallsyms_on_each_symbol_fn) {
        pr_warn("kallsyms_on_each_symbol not found!\n");
    }
#endif
#if HAVE_ON_EACH_MATCH_SYMBOL
    kallsyms_on_each_match_symbol_fn = find_kernel_symbol_exact("kallsyms_on_each_match_symbol");
    if (!kallsyms_on_each_match_symbol_fn) {
        pr_warn("kallsyms_on_each_match_symbol not found!\n");
    }
#endif
}
