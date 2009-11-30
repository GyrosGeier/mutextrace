#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <pthread.h>

int main()
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
