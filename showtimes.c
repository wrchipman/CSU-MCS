#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>

	/* Robbins & Robbins:  showtimes.c  */

static void showtimes (void)
{
    double ticks;
    struct tms tinfo;

    if ((ticks = (double) sysconf (_SC_CLK_TCK)) == -1)
        perror("Failed to determine clock ticks per second");
    else if (times(&tinfo) == (clock_t)-1)
        perror("Failed to get times information");
    else
    {
        fprintf(stderr, "User time:               %8.3f seconds\n",
            tinfo.tms_utime/ticks);
        fprintf(stderr, "System time:             %8.3f seconds\n",
            tinfo.tms_stime/ticks);
        fprintf(stderr, "Children's user time:    %8.3f seconds\n",
            tinfo.tms_cutime/ticks);
        fprintf(stderr, "Children's system time:  %8.3f seconds\n",
            tinfo.tms_cstime/ticks);
    }
}

int main(int argc, char *argv[])
{
    if (atexit(showtimes))
    {
        fprintf(stderr, "Failed to install showtimes exit handler\n");
        return 1;
    }
		    pid_t childpid = 0;
		    int i, n, j, k;

		    if (argc != 2)  /* check for valid number of command-line arguments */
		    {
			fprintf (stderr, "Usage: %s processes\n", argv[0]);
			return 1;
		    }

		    n = atoi(argv[1]);
		    for (i=1; i<n; i++)
			 if (childpid = fork())
				break;

		    fprintf (stderr, "i:%d process ID:%ld  parent ID:%ld child ID:%ld\n",
			i, (long)getpid(), (long)getppid(), (long)childpid );
			k= 100000000 * i;
		for (j=0; j<k; j++);

    return 0;
}

