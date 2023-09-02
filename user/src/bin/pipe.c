/*******************************************************************************
 *  The BYTE UNIX Benchmarks - Release 3
 *          Module: pipe.c   SID: 3.3 5/15/91 19:30:20
 *
 *******************************************************************************
 * Bug reports, patches, comments, suggestions should be sent to:
 *
 *	Ben Smith, Rick Grehan or Tom Yager
 *	ben@bytepb.byte.com   rick_g@bytepb.byte.com   tyager@bytepb.byte.com
 *
 *******************************************************************************
 *  Modification Log:
 *  $Header: pipe.c,v 3.5 87/06/22 14:32:36 kjmcdonell Beta $
 *  August 29, 1990 - modified timing routines (ty)
 *  October 22, 1997 - code cleanup to remove ANSI C compiler warnings
 *                     Andy Kahn <kahn@zk3.dec.com>
 *
 ******************************************************************************/
char SCCSid[] = "@(#) @(#)pipe.c:3.3 -- 5/15/91 19:30:20";
/*
 *  pipe  -- test single process pipe throughput (no context switching)
 *
 */

#include <stdio.h>
#include <stdlib.h>
// #include <errno.h>

unsigned long iter;

void report()
{
	printf("COUNT|%ld|1|lps\n", iter);
	exit(0);
}

int main(argc, argv)
int	argc;
char	*argv[];
{
	char buf[512];
	int	pvec[2], duration;

	// duration = atoi(argv[1]);

	pipe(pvec);

	// wake_me(duration, report);
	iter = 0;

    for (int i = 0; i< 10; i++){
	// while (1) {
        printf("this turn before is %d\n", iter);
		if (write(pvec[1], buf, sizeof(buf)) != sizeof(buf)) {
			// if ((errno != EINTR) && (errno != 0))
				// printf("write failed, error \n");
			}
		if (read(pvec[0], buf, sizeof(buf)) != sizeof(buf)) {
			// if ((errno != EINTR) && (errno != 0))
				// printf("read failed, error %d\n", errno);
			}
		iter++;
        printf("this turn after is %d\n", iter);
	// }
    }
    exit(0);
}
