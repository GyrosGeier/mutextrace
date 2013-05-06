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

    int kind = PTHREAD_MUTEX_FAST_NP;
    if(attr)
        pthread_mutexattr_gettype(attr, &kind);

    char const *kind_str;
    switch(kind)
    {
    case PTHREAD_MUTEX_FAST_NP:
        kind_str = "FAST";
        break;
    case PTHREAD_MUTEX_RECURSIVE_NP:
        kind_str = "RECURSIVE";
        break;
    case PTHREAD_MUTEX_ERRORCHECK_NP:
        kind_str = "ERRORCHECK";
        break;
    default:
        kind_str = "INVALID";
    }

    fprintf(stderr, "[%u] mutex_init(%u, %s)\n", t->num, n->num, kind_str);
    real_mutex_lock(&n->lock);
    n->state = unlocked;
    n->kind = kind;

    int res = real_mutex_init(mutex, attr);

    real_mutex_unlock(&n->lock);

    return res;
}
