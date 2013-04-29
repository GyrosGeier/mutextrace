#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "store.h"
#include "real.h"

#include <stdio.h>

struct startupinfo
{
    void *(*start_routine)(void *);
    void *arg;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};

static void cleanup(void *thread)
{
    struct thread *t = (struct thread *)thread;

    fprintf(stderr, "thread exit #%u\n", t->num);
}

static void *startup(void *startupinfo)
{
    struct startupinfo *sui = (struct startupinfo *)startupinfo;

    void *(*start_routine)(void *) = sui->start_routine;
    void *arg = sui->arg;
    
    real_mutex_lock(&sui->mutex);
    real_cond_signal(&sui->cond);
    real_mutex_unlock(&sui->mutex);

    struct thread *t = find_thread(pthread_self());

    fprintf(stderr, "thread create #%u\n", t->num);

    void *res;

    pthread_cleanup_push(&cleanup, t);

    res = start_routine(arg);

    pthread_cleanup_pop(1);

    return res;
}

int pthread_create(
        pthread_t *thread,
        pthread_attr_t const *attr,
        void *(*start_routine)(void*),
        void *arg)
{
    init();

    struct startupinfo sui = { start_routine, arg, {}, {} };
    real_cond_init(&sui.cond, 0);
    real_mutex_init(&sui.mutex, 0);

    real_mutex_lock(&sui.mutex);

    int res = real_create(thread, attr, &startup, &sui);

    real_cond_wait(&sui.cond, &sui.mutex);

    real_mutex_unlock(&sui.mutex);

    return res;
}
