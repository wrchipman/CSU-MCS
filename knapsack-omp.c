#include <stdio.h>
#include <stdlib.h>
#include "timer.h"
#include <omp.h>


#define    MAX(x,y)   ((x)>(y) ? (x) : (y))

int *weights, *profits;     // weights and profits
int *solution;
int *prev, *cur;            // previous row and current row
int *tmp, *tmp1;

int DC(int start, int stop, int capacity)
{
  int m=0, n=0;
  int i=0, j=0;
  int c=0;
  int max=0;
  
  if(start==stop)
  {
    if(capacity>=weights[start]) 
    {
      solution[start] = 1;
      return profits[start];
    }
    else
    {
      solution[start] = 0;
      return 0;
    }
  }
  else
  {
    n = stop-start+1;
    m = start+n/2-1;
  }

#pragma omp parallel private(i,j)
{
  //The first half 
  if(weights[start]<=capacity){
  #pragma omp for
    for (j=0; j<weights[start]; j++) {
      prev[j] = 0;
    }
  #pragma omp for
    for (j=weights[start]; j<=capacity; j++) {
      prev[j] = profits[start];
    }
  }
  else
  {
  #pragma omp for
    for (j=0; j<=capacity; j++) {
      prev[j] = 0;
    }
  }
  for ( i=start+1 ; i <= m ; i++ ) {     // Enumerating objects 
    if(weights[i]<=capacity){
    #pragma omp for
      for ( j=0 ; j < weights[i]; j++ ) {cur[j] = prev[j];}
    #pragma omp for
      for ( j=weights[i] ; j <= capacity ; j++ ) {
	    cur[j] = MAX(prev[j],profits[i]+prev[j-weights[i]]);
      }
    }
    else
    {
    #pragma omp for
      for (j=0; j<=capacity; j++) {
        cur[j] = prev[j];
      }
    }
    #pragma omp single
    {
    tmp = prev; prev = cur; cur = tmp;  // Swap arrays
    }
  }
  #pragma omp for
  for(j=0 ; j<=capacity; j++ ) 
    tmp1[j] = prev[j];
  
  //The second half 
  if(weights[m+1]<=capacity){
  #pragma omp for
    for (j=0; j<weights[m+1]; j++) {
      prev[j] = 0;
    }
  #pragma omp for
    for (j=weights[m+1]; j<=capacity; j++) {
      prev[j] = profits[m+1];
    }
  }
  else
  {
  #pragma omp for
    for (j=0; j<=capacity; j++) {
      prev[j] = 0;
    }
  }
  for ( i=m+1+1 ; i <= stop ; i++ ) {     // Enumerating objects 
    if(weights[i]<=capacity){
    #pragma omp for
      for ( j=0 ; j < weights[i]; j++ ) {cur[j] = prev[j];}
    #pragma omp for
      for ( j=weights[i] ; j <= capacity ; j++ ) {
	    cur[j] = MAX(prev[j],profits[i]+prev[j-weights[i]]);
      }
    }
    else
    {
    #pragma omp for
      for (j=0; j<=capacity; j++) {
        cur[j] = prev[j];
      }
    }
    #pragma omp single
    {
    tmp = prev; prev = cur; cur = tmp;  // Swap arrays
    }
  }
}
  //Find the c*
  c = 0;
  max = 0;
  for(j=0; j<=capacity; j++)
    if((tmp1[j]+prev[capacity-j])>max)
    {
      max = tmp1[j]+prev[capacity-j];
      c = j;
    }
  
  DC(start, m, c);
  DC(m+1, stop, capacity-c);
  
  return max;
} 

int main(int argc, char **argv) {

   FILE   *fp;
   int    N, C;                   // # of objects, capacity 

   // Temp variables
   int    i, j, count, size;
   int maxProfit;

   // Time
   double time;

   // Read input file (# of objects, capacity, (per line) weight and profit )
   if ( argc > 1 ) {
      fp = fopen(argv[1], "r"); 
      if ( fp == NULL) {
         printf("[ERROR] : Failed to read file named '%s'.\n", argv[1]);
         exit(1);
      }
   } else {
      printf("USAGE : %s [filename].\n", argv[0]);
      exit(1);
   }
   
   fscanf(fp, "%d %d", &N, &C);
   printf("The number of objects is %d, and the capacity is %d.\n", N, C);

   size    = N * sizeof(int);
   weights = (int *)malloc(size);
   profits = (int *)malloc(size);
   solution = (int *)malloc(size);

   if ( weights == NULL || profits == NULL || solution == NULL) {
      printf("[ERROR] : Failed to allocate memory for weights/profits/solution.\n");
      exit(1);
   }

   for ( i=0 ; i < N ; i++ ) {
      count = fscanf(fp, "%d %d", &(weights[i]), &(profits[i]));
      if ( count != 2 ) {
         printf("[ERROR] : Input file is not well formatted.\n");
         exit(1);
      }
   }

   fclose(fp);
   // End of "Read input file"
   

   // Solve for the optimal profit
   size = (C+1) * sizeof(int);
   prev = (int *)malloc(size);
   cur  = (int *)malloc(size);
   tmp1 = (int *)malloc(size);
   if ( prev == NULL || cur == NULL ) {
      printf("[ERROR] : Failed to allocate memory for previous/current rows.\n");
      exit(1);
   }
   
   initialize_timer ();
   start_timer();

   // Call the D&C recursion function
   maxProfit = DC(0, N-1, C);

   stop_timer();
   time = elapsed_time ();

   printf("The optimal profit is %d Time taken : %lf.\n", maxProfit, time);
   // End of "Solve for the optimal profit"
   
#ifdef DEBUG
     // Output the solution vector 
     printf("Solution vector is: \n --> ");
     for (i=0 ; i<N ; i++ ) {
         printf("%d ", solution[i]);
     }
     printf("\n");
#endif

   // Clean up
   free(weights); free(profits);
   free(solution);
   free(prev); free(cur); free(tmp1);

   return 0;
}
