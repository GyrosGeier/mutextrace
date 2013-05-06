#ifndef mutextrace_store_h_
#define mutextrace_store_h_ 1

#include <pthread.h>

enum state
{
    uninitialized = 0,
    unlocked = 1,
    locked = 2,
    destroyed = 3
};

struct mutex;

struct thread
{
    pthread_t thread;

    struct thread *left;
    struct thread *right;

    unsigned int num;

    struct mutex *owns;
};

struct mutex
{
    pthread_mutex_t *mutex;

    struct mutex *left;
    struct mutex *right;

    pthread_mutex_t lock;
    pthread_cond_t cond;

    unsigned int num;
    enum state state;
    int kind;

    /* Ownership tracking */
    struct thread *owner;
    struct mutex *owns_next;
};

struct thread *find_thread(pthread_t);
struct mutex *find_mutex(pthread_mutex_t *);

#endif
