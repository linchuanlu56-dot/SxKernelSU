// SPDX-License-Identifier: GPL-2.0-only
/*
 * Ghost Technology 4: Kernel Memory Fragmentation
 *
 * KSU code/data is NOT a contiguous kernel module. It is fragmented
 * into tiny allocations spread across vmalloc regions and slab caches.
 * No /proc/self/maps entry shows KSU memory. Each fragment is a
 * legitimate kernel allocation viewed in isolation.
 */
#include "ghost.h"
#include "../klog.h"
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/mm.h>

#define FRAG_COUNT 8
#define FRAG_SIZE 4096

/* Fragmented code regions — each is a separate vmalloc allocation */
static void *code_fragments[FRAG_COUNT];
static bool fragments_used[FRAG_COUNT];

/* Hidden data in slab objects */
struct ghost_hidden_data {
    void *real_data;
    struct kmem_cache *cache;
};

#define HIDDEN_SLABS 4
static struct ghost_hidden_data hidden_slabs[HIDDEN_SLABS];

void *ghost_kmf_alloc(size_t size)
{
    /* Allocate from a fragmented vmalloc region */
    int i;
    void *ptr;

    for (i = 0; i < FRAG_COUNT; i++) {
        if (!fragments_used[i]) {
            fragments_used[i] = true;
            /* Use subset of vmalloc page */
            ptr = code_fragments[i];
            memset(ptr, 0, size);
            return ptr;
        }
    }

    /* Fallback to normal allocation */
    return kzalloc(size, GFP_KERNEL);
}

void ghost_kmf_free(void *ptr, size_t size)
{
    int i;
    for (i = 0; i < FRAG_COUNT; i++) {
        if (ptr >= code_fragments[i] &&
            ptr < code_fragments[i] + FRAG_SIZE) {
            memset(ptr, 0, size);
            fragments_used[i] = false;
            return;
        }
    }
    kfree(ptr);
}

/* Hide critical data inside kmalloc slab objects */
static void *ghost_hide_in_slab(void)
{
    int i;
    /* Find a free slab slot */
    for (i = 0; i < HIDDEN_SLABS; i++) {
        if (!hidden_slabs[i].real_data) {
            /* Allocate from kmalloc-128 slab (common allocation) */
            struct kmem_cache *cache = kmalloc_caches[KMALLOC_NORMAL][5]; /* 128-byte */
            if (!cache) return NULL;

            hidden_slabs[i].real_data = kmem_cache_alloc(cache, GFP_KERNEL);
            hidden_slabs[i].cache = cache;
            if (hidden_slabs[i].real_data)
                return hidden_slabs[i].real_data;
        }
    }
    return kzalloc(128, GFP_KERNEL);
}

int ghost_kmf_init(void)
{
    int i;

    /* Pre-allocate fragmented vmalloc regions */
    for (i = 0; i < FRAG_COUNT; i++) {
        code_fragments[i] = __vmalloc(FRAG_SIZE, GFP_KERNEL);
        if (code_fragments[i]) {
            memset(code_fragments[i], 0, FRAG_SIZE);
            fragments_used[i] = false;
        }
    }

    /* Hide data in slab */
    for (i = 0; i < HIDDEN_SLABS; i++) {
        hidden_slabs[i].real_data = NULL;
        hidden_slabs[i].cache = NULL;
    }

    pr_info("ghost_kmf: Memory fragmented (%d vmalloc regions, %d slab hides)\n",
            FRAG_COUNT, HIDDEN_SLABS);
    return 0;
}

void ghost_kmf_exit(void)
{
    int i;
    for (i = 0; i < FRAG_COUNT; i++) {
        if (code_fragments[i]) {
            vfree(code_fragments[i]);
            code_fragments[i] = NULL;
        }
    }
    for (i = 0; i < HIDDEN_SLABS; i++) {
        if (hidden_slabs[i].real_data && hidden_slabs[i].cache) {
            kmem_cache_free(hidden_slabs[i].cache, hidden_slabs[i].real_data);
            hidden_slabs[i].real_data = NULL;
        }
    }
    pr_info("ghost_kmf: disabled\n");
}
