#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include "common.h"

#include <pthread.h>

int test_main()
{
    pthread_mutex_t mutex;

    if(pthread_mutex_init(&mutex, 0))
        return 1;

    if(pthread_mutex_lock(&mutex))
        return 1;

    if(pthread_mutex_unlock(&mutex))
        return 1;

    if(pthread_mutex_destroy(&mutex))
        return 1;

    return 0;
}

char const test_results[] =
    "[1] mutex_init(1, FAST)\n"
    "[1] mutex_lock(1)\n"
    "[1] mutex_unlock(1)\n"
    "[1] mutex_destroy(1)\n";

unsigned int const test_results_len = sizeof test_results;
bool const test_result_timeout = false;
