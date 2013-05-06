#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include "store.h"
#include "real.h"

#include <stdio.h>

#include <pthread.h>

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    init();

    struct thread *t = find_thread(pthread_self());
    struct mutex *n = find_mutex(mutex);

    real_mutex_lock(&n->lock);
    n->owner = 0;
    real_cond_signal(&n->cond);
    int res = real_mutex_unlock(mutex);
    real_mutex_unlock(&n->lock);

    fprintf(stderr, "[%u] mutex_unlock(%u)\n", t->num, n->num);

    return res;
}
