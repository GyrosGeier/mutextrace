#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "real.h"

#include <dlfcn.h>

int (*real_create)(pthread_t *, pthread_attr_t const *, void *(*)(void *), void *);

int (*real_mutex_init)(pthread_mutex_t *, pthread_mutexattr_t const *);
int (*real_mutex_destroy)(pthread_mutex_t *);
int (*real_mutex_lock)(pthread_mutex_t *);
int (*real_mutex_unlock)(pthread_mutex_t *);

int (*real_cond_init)(pthread_cond_t *, pthread_condattr_t const *);
int (*real_cond_wait)(pthread_cond_t *, pthread_mutex_t *);
int (*real_cond_signal)(pthread_cond_t *);

void init(void)
{
    static int initialized = 0;
    if(initialized)
        return;

    initialized = 1;

    real_create = dlsym(RTLD_NEXT, "pthread_create");
    real_mutex_init = dlsym(RTLD_NEXT, "pthread_mutex_init");
    real_mutex_destroy = dlsym(RTLD_NEXT, "pthread_mutex_destroy");
    real_mutex_lock = dlsym(RTLD_NEXT, "pthread_mutex_lock");
    real_mutex_unlock = dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    real_cond_init = dlsym(RTLD_NEXT, "pthread_cond_init");
    real_cond_wait = dlsym(RTLD_NEXT, "pthread_cond_wait");
    real_cond_signal = dlsym(RTLD_NEXT, "pthread_cond_signal");

    return;
}
