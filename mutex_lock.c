#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include "store.h"
#include "real.h"

#include <stdio.h>

#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    init();

    struct thread *t = find_thread(pthread_self());
    struct mutex *n = find_mutex(mutex);

    real_mutex_lock(&n->lock);
    struct thread *owner = n->owner;
    if(owner)
        real_cond_wait(&n->cond, &n->lock);
    int ret = real_mutex_lock(mutex);

    n->owner = t;
    t->owns = n;

    real_mutex_unlock(&n->lock);

    if(owner)
        fprintf(stderr, "lock #%u, thread %u -> thread %u\n", n->num, owner->num, t->num);
    else
        fprintf(stderr, "lock #%u, free -> thread %u\n", n->num, t->num);

    return ret;
}
