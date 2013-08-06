#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include "store.h"
#include "real.h"

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>

#include <pthread.h>

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    init();

    struct thread *t = find_thread(pthread_self());
    struct mutex *n = find_mutex(mutex);

    real_mutex_lock(&n->lock);
    struct thread *owner = n->owner;
    if(owner)
    {
        fprintf(stderr, "[%u] mutex_lock(%u) <waiting for thread %u> ...\n", t->num, n->num, owner->num);
        struct timeval starttime;
        gettimeofday(&starttime, 0);
        struct timespec timeout = { starttime.tv_sec, starttime.tv_usec * 1000 };
        timeout.tv_sec += 5;
        while(real_cond_timedwait(&n->cond, &n->lock, &timeout) == ETIMEDOUT)
        {
                fprintf(stderr, "[%u] ... mutex_lock(%u) <still waiting for thread %u> ...\n", t->num, n->num, owner->num);
                timeout.tv_sec += 5;
        }
    }
    int ret = real_mutex_lock(mutex);

    n->owner = t;
    t->owns = n;

    real_mutex_unlock(&n->lock);

    if(owner)
        fprintf(stderr, "[%u] ... mutex_lock(%u)\n", t->num, n->num);
    else
        fprintf(stderr, "[%u] mutex_lock(%u)\n", t->num, n->num);

    return ret;
}
