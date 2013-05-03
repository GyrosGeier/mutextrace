#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include <pthread.h>

#include <unistd.h>

void *thread_func(void *p)
{
    pthread_mutex_t *mutex = (pthread_mutex_t *)p;

    pthread_mutex_lock(mutex);
    pthread_mutex_unlock(mutex);

    return 0;
}

int main()
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
