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

                        bool seen_timeout = false;
                        bool fail = false;

                        unsigned int offset = 0;

                        char buffer[64];
                        while(offset < test_results_len)
                        {
                                fd_set readfds;
                                FD_ZERO(&readfds);
                                FD_SET(fds[0], &readfds);

                                struct timeval timeout = { 1, 0 };

                                int rc = select(fds[0] + 1, &readfds, 0, 0, &timeout);

                                if(rc == 0)
                                {
                                        kill(child, SIGKILL);
                                        seen_timeout = true;
                                        break;
                                }

                                ssize_t count = read(fds[0], buffer, sizeof buffer);

                                if(count == 0)
                                        break;
                                if(count == -1)
                                {
                                        fail = true;
                                        break;
                                }

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

                        fail = fail || (seen_timeout && !test_result_timeout);
                        fail = fail || (!seen_timeout && test_result_timeout);

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

                        if(seen_timeout)
                                return !(WIFSIGNALED(status) && WTERMSIG(status) == SIGKILL);

                        return status;
                }
        }
        else
        {
                return test_main();
        }
}
