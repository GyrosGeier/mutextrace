#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "store.h"
#include "real.h"

#include <stdio.h>

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    struct mutex *n = find_mutex(mutex);

    fprintf(stderr, "destroy #%u\n", n->num);

    return real_mutex_destroy(mutex);
}