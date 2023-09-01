// /*******************************************************************************
//  *  The BYTE UNIX Benchmarks - Release 3
//  *          Module: syscall.c   SID: 3.3 5/15/91 19:30:21
//  *
//  *******************************************************************************
//  * Bug reports, patches, comments, suggestions should be sent to:
//  *
//  *	Ben Smith, Rick Grehan or Tom Yager at BYTE Magazine
//  *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
//  *
//  *******************************************************************************
//  *  Modification Log:
//  *  $Header: syscall.c,v 3.4 87/06/22 14:32:54 kjmcdonell Beta $
//  *  August 29, 1990 - Modified timing routines
//  *  October 22, 1997 - code cleanup to remove ANSI C compiler warnings
//  *                     Andy Kahn <kahn@zk3.dec.com>
//  *
//  ******************************************************************************/
// /*
//  *  syscall  -- sit in a loop calling the system
//  *
//  */
// char SCCSid[] = "@(#) @(#)syscall.c:3.3 -- 5/15/91 19:30:21";

// #include <stdio.h>
// #include <stdlib.h>
// #include <unistd.h>
// #include <sys/stat.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <sys/syscall.h>
// #include <unistd.h>
// #include "timeit.c"

// unsigned long iter;

// void report()
// {
// 	fprintf(stderr,"COUNT|%ld|1|lps\n", iter);
// 	exit(0);
// }

// int create_fd()
// {
// 	int fd[2];

// 	if (pipe(fd) != 0 || close(fd[1]) != 0)
// 	    exit(1);

// 	return fd[0];
// }

// int main(argc, argv)
// int	argc;
// char	*argv[];
// {
//         char   *test;
// 	int	duration;
// 	int	fd;

// 	if (argc < 2) {
// 		fprintf(stderr,"Usage: %s duration [ test ]\n", argv[0]);
//                 fprintf(stderr,"test is one of:\n");
//                 fprintf(stderr,"  \"mix\" (default), \"close\", \"getpid\", \"exec\"\n");
// 		exit(1);
// 	}
//         if (argc > 2)
//             test = argv[2];
//         else
//             test = "mix";

// 	duration = atoi(argv[1]);

// 	iter = 0;
// 	wake_me(duration, report);

//         switch (test[0]) {
//         case 'm':
// 	   fd = create_fd();
// 	   while (1) {
// 		close(dup(fd));
// 		syscall(SYS_getpid);
// 		getuid();
// 		umask(022);
// 		iter++;
// 	   }
// 	   /* NOTREACHED */
//         case 'c':
//            fd = create_fd();
//            while (1) {
//                 close(dup(fd));
//                 iter++;
//            }
//            /* NOTREACHED */
//         case 'g':
//            while (1) {
//                 syscall(SYS_getpid);
//                 iter++;
//            }
//            /* NOTREACHED */
//         case 'e':
//            while (1) {
//                 pid_t pid = fork();
//                 if (pid < 0) {
//                     fprintf(stderr,"%s: fork failed\n", argv[0]);
//                     exit(1);
//                 } else if (pid == 0) {
//                     execl("/bin/true", "/bin/true", (char *) 0);
//                     fprintf(stderr,"%s: exec /bin/true failed\n", argv[0]);
//                     exit(1);
//                 } else {
//                     if (waitpid(pid, NULL, 0) < 0) {
//                         fprintf(stderr,"%s: waitpid failed\n", argv[0]);
//                         exit(1);
//                     }
//                 }
//                 iter++;
//            }
//            /* NOTREACHED */
//         }

//         exit(9);
// }

/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: syscall.c   SID: 3.3 5/15/91 19:30:21
 *
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yager at BYTE Magazine
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  $Header: syscall.c,v 3.4 87/06/22 14:32:54 kjmcdonell Beta $
 *  August 29, 1990 - Modified timing routines
 *  October 22, 1997 - code cleanup to remove ANSI C compiler warnings
 *                     Andy Kahn <kahn@zk3.dec.com>
 *
 ******************************************************************************/
/*
 *  syscall  -- sit in a loop calling the system
 *
 */
char SCCSid[] = "@(#) @(#)syscall.c:3.3 -- 5/15/91 19:30:21";

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    int pipe_ans = pipe(fd);
    printf("pipe ans is %d\n", pipe_ans);
    printf("fd 0 is %d, fd 1 is %d\n", fd[0], fd[1]); 
    int close_ans = close(fd[1]); 

    printf("close ans is %d\n", close_ans);

	if (pipe_ans != 0 || close_ans != 0)
	    exit(1);

    printf("fd create over\n");

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
	// wake_me(duration, report);

    switch (test[0]) {
        case 'm':
	        fd = create_fd();
	        // while (1) {
                dup(fd);
		        // close(dup(fd));
                // printf("close over\n");
                // getpid();
                // printf("get pid over\n");
		        // getuid();
                // printf("get uid over\n");
		        // umask(022);
		        iter++;
                printf("this term iter is %d\n", iter);
	        // }
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
                    if (waitpid(pid, 0) < 0) {
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

