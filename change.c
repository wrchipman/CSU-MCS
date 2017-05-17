#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define num_coins 8



//int make_change (int N, int* coins, int curr, int &ways) {
double make_change(int N, int* coins, int curr, int* ways){
if (curr == num_coins-1) {
	(*ways)++;
	return 1;
}

if (N == 0){
	(*ways)++;
	return 1;
}

double count = 1.0;


//If the largest value in the dom array is smaller than N then subtract dom from N and call make change with new value and the same curr coin. 
if (N >= coins[curr]){

//printf ("Top_make_change(%d, coins, %d);\n",N-coins[curr], coins[curr]);
	count += make_change(N-coins[curr], coins, curr, ways);
}

// Now choose to not use the current coin but instead call make_change with N and the next smaller coin
//printf ("Bottom_make_change(%d, coins, %d);\n",N, coins[curr+1]);
count += make_change(N, coins, ++curr, ways);



return count;
} // end make_change


main(int argc, char** argv){
  clock_t tbegin;
  clock_t tend;

  if (argc!=2){
    printf(" Use :\n");
    printf("%s <N>  \n",argv[0]);
    return EXIT_FAILURE;
  }//end if

  int N = atoi(argv[1]);

  int coins[] = {100, 50, 25, 20, 10, 5, 2, 1};

  
  double totalCount = 0.0;
  int i = 0;
  int *ways = &i;  
  
  tbegin=clock();
  totalCount =  make_change (N, coins, 0, ways);
  tend=clock();
  printf("For making change of %d cents:\n", N);
  printf("Total Function calls %.0lf)\n", totalCount);
  printf("Total ways %d)\n", i);
  float time =  (tend-tbegin)/(double)CLOCKS_PER_SEC;
  printf("Time: %.4lf s\n", time);

}//End main
