#ifndef mutextrace_real_h_
#define mutextrace_real_h_ 1

#include <pthread.h>

extern int (*real_create)(pthread_t *, pthread_attr_t const *, void *(*)(void *), void *);

extern int (*real_mutex_init)(pthread_mutex_t *, pthread_mutexattr_t const *);
extern int (*real_mutex_destroy)(pthread_mutex_t *);
extern int (*real_mutex_lock)(pthread_mutex_t *);
extern int (*real_mutex_unlock)(pthread_mutex_t *);

extern int (*real_cond_init)(pthread_cond_t *, pthread_condattr_t const *);
extern int (*real_cond_wait)(pthread_cond_t *, pthread_mutex_t *);
extern int (*real_cond_signal)(pthread_cond_t *);

void init(void);

#endif
