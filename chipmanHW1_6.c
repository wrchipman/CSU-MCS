/*  Code from SGG:  Figure 3.29  */

#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>


int main()
{
    pid_t pid;
    int iNumber,z, i=0, j = 1, total=0;

/* fork a child process  */
    pid = fork();

    if (pid < 0)  {  /* error occurred  */
        fprintf(stderr, "Fork Failed");
        return 1;
    
    } else if (pid == 0)  {  /* child process  */
	printf("Enter a Number\n");
        scanf("%d", &iNumber);
	
	printf("Fibonacci Sequence for  %d\n", iNumber);     
        if (iNumber < 0) {
		printf ("The number must be 0 or larger");
	} else if (iNumber == 0) {
		printf ("%d ", i);
	} else if(iNumber == 1) {
		printf ("%d ", j);
	} else {
		printf("%d %d ", i, j);
		for (z = 2; z<iNumber; z++) {
			total = i+j;
			i = j;
			j = total;
			printf("%d ", total);
		}
		printf("\n\n");	 
        }

    } else   {  /* parent process  */
		wait();

    }

    return(0);
}
