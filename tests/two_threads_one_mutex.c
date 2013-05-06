#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include <pthread.h>

#include <unistd.h>

#include "common.h"

void *thread_func(void *p)
{
    pthread_mutex_t *mutex = (pthread_mutex_t *)p;

    pthread_mutex_lock(mutex);
    pthread_mutex_unlock(mutex);

    return 0;
}

int test_main()
{
    pthread_t thread;
    pthread_mutex_t mutex;

    if(pthread_mutex_init(&mutex, 0))
        return 1;

    if(pthread_mutex_lock(&mutex))
        return 1;

    if(pthread_create(&thread, 0, &thread_func, &mutex))
        return 1;

    usleep(500);

    if(pthread_mutex_unlock(&mutex))
        return 1;

    void *res;

    if(pthread_join(thread, &res))
        return 1;

    if(pthread_mutex_destroy(&mutex))
        return 1;

    return 0;
}

char const test_results[] =
    "[1] mutex_init(1, FAST)\n"
    "[1] mutex_lock(1)\n"
    "[2] started\n"
    "[2] mutex_lock(1) <waiting for thread 1> ...\n"
    "[1] mutex_unlock(1)\n"
    "[2] ... mutex_lock(1)\n"
    "[2] mutex_unlock(1)\n"
    "[2] finished (normal exit)\n"
    "[1] mutex_destroy(1)\n";

unsigned int const test_results_len = sizeof test_results;
bool const test_result_timeout = false;
