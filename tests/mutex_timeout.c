#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "common.h"

#include <pthread.h>

int test_main(void)
{
        pthread_mutex_t mutex;

        pthread_mutex_init(&mutex, 0);
        pthread_mutex_lock(&mutex);
        pthread_mutex_lock(&mutex);

        return 0;
}

char const test_results[] =
        "[1] mutex_init(1, FAST)\n"
        "[1] mutex_lock(1)\n"
        "[1] mutex_lock(1) <waiting for thread 1> ...\n";

unsigned int const test_results_len = sizeof test_results;
bool const test_result_timeout = true;
