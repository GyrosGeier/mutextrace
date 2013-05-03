#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include "store.h"
#include "real.h"

#include <stdlib.h>

struct thread *create_thread(void)
{
    static unsigned int num;

    struct thread *new_thread = (struct thread *)malloc(sizeof(struct thread));
    new_thread->left = 0;
    new_thread->right = 0;
    new_thread->num = ++num;
    new_thread->owns = 0;

    return new_thread;
}

struct thread *find_thread(pthread_t thread)
{
    static struct thread *root;

    if(!root)
    {
        root = create_thread();
        root->thread = thread;
        return root;
    }

    struct thread *current = root;

    for(;;)
    {
        if(current->thread == thread)
            return current;

        if(current->thread > thread)
        {
            if(!current->left)
            {
                current->left = create_thread();
                current->left->thread = thread;
            }
            current = current->left;
        }
        else
        {
            if(!current->right)
            {
                current->right = create_thread();
                current->right->thread = thread;
            }
            current = current->right;
        }
    }
}

struct mutex *create_mutex(void)
{
    init();

    static unsigned int num;

    struct mutex *new_mutex = (struct mutex *)malloc(sizeof(struct mutex));
    new_mutex->left = 0;
    new_mutex->right = 0;
    real_mutex_init(&new_mutex->lock, 0);
    real_cond_init(&new_mutex->cond, 0);
    new_mutex->num = ++num;
    new_mutex->state = uninitialized;
    new_mutex->owner = 0;
    new_mutex->owns_next = 0;

    return new_mutex;
}

struct mutex *find_mutex(pthread_mutex_t *mutex)
{
    static struct mutex *root;

    if(!root)
    {
        root = create_mutex();
        root->mutex = mutex;
        return root;
    }

    struct mutex *current = root;

    for(;;)
    {
        if(current->mutex == mutex)
            return current;

        if(current->mutex > mutex)
        {
            if(!current->left)
            {
                current->left = create_mutex();
                current->left->mutex = mutex;
            }
            current = current->left;
        }
        else
        {
            if(!current->right)
            {
                current->right = create_mutex();
                current->right->mutex = mutex;
            }
            current = current->right;
        }
    }
}
