#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include "store.h"

#include "real.h"

#include <stdio.h>

#include <pthread.h>

int pthread_mutex_init(pthread_mutex_t *mutex, pthread_mutexattr_t const *attr)
{
    init();

    struct thread *t = find_thread(pthread_self());
    struct mutex *n = find_mutex(mutex);

    fprintf(stderr, "[%u] mutex_init(%u)\n", t->num, n->num);
    real_mutex_lock(&n->lock);
    n->state = unlocked;

    int res = real_mutex_init(mutex, attr);

    real_mutex_unlock(&n->lock);

    return res;
}
