#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char **argv, char **envp)
{
    unsigned int const max_args = 512;
    unsigned int const max_envs = 512;
    unsigned int const max_preload = 512;

    char *new_argv[max_args];
    char *new_envp[max_envs];
    char new_preload[max_preload];

    char pathbuf[PATH_MAX];
    char *path = 0;

    char **i, **o;
    char *ii, *oo;

    char added_preloadlib = 0;

    if(argc == 1)
    {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1;
    }

    i = argv + 1;
    o = new_argv;

    for(; argc; --argc, ++i, ++o)
        *o = *i;
    *o = 0;

    i = envp;
    o = new_envp;

    for(; *i; ++i, ++o)
    {
        if(strncmp(*i, "LD_PRELOAD=", 11))
        {
            *o = *i;
            if(!strncmp(*i, "PATH=", 5))
                path = *i + 5;
        }
        else
        {
            ii = *i;
            oo = new_preload;

            for(; *ii; ++ii, ++oo)
                *oo = *ii;

            *oo++ = ' ';
            strcpy(oo, PRELOADLIB);
            *o = new_preload;

            added_preloadlib = 1;
        }
    }

    if(!added_preloadlib)
    {
        strcpy(new_preload, "LD_PRELOAD=" PRELOADLIB);
        *o++ = new_preload;
    }

    *o = 0;

    if(!path || strchr(argv[1], '/'))
    {
        execve(argv[1], new_argv, new_envp);
        fprintf(stderr, "Failed to run %s: %s\n", argv[1], strerror(errno));
        return 1;
    }
    else
    {
        ii = path;

        while(*ii)
        {
            oo = pathbuf;
            for(; *ii && *ii != ':'; ++ii, ++oo)
                *oo = *ii;

            if(oo == pathbuf)
            {
                fprintf(stderr, "Empty element in PATH\n");
                return 1;
            }
            
            *oo++ = '/';

            strcpy(oo, argv[1]);
            if(!access(pathbuf, X_OK))
            {
                execve(pathbuf, new_argv, new_envp);
                fprintf(stderr, "Failed to run %s: %s\n", pathbuf, strerror(errno));
                return 1;
            }

            ++ii;
        }
    }

    fprintf(stderr, "Could not find %s in PATH\n", argv[1]);
    return 1;
}
