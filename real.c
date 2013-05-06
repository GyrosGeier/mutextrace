#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include "real.h"

#include <stdlib.h>

#include <link.h>
#include <string.h>

int (*real_create)(pthread_t *, pthread_attr_t const *, void *(*)(void *), void *);

int (*real_mutex_init)(pthread_mutex_t *, pthread_mutexattr_t const *);
int (*real_mutex_destroy)(pthread_mutex_t *);
int (*real_mutex_lock)(pthread_mutex_t *);
int (*real_mutex_unlock)(pthread_mutex_t *);

int (*real_cond_init)(pthread_cond_t *, pthread_condattr_t const *);
int (*real_cond_wait)(pthread_cond_t *, pthread_mutex_t *);
int (*real_cond_signal)(pthread_cond_t *);

#ifdef __mips__
#define OFFSET(x) ((x) + (map->l_addr))
#else
#define OFFSET(x) (x)
#endif

void init(void)
{
    static int initialized = 0;
    if(initialized)
        return;

    initialized = 1;

    struct link_map *map = _r_debug.r_map;

    if(!map)
        abort();

    for(; map; map = map->l_next)
    {
        if(strstr(map->l_name, "libpthread.so"))
        {
            ElfW(Dyn) *dyn;
            ElfW(Sym) *symtab = 0;
            char const *strtab = 0;
            unsigned int nsymbols = 0;

            for(dyn = map->l_ld; dyn->d_tag != DT_NULL; ++dyn)
            {
                if(dyn->d_tag == DT_SYMTAB)
                    symtab = (ElfW(Sym) *)OFFSET(dyn->d_un.d_ptr);
                else if(dyn->d_tag == DT_STRTAB)
                    strtab = (char const *)OFFSET(dyn->d_un.d_ptr);
                else if(dyn->d_tag == DT_HASH)
                    nsymbols = ((unsigned int *)OFFSET(dyn->d_un.d_ptr))[1];
            }

            if(!symtab || !strtab || !nsymbols)
                abort();

            for(; nsymbols; ++symtab, --nsymbols)
            {
                char const *name = strtab + symtab->st_name;
                void *value = (void *)symtab->st_value + map->l_addr;
                if(!strcmp(name, "pthread_create"))
                    real_create = value;
                else if(!strcmp(name, "pthread_mutex_init"))
                    real_mutex_init = value;
                else if(!strcmp(name, "pthread_mutex_destroy"))
                    real_mutex_destroy = value;
                else if(!strcmp(name, "pthread_mutex_lock"))
                    real_mutex_lock = value;
                else if(!strcmp(name, "pthread_mutex_unlock"))
                    real_mutex_unlock = value;
                else if(!strcmp(name, "pthread_cond_init"))
                    real_cond_init = value;
                else if(!strcmp(name, "pthread_cond_wait"))
                    real_cond_wait = value;
                else if(!strcmp(name, "pthread_cond_signal"))
                    real_cond_signal = value;
            }
        }
    }

    return;
}
