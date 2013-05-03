#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE 1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

int test_main(void);

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
                        dup2(STDERR_FILENO, fds[1]);
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

                        char buffer[64];
                        ssize_t count;
                        while((count = read(fds[0], buffer, sizeof buffer)) > 0)
                        {
                                write(STDERR_FILENO, buffer, count);
                        }

                        int status;

                        waitpid(child, &status, 0);

                        return status;
                }
        }
        else
        {
                return test_main();
        }
}
