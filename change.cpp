#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define num_coins 4



//int make_change (int N, int* coins, int curr, int &ways) {
int make_change(int N, int* coins, int curr, int ways){
if (curr == num_coins-1) return 1;
//printf ("2\n");
if (N == 0){
	ways++;
	return 1;
}
//printf ("3\n");
int count = 1;

//printf ("4\n");
//If the largest value in the dom array is smaller than N then subtract dom from N and call make change with new value and the same curr coin. 
if (N >= coins[curr]){
//printf ("6\n");
printf ("Top_make_change(%d, coins, %d);\n",N-coins[curr], coins[curr]);
	count += make_change(N-coins[curr], coins, curr, ways);
}
//printf ("5\n");
// Now choose to not use the current coin but instead call make_change with N and the next smaller coin
printf ("Bottom_make_change(%d, coins, %d);\n",N, coins[curr+1]);
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

  int coins[] = {25, 10, 5, 1};

  //printf ("1\n");
  int totalCount = 0;
  int ways = 0;  
  totalCount =  make_change (N, coins, 0, ways);

  printf("Total Function calls %d)\n", totalCount);


}//End main
