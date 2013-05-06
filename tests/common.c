#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include "common.h"

int main(int argc, char **argv, char **envp)
{
        (void)argc;

        char const *preload = getenv("LD_PRELOAD");
        if(!preload || !strstr(preload, "mutextrace.so"))
        {
                int fds[2];

                int rc = pipe(fds);
                if(rc == -1)
                {
                        perror("pipe()");
                        return 1;
                }

                pid_t child = fork();
                if(child == -1)
                {
                        perror("fork()");
                        return 1;
                }

                if(child == 0)
                {
                        dup2(fds[1], STDERR_FILENO);
                        close(fds[0]);
                        close(fds[1]);

                        char const *wrapper = "mutextrace";

                        char *const new_argv[] =
                        {
                                (char *)wrapper,
                                argv[0],
                                0
                        };

                        execve(wrapper, new_argv, envp);
                        _exit(1);
                }
                else
                {
                        close(fds[1]);

                        bool fail = false;

                        unsigned int offset = 0;

                        char buffer[64];
                        ssize_t count; 
                        while((count = read(fds[0], buffer, sizeof buffer)) > 0)
                        {
                                unsigned int i;
                                for(i = 0; i < count && offset < test_results_len; ++i, ++offset)
                                {
                                        if(buffer[i] != test_results[offset])
                                        {
                                                fail = true;
                                                break;
                                        }
                                }
                        }

                        fail = fail || (offset != test_results_len - 1);

                        if(fail)
                        {
                                unsigned int line = 1;
                                unsigned int col = 1;
                                unsigned int i;
                                for(i = 0; i < offset; ++i, ++col)
                                {
                                        if(test_results[i] == '\n')
                                        {
                                                ++line;
                                                col = 0;
                                        }
                                }
                                fprintf(stderr, "%s:%u:%u\n", argv[0], line, col);
                        }

                        int status;

                        waitpid(child, &status, 0);

                        if(fail && !status)
                                return 1;

                        return status;
                }
        }
        else
        {
                return test_main();
        }
}
