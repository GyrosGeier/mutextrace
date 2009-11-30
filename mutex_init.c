#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "store.h"

#include "real.h"

#include <stdio.h>

#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t const *attr)
{
    struct mutex *n = find_mutex(mutex);

    fprintf(stderr, "init #%u\n", n->num);
    real_mutex_lock(&n->lock);
    n->state = unlocked;

    int res = real_mutex_init(mutex, attr);

    real_mutex_unlock(&n->lock);

    return res;
}
