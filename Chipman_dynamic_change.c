#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define num_coins 8


main(int argc, char** argv){
  clock_t tbegin;
  clock_t tend;

  if (argc!=2){
    printf(" Use :\n");
    printf("%s <N>  \n",argv[0]);
    return EXIT_FAILURE;
  }//end if

  int N = atoi(argv[1]);

  int coins[] = {1, 2, 5, 10, 20, 25, 50, 100};
  int ways = 0;
  int j, i;
  double numberofways [N+1][2];
  for (j=0; j<N;j++)
	numberofways[j][0] = 1.0;
	numberofways[j+1][0] = 1.0;
  tbegin=clock();
  

  for(i=1; i<num_coins; i++){
	for(j=0;j<=N;j++){
		if (j>=coins[i]){
			numberofways[j][1] = numberofways[j][0] + numberofways[j-coins[i]][1];
			
		} else {
			numberofways[j][1] = numberofways[j][0];
			
	
		}
	}
	numberofways[j+1][1] = numberofways[j+1][0] + numberofways[j+1-coins[i]][1];
	for (j =0;j<=N;j++) numberofways[j][0] = numberofways[j][1];		
  }
	



  tend=clock();


  printf("For making change of %d cents:\n", N);
  printf("Total Ways %.0lf)\n", numberofways[N][1] + 1);
 
  float time =  (tend-tbegin)/(double)CLOCKS_PER_SEC;
  printf("Time: %.4lf s\n", time);



}//End main
