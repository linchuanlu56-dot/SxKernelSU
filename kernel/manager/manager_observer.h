#ifndef __SKS_H_MANAGER_OBSERVER
#define __SKS_H_MANAGER_OBSERVER

#ifdef CONFIG_SKS_DISABLE_MANAGER
static inline int sksu_observer_init(void)
{
    return 0;
}

static inline void sksu_observer_exit(void)
{
}
#else
int sksu_observer_init(void);
void sksu_observer_exit(void);
#endif

#endif // __SKS_H_MANAGER_OBSERVER
