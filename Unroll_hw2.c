#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

void randomInit(float* data, int size){
  int i;
  for (i = 0; i < size; ++i)
    data[i] = rand() / (float)RAND_MAX;
    
}
void zeroInit(float* data, int size){
  int i;
  for (i = 0; i < size; i++)
    data[i] = 0;
}

int compareMatrix(float* H, float* J, int size){
  int i;
  for (i = 0; i < size; i++)
	if (H[i] != J[i])
		return FALSE;
  return TRUE;
}

void transposeMatrix(float* K, int size){

   int i, j;
   float g;
   for (i = 0; i < size - 1; i++)
	for (j = i + 1; j < size; j++){
	  g = K[i * size + j];
	  K[i * size + j] = K[j * size + i];
	  K[j * size + i] = g;
	}

/*for (i = 0; i <size * size; i++)
	printf ("%f, ", K[i]);*/

}

main(int argc, char** argv){
  clock_t tbegin;
  clock_t tend;

  if (argc!=2){
    printf(" Use :\n");
    printf("%s <N>  \n",argv[0]);
    return EXIT_FAILURE;
  }

  int N = atoi(argv[1]);
  int i,j,k;

  //allocate matrices
  unsigned int mem_size = sizeof(float) * N * N;
  float* A = (float*) malloc(mem_size);
  float* B = (float*) malloc(mem_size);
  float* C = (float*) malloc(mem_size);
  float* D = (float*) malloc(mem_size);

  // initialize
  randomInit(A, N*N);
  randomInit(B, N*N);
  zeroInit(C, N*N);
  zeroInit(D, N*N);


  tbegin=clock();
  //compute matrix multiplication
    for (i = 0; i < N; ++i)
      for (j = 0; j < N; ++j) {
        float sum = 0;
        for (k = 0; k < N; ++k) {
	  sum += A[i * N + k] * B[k * N + j];
        }
        C[i * N + j] = sum;
      }
   tend=clock();
  float time =  (tend-tbegin)/(double)CLOCKS_PER_SEC;
  printf ("Non-optimized code\n");
  printf("Time: %.4lf s\n", time);
  printf("Gflops: %f\n", 2*N* ((N*N/time) / 1e9));


/*for (i = 0; i <N *N; i++)
	printf ("%f, ", C[i]);*/
printf("\n");
  tbegin=clock();
  //compute matrix multiplication
    for (i = 0; i < N; ++i)
      for (j = 0; j < N; ++j) {
        float sum = 0;
        for (k = 0; k < (N/8)*8; k+=8) {
	  sum = sum + A[i * N + k] * B[k * N + j] +
		A[i * N + k + 1] * B[(k + 1) * N + j] +
		A[i * N + k + 2] * B[(k + 2) * N + j] +
		A[i * N + k + 3] * B[(k + 3) * N + j] +
		A[i * N + k + 4] * B[(k + 4) * N + j] +
		A[i * N + k + 5] * B[(k + 5) * N + j] +
		A[i * N + k + 6] * B[(k + 6) * N + j] +
                A[i * N + k + 7] * B[(k + 7) * N + j];
        }
	for (k = (N/8)*8 +1; k< N; ++k)
		sum += A[i * N + k] * B[k * N + j];
        D[i * N + j] = sum;
      }
   tend=clock();


  time =  (tend-tbegin)/(double)CLOCKS_PER_SEC;
  printf ("Unrolled Matrix B code\n");
  printf("Time: %.4lf s\n", time);
  printf("Gflops: %f\n", 2*N* ((N*N/time) / 1e9));

/* for (i = 0; i <N *N; i++)
	printf ("%f, ", D[i]);*/
printf("\n");

if (compareMatrix(C, D, N*N))
	printf("Matrix match confirmed\n");
else
	printf("Matrix compare failed\n");
  
  
}



