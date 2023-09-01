#include "stdio.h"
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <signal.h>

void wake_me(seconds, func)
	int seconds;
	void (*func)();
{
	/* set up the signal handler */
	// signal(14, func);
	/* get the clock running */
	// alarm(seconds);
}

unsigned long iter;

void report()
{
	printf("COUNT|%ld|1|lps\n", iter);
	exit(0);
}

int create_fd()
{
	int fd[2];

	if (pipe(fd) != 0 || close(fd[1]) != 0)
	    exit(1);

	return fd[0];
}

int main(argc, argv)
int	argc;
char	*argv[];
{
    char   *test;
	int	duration;
	int	fd;
    printf("exec syscall test\n");
    test = "close";

	duration = 10;

	iter = 0;
	wake_me(duration, report);

    switch (test[0]) {
        case 'm':
	        fd = create_fd();
	        while (1) {
		        close(dup(fd));
                getpid();
		        getuid();
		        // umask(022);
		        iter++;
	        }
	    /* NOTREACHED */
        case 'c':
           fd = create_fd();
           while (1) {
                close(dup(fd));
                iter++;
           }
           /* NOTREACHED */
        case 'g':
           while (1) {
                getpid();
                iter++;
           }
           /* NOTREACHED */
        case 'e':
           while (1) {
                int pid = fork();
                if (pid < 0) {
                    printf("%s: fork failed\n", argv[0]);
                    exit(1);
                } else if (pid == 0) {
                    printf("%s: exec /bin/true failed\n", argv[0]);
                    exit(1);
                } else {
                    if (waitpid(pid, NULL, 0) < 0) {
                        printf("%s: waitpid failed\n", argv[0]);
                        exit(1);
                    }
                }
                iter++;
           }
           /* NOTREACHED */
        }

        exit(9);
}

